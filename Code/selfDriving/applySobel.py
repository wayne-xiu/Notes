import numpy as np
import cv2
import matplotlib.pyplot as plt
import matplotlib.image as mpimg
import pickle

image_name = "signs_vehicles_xygrad.png"
image = mpimg.imread("../../Media/" + image_name)

def abs_sobel_thresh(img, orient = 'x', thresh_min = 0, thresh_max = 255):
    # apply the following steps to img
    # 1) convert to grayscale
    # 2) take the derivative in the "orient" direction
    # 3) take the absolute value of the derivative or gradient
    # 4) scale to 8-bit (0-255) then convert to type = np.uint8
    # 5) create a mask of 1's where the scaled gradient mangnitue is between thresh_min and thresh_max
    # 6) return this mask as your binary_output image
    gray = cv2.cvtColor(img, cv2.COLOR_RGB2GRAY)
    if orient == 'x':
        sobeldire = cv2.Sobel(gray, cv2.CV_64F, 1, 0)
    if orient == 'y':
        sobeldire = cv2.Sobel(gray, cv2.CV_64F, 0, 1)
    abs_sobeldire = np.absolute(sobeldire)
    scaled_sobel = np.uint8(255*abs_sobeldire/np.max(abs_sobeldire))

    binary_output = np.zeros_like(scaled_sobel)
    binary_output[(scaled_sobel >= thresh_min) & (scaled_sobel <= thresh_max)] = 1
    return binary_output

grad_binary = abs_sobel_thresh(image, orient='y', thresh_min=20, thresh_max=100)

f, (ax1, ax2) = plt.subplots(1, 2, figsize=(24, 9))
f.tight_layout()
ax1.imshow(image)
ax1.set_title("original image", fontsize = 50)
ax2.imshow(grad_binary, cmap = 'gray')
ax2.set_title("threshold gradient", fontsize = 50)
plt.subplots_adjust(left=0., right=1, top=0.9, bottom=0.)
plt.show()