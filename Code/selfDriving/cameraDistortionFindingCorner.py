import numpy as np
import cv2
import matplotlib.pyplot as plt
import matplotlib.image as mpimg

# prepare object points
nx = 8
ny = 6

# make a list of calibration images
fname = "calibrationTestDistorted.png"
img = cv2.imread("../../Media/" + fname)

# convert to grayscale
# note: if read with cv2.imread, use cv2.COLOR_BGR2GRAY
#       if read with mpimg.imread, use cv2.COLOR_RGB2GRAY
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

# find the chessboard corners
ret, corners = cv2.findChessboardCorners(gray, (nx, ny), None)
print(type(corners), corners.shape)

# if found, draw corners
if ret == True:
    cv2.drawChessboardCorners(img, (nx, ny), corners, ret)
    plt.imshow(img)
    plt.show()