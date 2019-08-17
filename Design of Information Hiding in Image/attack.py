import copy
import cv2
import time
import math
import glob
import numpy as np
import matplotlib.pyplot as plt
import extraction

img_wm = cv2.imread("./data/img_wm.png")
img_ori = cv2.imread("./data/CoverImg.png")

attack_list ={}
attack_list['ori']          = 'Ori'
attack_list['blur']         = 'blur'        #模糊处理
attack_list['rotate180']    = 'rotate180'   #旋转180
attack_list['rotate90']     = 'rotate90'    #旋转90
attack_list['saltnoise']    = 'saltnoise'   #散粒噪声
attack_list['randline']     = 'randline'    #随机加线条
attack_list['randcover']    = 'randcover'   #随机覆盖
attack_list['brighter10']   = 'brighter10%' #亮度+10%
attack_list['darker10']     = 'darker10%'   #亮度-10%
attack_list['redgray']      = 'redgray'     #红色灰度处理

type = 'blur'

def nc(im1, im2):
    a = np.array(im1)
    b = np.array(im2)
    ab = a * b
    aa = a * a
    bb = b * b
    nc_value = (ab.sum() * ab.sum())/(aa.sum() * bb.sum())
    return nc_value

def psnr(im1, im2):
    if im1.shape != im2.shape or len(im2.shape) < 2:
        return 0

    di = im2.shape[0] * im2.shape[1]
    if len(im2.shape) == 3:
        di = im2.shape[0] * im2.shape[1] * im2.shape[2]

    diff = np.abs(im1 - im2)
    rmse = np.sum(diff * diff) / di
    print(rmse)
    psnr = 20 * np.log10(255 / rmse)
    return psnr

def rotate_about_center(src, angle, scale=1.):
    w = src.shape[1]
    h = src.shape[0]
    rangle = np.deg2rad(angle)  # angle in radians
    nw = (abs(np.sin(rangle)*h) + abs(np.cos(rangle)*w))*scale
    nh = (abs(np.cos(rangle)*h) + abs(np.sin(rangle)*w))*scale
    rot_mat = cv2.getRotationMatrix2D((nw*0.5, nh*0.5), angle, scale)
    rot_move = np.dot(rot_mat, np.array([(nw-w)*0.5, (nh-h)*0.5,0]))
    rot_mat[0,2] += rot_move[0]
    rot_mat[1,2] += rot_move[1]
    return cv2.warpAffine(src, rot_mat, (int(math.ceil(nw)), int(math.ceil(nh))), flags=cv2.INTER_LANCZOS4)

def attack(img,type):

    if type == "ori":
        return img

    if type == "gaussian_blur":
        return cv2.GaussianBlur(img,(3,3),1.8)

    if type == "blur":
        kernel = np.ones((5,5),np.float32)/25
        return cv2.filter2D(img,-1,kernel)

    if type=="rotate180":
        return rotate_about_center(img,180)

    if type == "rotate90":
        return rotate_about_center(img, 90)

    if type == "redgray":
        return  img[:,:,0]

    if type == "saltnoise":
        for k in range(1000):
            i = int(np.random.random() * img.shape[1])
            j = int(np.random.random() * img.shape[0])
            if img.ndim == 2:
                img[j, i] = 255
            elif img.ndim == 3:
                img[j, i, 0] = 255
                img[j, i, 1] = 255
                img[j, i, 2] = 255
        return img

    if type == "randline":
        cv2.rectangle(img, (384, 0), (510, 128), (0, 255, 0), 3)
        cv2.rectangle(img, (0, 0), (300, 128), (255, 0, 0), 3)
        cv2.line(img, (0, 0), (511, 511), (255, 0, 0), 5)
        cv2.line(img, (0, 511), (511, 0), (255, 0, 255), 5)

        return img

    if type == "randcover":
        cv2.circle(img, (256, 256), 63, (0, 0, 255), -1)
        font = cv2.FONT_HERSHEY_SIMPLEX
        cv2.putText(img, 'Just DO it ', (10, 500), font, 4, (255, 255, 0), 2)
        return img

    if type == "brighter10":
        w, h = img.shape[:2]
        for xi in range(0, w):
            for xj in range(0, h):
                img[xi, xj, 0] = int(img[xi, xj, 0] * 1.1)
                img[xi, xj, 1] = int(img[xi, xj, 1] * 1.1)
                img[xi, xj, 2] = int(img[xi, xj, 2] * 1.1)
        return img

    if type == "darker10":
        w, h = img.shape[:2]
        for xi in range(0, w):
            for xj in range(0, h):
                img[xi, xj, 0] = int(img[xi, xj, 0] * 0.9)
                img[xi, xj, 1] = int(img[xi, xj, 1] * 0.9)
                img[xi, xj, 2] = int(img[xi, xj, 2] * 0.9)
        return img

    #return img

a = attack(img_wm, type)

#plt.imshow(a)
#plt.show()

cv2.imwrite("data/wm" + type + ".png", a)

#b = psnr(img_ori,img_wm)
#print(b)

#wm = extraction.extract(img_ori, img_wm)

#print(psnr(img_ori,a))
#print(nc(img_ori,a))
