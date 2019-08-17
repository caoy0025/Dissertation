from PIL import Image
import matplotlib.pyplot as plt
import matplotlib.image as mpimg
import numpy as np
import cv2
import pywt
import math

#  Scaling factor

k_para = 0

#  RGB -> YUV

img_path = './data/CoverImg.png'
img_ori = cv2.imread(img_path)
img = cv2.imread(img_path)
length, width, height = img.shape

mat = np.array(
    [[65.481, 128.553, 24.966],
     [-37.797, -74.203, 112.0],
     [112.0, -93.786, -18.214]])
mat_inv = np.linalg.inv(mat)
offset = np.array([16, 128, 128])


def rgb2ycbcr(rgb_img):
    ycbcr_img = np.zeros(rgb_img.shape)
    for x in range(rgb_img.shape[0]):
        for y in range(rgb_img.shape[1]):
            ycbcr_img[x, y, :] = np.round(np.dot(mat, rgb_img[x, y, :] * 1.0 / 255) + offset)
    return ycbcr_img

def ycbcr2rgb(ycbcr_img):
    rgb_img = np.zeros(ycbcr_img.shape, dtype=np.uint8)
    for x in range(ycbcr_img.shape[0]):
        for y in range(ycbcr_img.shape[1]):
            [r, g, b] = ycbcr_img[x,y,:]
            rgb_img[x, y, :] = np.maximum(0, np.minimum(255, np.round(np.dot(mat_inv, ycbcr_img[x, y, :] - offset) * 255.0)))
    return rgb_img

image_ycbcr = rgb2ycbcr(img)

'''
image_ycbcr = image_ycbcr / 255
plt.subplot(2,2,1)     #将窗口分为两行两列四个子图，则可显示四幅图片
plt.title('origin image')   #第一幅图片标题
plt.imshow(img)      #绘制第一幅图片

plt.subplot(2,2,2)     #第二个子图
plt.title('Y channel')   #第二幅图片标题
plt.imshow(image_ycbcr[:,:,0],plt.cm.gray)      #绘制第二幅图片,且为灰度图
plt.axis('off')     #不显示坐标尺寸

plt.subplot(2,2,3)     #第三个子图
plt.title('Cb channel')   #第三幅图片标题
plt.imshow(image_ycbcr[:,:,1],plt.cm.gray)      #绘制第三幅图片,且为灰度图
plt.axis('off')     #不显示坐标尺寸

plt.subplot(2,2,4)     #第四个子图
plt.title('Cr channel')   #第四幅图片标题
plt.imshow(image_ycbcr[:,:,2],plt.cm.gray)      #绘制第四幅图片,且为灰度图
plt.axis('off')     #不显示坐标尺寸

plt.show()   #显示窗口
'''

y = image_ycbcr[:,:,0]
Cb = image_ycbcr[:,:,1]
Cr = image_ycbcr[:,:,2]

y = np.reshape(y,(length, width))
Cb = np.reshape(Cb,(length, width))
Cr = np.reshape(Cr,(length, width))

#print("Y is:")
#print(y)

#  Apply DWT to Y

[L, H] = pywt.dwt2(y, 'haar', axes=(-2, -1))
print("L is:")
print(L)

#  Divide into 4*4 block and apply dct to each block "B"

length1, width1 = L.shape
length1 = int(length1)
width1 = int(width1)
block = np.ones((4, 4)) * 0
r = int(length1/4 * width1/4)
z = np.ones((r,4,4)) * 0
k = 0
for i in range(0, length1, 4):
    for j in range(0, width1, 4):
        block = L[i : i+4, j : j+4]
        img_dct = cv2.dct(block)
        z[k,0:4,0:4] = img_dct
        k = k + 1
print("z is:")
print(z)

#  Apply SVD to each B, resulting U,S,V

U = np.ones((r, 4, 4)) * 0
S = np.ones((r, 4, 4)) * 0
V = np.ones((r, 4, 4)) * 0

z_svd= np.ones((4, 4)) * 0
u_svd = np.ones((4, 4)) * 0
s_svd = np.ones((4, 4)) * 0
v_svd = np.ones((4, 4)) * 0
mid = np.ones((4, 4)) * 0
for k in range(0, r):
    z_svd = np.reshape(z[k,:,:],(4,4))
    u_svd, s_svd, v_svd= np.linalg.svd(z_svd)
    U[k,:,:] = np.reshape(u_svd,(1,4,4))
    for i in range(0,4):
        mid[i,i] = s_svd[i]
    S[k, :, :] = np.reshape(mid, (1, 4, 4))
    V[k, :, :] = np.reshape(v_svd, (1, 4, 4))


#print("S is:")
#print(S)
#for k in range(0, r):
#    print(S[k,0,0])

#  Read watermark image W

wm_path = './data/wm_level2.png'
wm = cv2.imread(wm_path)

#  Embed W to the cover image

S_new = np.ones((r, 4, 4)) * 0
wm_array = np.array(wm)
B_new = np.ones((r, 4, 4)) * 0
B_inter = np.ones((r, 4, 4)) * 0

kk = 0
for i in range(0, int(length1/4)):
    for j in range(0, int(width1/4)):
        S_new[kk,:,:] = S[kk,:,:]
        S_new[kk,0,0] = S[kk,0,0] * (1 + k_para * wm_array[i,j,0])
        kk = kk+1

#print("S_new is:")

#for k in range(0, r):
#    print(S_new[k,0,0])

for k in range(0, r):
    B_inter[k,:,:] = np.dot(U[k,:,:],S_new[k,:,:])
    B_new[k,:,:] = np.dot(B_inter[k,:,:],V[k,:,:])

#print("B_new is:")
#print(B_new)

#  Apply IDCT and IDWT

D_new = np.ones((r, 4, 4)) * 0
Y_new = np.ones((r, 4, 4)) * 0

for k in range(0, r):
    D_new[k,:,:] = cv2.idct(B_new[k,:,:])

#print(D_new)
#print("IDCT successful")

#  reshape

sq = int(math.sqrt(r))

b = np.ones((sq * 4,sq * 4)) * 0
c = np.ones((sq * 4,sq * 4)) * 0
b = np.reshape(D_new,(1,r * 4 * 4))
for k in range(0,r):
    for i in range(0,4):
        for j in range(0,4):
            c[int(k/sq) * 4 + j, k % sq * 4 + i] = b[0, i + 4 * j + 16 * k]

#print("L_new is:")
#print(c)

Y_new = pywt.idwt2([c, H], 'haar')
Y_new = Y_new

#print("Y_new")
#print(Y_new)

image_ycbcr_new = np.ones((length, width, height)) * 0
image_ycbcr_new[:,:,0] = Y_new
image_ycbcr_new[:,:,1] = Cb
image_ycbcr_new[:,:,2] = Cr

image_wm = ycbcr2rgb(image_ycbcr_new)
#print(image_wm)

plt.subplot(2,1,1)     #将窗口分为两行两列四个子图，则可显示四幅图片
plt.title('origin image')   #第一幅图片标题
plt.imshow(img_ori)      #绘制第一幅图片

plt.subplot(2,1,2)     #第二个子图
plt.title('Watermarked image')   #第二幅图片标题
plt.imshow(image_wm)      #绘制第二幅图片,且为灰度图
plt.show()

cv2.imwrite("./data/img_wm_0.png", image_wm)
