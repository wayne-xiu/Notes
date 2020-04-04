import pickle
import numpy as np
import cv2
import matplotlib.pyplot as plt
import matplotlib.image as mpimg

# read in the saved camera matrix and distortion coefficients
dist_pickle = pickle.load(open("..\..\Media\calibration_wide\wide_dist_pickle.p", "rb"))
mtx = dist_pickle["mtx"]
dis = dist_pickle["dist"]

# read in the image
img = cv2.imread("..\..\Media\calibration_wide\test_image2.png")
nx = 8
ny = 6

def corners_unwarp(img, nx, ny, mtx, dist):
    # 1) undistort using mtx, dist
    # 2) convert to grayscale
    # 3) find the chessboard corners
    # 4) If corners found:
        # a) draw corners
        # b) define 4 source points src = np.float32([[,], [,], [,], [,]])
        #   we could pick any four of the detected corners; recommend using
        #   the automatic detection
        # c) define 4 destination points dst = np.float32([[,], [,], [,], [,]])
        # d) use cv2.getPerspectiveTransform() to get M, the transform matrix
        # e) use cv2.warpPerspective() to warp the image to a top-down view
    M = None
    warped = np.copy(img)
    return warped, M

top_down, perspective_M = corners_unwarp(img, nx, ny, mtx, dis)
f, (ax1, ax2) = plt.subplot(1, 2, figsize = (24, 9))
f.tight_layout()
ax1.imshow(img)
ax1.set_title("Original image", fontsize = 50)
ax2.imshow(top_down)
ax2.set_title("Undistorted and Warped image", fontsize = 50)
plt.subplots_adjust(left = 0., right = 1, top = 0.9, bottomo = 0.)
plt.show()