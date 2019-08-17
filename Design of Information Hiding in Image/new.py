from PIL import Image
import matplotlib.pyplot as plt
import matplotlib.image as mpimg
import numpy as np
import cv2
import pywt
import math

#  Scaling factor

k_para = 0.01

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

y = image_ycbcr[:,:,0]
Cb = image_ycbcr[:,:,1]
Cr = image_ycbcr[:,:,2]

y = np.reshape(y,(length, width))
Cb = np.reshape(Cb,(length, width))
Cr = np.reshape(Cr,(length, width))

[LLy, Hy] = pywt.dwt2(y, 'haar', axes=(-2, -1))
[LLcb, Hcb] = pywt.dwt2(Cb, 'haar', axes=(-2, -1))
[LLcr, Hcr] = pywt.dwt2(Cr, 'haar', axes=(-2, -1))

length1, width1 = LLy.shape
length1 = int(length1)
width1 = int(width1)
block = np.ones((4, 4)) * 0
r = int(length1/4 * width1/4)
z_y = np.ones((r,4,4)) * 0
k = 0
for i in range(0, length1, 4):
    for j in range(0, width1, 4):
        block = LLy[i : i+4, j : j+4]
        img_dct = cv2.dct(block)
        z_y[k,0:4,0:4] = img_dct
        k = k + 1

length1, width1 = LLcb.shape
length1 = int(length1)
width1 = int(width1)
block = np.ones((4, 4)) * 0
r = int(length1/4 * width1/4)
z_cb = np.ones((r,4,4)) * 0
k = 0
for i in range(0, length1, 4):
    for j in range(0, width1, 4):
        block = LLcb[i : i+4, j : j+4]
        img_dct = cv2.dct(block)
        z_cb[k,0:4,0:4] = img_dct
        k = k + 1

length1, width1 = LLcr.shape
length1 = int(length1)
width1 = int(width1)
block = np.ones((4, 4)) * 0
r = int(length1/4 * width1/4)
z_cr = np.ones((r,4,4)) * 0
k = 0
for i in range(0, length1, 4):
    for j in range(0, width1, 4):
        block = LLcr[i : i+4, j : j+4]
        img_dct = cv2.dct(block)
        z_cr[k,0:4,0:4] = img_dct
        k = k + 1


U_y = np.ones((r, 4, 4)) * 0
S_y = np.ones((r, 4, 4)) * 0
V_y = np.ones((r, 4, 4)) * 0

z_svd= np.ones((4, 4)) * 0
u_svd = np.ones((4, 4)) * 0
s_svd = np.ones((4, 4)) * 0
v_svd = np.ones((4, 4)) * 0
mid = np.ones((4, 4)) * 0
for k in range(0, r):
    z_svd = np.reshape(z_y[k,:,:],(4,4))
    u_svd, s_svd, v_svd= np.linalg.svd(z_svd)
    U_y[k,:,:] = np.reshape(u_svd,(1,4,4))
    for i in range(0,4):
        mid[i,i] = s_svd[i]
    S_y[k, :, :] = np.reshape(mid, (1, 4, 4))
    V_y[k, :, :] = np.reshape(v_svd, (1, 4, 4))

U_cb = np.ones((r, 4, 4)) * 0
S_cb = np.ones((r, 4, 4)) * 0
V_cb = np.ones((r, 4, 4)) * 0

z_svd= np.ones((4, 4)) * 0
u_svd = np.ones((4, 4)) * 0
s_svd = np.ones((4, 4)) * 0
v_svd = np.ones((4, 4)) * 0
mid = np.ones((4, 4)) * 0
for k in range(0, r):
    z_svd = np.reshape(z_cb[k,:,:],(4,4))
    u_svd, s_svd, v_svd= np.linalg.svd(z_svd)
    U_cb[k,:,:] = np.reshape(u_svd,(1,4,4))
    for i in range(0,4):
        mid[i,i] = s_svd[i]
    S_cb[k, :, :] = np.reshape(mid, (1, 4, 4))
    V_cb[k, :, :] = np.reshape(v_svd, (1, 4, 4))

U_cr = np.ones((r, 4, 4)) * 0
S_cr = np.ones((r, 4, 4)) * 0
V_cr = np.ones((r, 4, 4)) * 0

z_svd= np.ones((4, 4)) * 0
u_svd = np.ones((4, 4)) * 0
s_svd = np.ones((4, 4)) * 0
v_svd = np.ones((4, 4)) * 0
mid = np.ones((4, 4)) * 0
for k in range(0, r):
    z_svd = np.reshape(z_cr[k,:,:],(4,4))
    u_svd, s_svd, v_svd= np.linalg.svd(z_svd)
    U_cr[k,:,:] = np.reshape(u_svd,(1,4,4))
    for i in range(0,4):
        mid[i,i] = s_svd[i]
    S_cr[k, :, :] = np.reshape(mid, (1, 4, 4))
    V_cr[k, :, :] = np.reshape(v_svd, (1, 4, 4))

