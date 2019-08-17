
from PIL import Image
import numpy as np
import matplotlib


image_file = Image.open('./data/wm_ori.png') # open colour image
image_file = image_file.convert('1') # convert image to black and white
img = np.array(image_file)
img = img[:512,:512]
im = Image.fromarray(img)


matplotlib.image.imsave('./data/wm.png', im)