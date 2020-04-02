import numpy as np
import cv2
import glob
import matplotlib.pyplot as plt

# prepare object points, like (0, 0, 0), (1, 0, 0,), (2, 0, 0) ..., (5, 7, 0)
objp = np.zeros((6*8, 3), np.float32)
objp[:, :2] = np.mgrid[0:8, 0:6].T.reshape(-1, 2)

# arrays to store object points and image points form all the images
objpoints = []  # 3d points in real world space
imgpoints = []  # 2d points in image plane

# make a list of calirbation images
images = glob.glob("../../Media/calibration_wide/GO*.jpg")

# step through the list and search for chessbaord corners
for idx, fname in enumerate(images):
    img = cv2.imread(fname)
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    # find the chessboard corners
    ret, corners = cv2.findChessboardCorners(gray, (8, 6), None)

    # if found, add object points, image points
    if ret == True:
        objpoints.append(objp)
        imgpoints.append(corners)

        # draw and display the corners
        cv2.drawChessboardCorners(img, (8, 6), corners, ret)
        # write_name = "corners_found" + str(idx) +".jpg"
        # cv2.imwrite(write_name, img)
        cv2.imshow("img", img)
        cv2.waitKey(200)

cv2.destroyAllWindows()

# use the above objpoints and imgpoints for camera calibration and verify
# with one image distortion
import pickle

img = cv2.imread("../../Media/calibration_wide/test_image.jpg")
img_size = (img.shape[1], img.shape[0])

ret, mtx, dist, rvecs, tvecs = cv2.calibrateCamera(objpoints, imgpoints, img_size, None, None)

dst = cv2.undistort(img, mtx, dist, None, mtx)
cv2.imwrite("../../Media/calibration_wide/test_image_undist.jpg", dst)

# save the camera calibration result for later use (no need to worry about rvecs, tvecs)
dist_pickle = {}
dist_pickle["mtx"] = mtx
dist_pickle["dist"] = dist
pickle.dump(dist_pickle, open("../../Media/calibration_wide/wide_dist_pickle.p","wb"))
# Visualize undistortion
f, (ax1, ax2) = plt.subplots(1, 2, figsize=(24, 9))
ax1.imshow(img)
ax1.set_title('Original Image', fontsize=30)
ax2.imshow(dst)
ax2.set_title('Undistorted Image', fontsize=30)
plt.subplots_adjust(left=0., right=1, top=0.9, bottom=0.)
plt.show()