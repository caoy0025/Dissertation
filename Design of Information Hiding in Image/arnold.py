from PIL import Image
import matplotlib.pyplot as plt
import matplotlib.image as mpimg
import numpy as np
import cv2
import pywt
import math

k1 = 0
k2 = 8

img_path = './data/CoverImg.png'
img = cv2.imread(img_path)
img = np.array(img)
[M, N, H] = img.shape
BN = np.zeros((M,N))
AN = np.zeros((M,N))
AN = np.reshape(img[:,:,0],(M,N))
UN = np.reshape(img[:,:,1],(M,N))
VN = np.reshape(img[:,:,2],(M,N))

for num in range(0,k1):
    BN = np.zeros((M, N))
    for i in range(0,M):
        for j in range(0, N):
            ik = i + j
            ik = ik % M
            jk = i + 2 * j
            jk = jk % N
            BN[ik, jk] = AN[i,j]
    AN = BN

for num in range(0,k2):
    for i in range(0, M):
        for j in range(0, N):
            y = AN[i,j] % 16
            x = (AN[i,j] - y) / 16
            xk = x + y
            yk = x + 2 * y
            xk = xk % 16
            yk = yk % 16
            AN[i,j] = xk * 16 + yk

for num in range(0,k1):
    BN = np.zeros((M, N))
    for i in range(0,M):
        for j in range(0, N):
            ik = i + j
            ik = ik % M
            jk = i + 2 * j
            jk = jk % N
            BN[ik, jk] = UN[i,j]
    UN = BN

for num in range(0,k2):
    for i in range(0, M):
        for j in range(0, N):
            y = UN[i,j] % 16
            x = (UN[i,j] - y) / 16
            xk = x + y
            yk = x + 2 * y
            xk = xk % 16
            yk = yk % 16
            UN[i,j] = xk * 16 + yk

for num in range(0,k1):
    BN = np.zeros((M, N))
    for i in range(0,M):
        for j in range(0, N):
            ik = i + j
            ik = ik % M
            jk = i + 2 * j
            jk = jk % N
            BN[ik, jk] = VN[i,j]
    VN = BN

for num in range(0,k2):
    for i in range(0, M):
        for j in range(0, N):
            y = VN[i,j] % 16
            x = (VN[i,j] - y) / 16
            xk = x + y
            yk = x + 2 * y
            xk = xk % 16
            yk = yk % 16
            VN[i,j] = xk * 16 + yk

CN = np.random.randint(0,1,size=(M,N,3))*0
CN[:,:,:1] = np.reshape(AN,(M,N,1))
CN[:,:,1:2] = np.reshape(UN,(M,N,1))
CN[:,:,2:3] = np.reshape(VN,(M,N,1))
one = AN.reshape(M*N)
img_hist, bins, patches = plt.hist(one, bins=256, normed=1, alpha=0.75)
plt.imshow(CN)
plt.show()

cv2.imwrite("./data/wrongk2.png", CN)
