from PIL import Image
import matplotlib.pyplot as plt
import matplotlib.image as mpimg
import numpy as np
import cv2
import pywt
import math

#  RGB -> YUV


img_path = './data/CoverImg.png'
img_ori = cv2.imread(img_path)
img_wm = cv2.imread("./data/img_wm_0.png")

def extract(img_ori, img_wm):
    length, width, height = img_wm.shape

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

    image_ori_ycbcr = rgb2ycbcr(img_ori)
    image_wm_ycbcr = rgb2ycbcr(img_wm)

    y_ori = image_ori_ycbcr[:,:,0]
    Cb_ori = image_ori_ycbcr[:,:,1]
    Cr_ori = image_ori_ycbcr[:,:,2]

    y_ori = np.reshape(y_ori,(length, width))
    Cb_ori = np.reshape(Cb_ori,(length, width))
    Cr_ori = np.reshape(Cr_ori,(length, width))

    y_wm = image_wm_ycbcr[:,:,0]
    Cb_wm = image_wm_ycbcr[:,:,1]
    Cr_wm = image_wm_ycbcr[:,:,2]

    y_wm = np.reshape(y_wm,(length, width))
    Cb_wm = np.reshape(Cb_wm,(length, width))
    Cr_wm = np.reshape(Cr_wm,(length, width))

    #  Apply DWT to Y and Y'

    [L_ori, H_ori] = pywt.dwt2(y_ori, 'haar', axes=(-2, -1))
    [L_wm, H_wm] = pywt.dwt2(y_wm, 'haar', axes=(-2, -1))

    #  Divide into 4*4 block and apply dct to each block B and B'

    length1, width1 = L_ori.shape
    length1 = int(length1)
    width1 = int(width1)
    block = np.ones((4, 4)) * 0
    r = int(length1/4 * width1/4)
    B_ori = np.ones((r,4,4)) * 0
    k = 0
    for i in range(0, length1, 4):
        for j in range(0, width1, 4):
            block = L_ori[i : i+4, j : j+4]
            img_dct = cv2.dct(block)
            B_ori[k,0:4,0:4] = img_dct
            k = k + 1

    B_wm = np.ones((r,4,4)) * 0
    k = 0
    for i in range(0, length1, 4):
        for j in range(0, width1, 4):
            block = L_wm[i : i+4, j : j+4]
            img_dct = cv2.dct(block)
            B_wm[k,0:4,0:4] = img_dct
            k = k + 1

    #  Apply SVD to each B, resulting U,S,V

    U_ori = np.ones((r, 4, 4)) * 0
    S_ori = np.ones((r, 4, 4)) * 0
    V_ori = np.ones((r, 4, 4)) * 0

    z_svd_ori = np.ones((4, 4)) * 0
    u_svd_ori = np.ones((4, 4)) * 0
    s_svd_ori = np.ones((4, 4)) * 0
    v_svd_ori = np.ones((4, 4)) * 0
    mid_ori = np.ones((4, 4)) * 0
    for k in range(0, r):
        z_svd_ori = np.reshape(B_ori[k,:,:],(4,4))
        u_svd_ori, s_svd_ori, v_svd_ori= np.linalg.svd(z_svd_ori)
        U_ori[k,:,:] = np.reshape(u_svd_ori,(1,4,4))
        for i in range(0,4):
            mid_ori[i,i] = s_svd_ori[i]
            S_ori[k, :, :] = np.reshape(mid_ori, (1, 4, 4))
            V_ori[k, :, :] = np.reshape(v_svd_ori, (1, 4, 4))

    U_wm = np.ones((r, 4, 4)) * 0
    S_wm = np.ones((r, 4, 4)) * 0
    V_wm = np.ones((r, 4, 4)) * 0

    z_svd_wm = np.ones((4, 4)) * 0
    u_svd_wm = np.ones((4, 4)) * 0
    s_svd_wm = np.ones((4, 4)) * 0
    v_svd_wm = np.ones((4, 4)) * 0
    mid_wm = np.ones((4, 4)) * 0

    for k in range(0, r):
        z_svd_wm = np.reshape(B_wm[k,:,:],(4,4))
        u_svd_wm, s_svd_wm, v_svd_wm= np.linalg.svd(z_svd_wm)
        U_wm[k,:,:] = np.reshape(u_svd_wm,(1,4,4))
        for i in range(0,4):
            mid_wm[i,i] = s_svd_wm[i]
            S_wm[k, :, :] = np.reshape(mid_wm, (1, 4, 4))
            V_wm[k, :, :] = np.reshape(v_svd_wm, (1, 4, 4))


    wm_path = './data/wm_level2.png'
    wm = cv2.imread(wm_path)

    S_new_ori = np.ones((r, 4, 4)) * 0
    wm_array = np.array(wm)
    B_new_ori = np.ones((r, 4, 4)) * 0
    B_inter = np.ones((r, 4, 4)) * 0

    for k in range(0, r):
        for i in range(0, int(length1/4)):
            for j in range(0, int(width1/4)):
                S_new_ori[k,:,:] = S_ori[k,:,:]

    for k in range(0, r):
        B_inter[k,:,:] = np.dot(U_ori[k,:,:],S_new_ori[k,:,:])
        B_new_ori[k,:,:] = np.dot(B_inter[k,:,:],V_ori[k,:,:])

    S_new_wm = np.ones((r, 4, 4)) * 0
    wm_array = np.array(wm)
    B_new_wm = np.ones((r, 4, 4)) * 0
    B_inter = np.ones((r, 4, 4)) * 0

    for k in range(0, r):
        for i in range(0, int(length1/4)):
            for j in range(0, int(width1/4)):
                S_new_wm[k,:,:] = S_wm[k,:,:]

    for k in range(0, r):
        B_inter[k,:,:] = np.dot(U_wm[k,:,:],S_new_wm[k,:,:])
        B_new_wm[k,:,:] = np.dot(B_inter[k,:,:],V_wm[k,:,:])

    S_delta = S_new_wm - S_new_ori
    count = np.ones((1,4096)) * 0
    wm_ext = np.ones((64,64)) * 0
    #print(S_delta)

    for k in range(0, r):
        if np.abs(S_delta[k,0,0]) <= 1:
            count[0,k] = 0
        else:
            count[0,k] = 255

    #print(count)

    for i in range(0,64):
        for j in range(0,64):
            wm_ext[i,j] = count[0, 64 * i + j]

    plt.imshow(wm_ext)
    plt.show()

    cv2.imwrite("data/wm.png", wm_ext)
    return wm_ext

extract(img_ori, img_wm)