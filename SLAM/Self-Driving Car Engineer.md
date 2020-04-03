# Self-Driving Car Engineer

[toc]

Projects

- Finding Lane Lines
- Advanced Lane Finding
- Traffic Sign Classifier
- Behavioral Cloning
- Extended KF
- Kidnapped Vehicle
- Highway Driving
- PID Controller
- LinkedIn, Github profiles
- Programming a Real Self-Driving Car

## Part 1. Computer Vision, Deep Learning, and Sensor Fusion

### Introduction; 

Robotics approach and Deep learning approach

Stanley; DARPA Grand Challenge

### Computer Vision Fundamentals

vision perception; 

The following features could be useful in the identification of lane lines on the road

- Color
- Shape
- Orientation
- Position in the image

RGB channels; 0: darkest, 255: white; pure while: [255, 255, 255]



#### Color Selection

modify the threshold values until retain as much of the lane lines as possible while dropping everything else

```python
import matplotlib.pyplot as plt
import matplotlib.image as mpimg
import numpy as np

# read image
image = mpimg.imread("../../Media/laneOrg.jpg")
print("this iamge is: ", type(image),
      "with dimension: ", image.shape)  # (nrow, ncol, 3)

xsize = image.shape[1]
ysize = image.shape[0]

color_select = np.copy(image)

red_threshold = 200
green_threshold = 200
blue_threshold = 200
rgb_threshold = [red_threshold, green_threshold, blue_threshold]
# print(type(rgb_threshold))

# pixels below the threshold
thresholds = (image[:, :, 0] < rgb_threshold[0]) \
    | (image[:, :, 1] < rgb_threshold[1]) \
    | (image[:, :, 2] < rgb_threshold[2])
print(type(thresholds), '\t', thresholds.shape)
color_select[thresholds] = [0, 0, 0]

plt.imshow(color_select)
plt.show()

# mpimg.imsave("../../Media/laneSelected.jpg", color_select)
```

![laneOrg](../Media/laneOrg.jpg)

![laneSelected](../Media/laneSelected.jpg)



#### Region Masking

Assuming that the front facing camera that took the image is mounted in a fixed position on the car, such that the lane lines will always appear in the same general region of the image. We are using a triangular mask for illustration, in principle, we could use any polygon.

```python
import matplotlib.pyplot as plt
import matplotlib.image as mpimg
import numpy as np

# read image
image = mpimg.imread("../../Media/laneOrg.jpg")
print("this iamge is: ", type(image),
      "with dimension: ", image.shape)  # (nrow, ncol, 3)

xsize = image.shape[1]
ysize = image.shape[0]

color_select = np.copy(image)
line_image = np.copy(image)

red_threshold = 200
green_threshold = 200
blue_threshold = 200
rgb_threshold = [red_threshold, green_threshold, blue_threshold]
# print(type(rgb_threshold))

# Define a triangle region of interest
left_bottom = [120, 540]
right_bottom = [820, 540]
apex = [475, 310]

fit_left = np.polyfit((left_bottom[0], apex[0]), (left_bottom[1], apex[1]), 1)
fit_right = np.polyfit(
    (right_bottom[0], apex[0]), (right_bottom[1], apex[1]), 1)
fit_bottom = np.polyfit(
    (left_bottom[0], right_bottom[0]), (left_bottom[1], right_bottom[1]), 1)

# mask pixels below the threshold
color_thresholds = (image[:, :, 0] < rgb_threshold[0]) \
    | (image[:, :, 1] < rgb_threshold[1]) \
    | (image[:, :, 2] < rgb_threshold[2])

# find the region inside the triangle
XX, YY = np.meshgrid(np.arange(0, xsize), np.arange(0, ysize))
region_thresholds = (YY > (XX*fit_left[0] + fit_left[1])) & \
    (YY > (XX*fit_right[0] + fit_right[1])) & \
    (YY < (XX*fit_bottom[0] + fit_bottom[1]))

color_select[color_thresholds] = [0, 0, 0]
# find where the image is both colored right and in the region
line_image[~color_thresholds & region_thresholds] = [255, 0, 0]

plt.imshow(image)
x = [left_bottom[0], right_bottom[0], apex[0], left_bottom[0]]
y = [left_bottom[1], right_bottom[1], apex[1], left_bottom[1]]
plt.plot(x, y, 'b--', lw=4)
plt.imshow(color_select)
plt.imshow(line_image)
plt.show()
```

![laneColorRegion](../Media/laneColorRegion.png)

Lane lines are not always the same color, and even lines of the same color under different lighting conditions may fail to be detected by the simple color selection

#### Canny Edge Detection

OpenCV in Python

John F. Canny; Convert to grayscale, calculate gradient; Canny algorithm will thin the edges

```
edges = cv2.Canny(gray, low_threshold, high_threshold)
```

The Canny algorithm will first detect strong edge (strong gradient) pixels above the high_threshold, and reject pixels below the low_threshold. Pixels with values between the low_threshold and high_threshold will be included as long as they are connected to strong edges. The derivatives (i.e. the value differences from pixel to pixel), will be on the scale of tens or hundreds. John Canny recommended a low_threshold to high_threshold ratio of 1:2 or 1:3.

Applying Gaussian smoothing before running Canny will suppress noise and spurious gradients by averaging. cv2.Canny function actually applies Gaussian smoothing internally, but further smoothing will yield different results. 

```python
import matplotlib.pyplot as plt
import matplotlib.image as mpimg
import numpy as np
import cv2

image = mpimg.imread("../../Media/exitRamp.jpg")
plt.imshow(image)

gray = cv2.cvtColor(image, cv2.COLOR_RGB2GRAY)
plt.imshow(gray, cmap="gray")

kernel_size = 3  # odd number implying the averaging window
blur_gray = cv2.GaussianBlur(gray, (kernel_size, kernel_size), 0)

low_threshold = 50
high_threshold = 150
edges = cv2.Canny(blur_gray, low_threshold, high_threshold)

plt.imshow(edges, cmap = "Greys_r")
plt.show()
```

![exitRamp](../Media/exitRamp.jpg)

![exitRampEdgeDetected](../Media/exitRampEdgeDetected.jpeg)



#### Hough Transform

using the Hough transform to find Lines from Canny edges.

In 2D image space, a line can be described as $y = m*x+b$, which could also be represented in the parameter space. The Hough transform is jus the conversion from image space to Hough space. *The characterization of a line in image space is a single point at the position (m, b) in Hough space*.

Inversely, a line $y = m*x+b$ can be written as $b = -x*m+y$, so a point in image space is a line in Hough space with slope of $-x$. The intersection point of the two lines in the Hough space correspond to [A line in image space that passes through both $(x1, y1)$ and $(x2, y2)$], since we have the same $(m, b)$ at the intersection point.

So our strategy in finding straight lines in image space is to look for intersecting lines in the Hough space. One problem is that vertical lines in image space has infinite slope; the polar coordinate is the strategy.

 ![lineInPolarHough](../Media/lineInPolarHough.png)

```python
lines = cv2.HoughLinesP(masked_edges, rho, theta, threshold,
                       np.array([]), min_line_length, max_line_gap)
```

- masked_edges: input image
- lines: array containing endpoints $(x1, y1, x2, y2)$ of all line segments detected by the transform operation
- rho, theta: distance and angular resolution of our grid in Hough space; rho unit pixel, theta unit radian; rho with a minimum value of 1, a reasonable starting for theta is 1 degree (pi/180 radians)
- threshold: the minimum number of votes (intersections in a given grid cell) a candidate line needs to have to make it into the output
- np.array([]): empty placeholder
- min_line_length: minimum length of a line (in pixels) that will be accepted in the output
- max_line_gap: maximum distance (in pixels) between segments that will be allowed to be connected into a single line

```python
import matplotlib.pyplot as plt
import matplotlib.image as mpimg
import numpy as np
import cv2

image = mpimg.imread("../../Media/exitRamp.jpg")
plt.imshow(image)

# grayscale image
gray = cv2.cvtColor(image, cv2.COLOR_RGB2GRAY)
plt.imshow(gray, cmap="gray")

# define a kernel size and apply Gaussian smoothing
kernel_size = 5  # odd number implying the averaging window
blur_gray = cv2.GaussianBlur(gray, (kernel_size, kernel_size), 0)

# Canny edge detection
low_threshold = 50
high_threshold = 150
masked_edges = cv2.Canny(blur_gray, low_threshold, high_threshold)

# create a masked edges image using cv2.fillPoly
mask = np.zeros_like(masked_edges)
ignore_mask_color = 255
# define a four sided polygon to mask
imshape = image.shape
vertices = np.array([[(0,imshape[0]),(450, 300), (500, 300), (imshape[1],imshape[0])]], dtype=np.int32)
cv2.fillPoly(mask, vertices, ignore_mask_color)
masked_edges = cv2.bitwise_and(masked_edges, mask)

# Define the Hough transform parameters
rho = 1
theta = 0.5*np.pi/180.0
threshold = 5
min_line_length = 10
max_line_gap = 1
line_image = np.copy(image)*0  # creating a blank to draw lines on

lines = cv2.HoughLinesP(masked_edges, rho, theta, threshold,
np.array([]), min_line_length, max_line_gap)

for line in lines:
    for x1, y1, x2, y2 in line:
        cv2.line(line_image, (x1, y1), (x2, y2), (255, 0, 0), 10)

# Create a "color" binary imag eto combine with line image
color_edges = np.dstack((masked_edges, masked_edges, masked_edges))

# Draw the lines on the edge image
combo = cv2.addWeighted(color_edges, 0.8, line_image, 1, 0)

plt.imshow(combo)
plt.show()
```

![exitRampEdgeDetectedHough](../Media/exitRampEdgeDetectedHough.png)

#### Parameter Tuning

Parameter tuning is one of the biggest challenges in CV - what works well for one image may not work at all with different lighting and/or backgrounds.

develop a parameter tuning tool overtime

### Project: Finding Lane Lines

finding lane lines in a video stream; video is a just a series of images

The README.md file for each repository (in GitHub) can include the following information:

- a list of files contained in the repository with a brief description of each file
- any instructions someone might need for running the code
- an overview of the project

The tools we have:

- color selection
- region of interest selection
- grayscaling
- Gaussian smoothing
- Canny Edge Detection
- Hough Transform line detection

Some OpenCV functions that might be useful for this project are:

- cv2.inRange() for color selection
- cv2.fillPoly() for regions selection
- cv2.line() to draw lines on an image given endpoints
- cv2.addWeighted() to add/overlay two images
- cv2.cvtColor() to grayscale or change color
- cv2.imwrite() to output images to file
- cv2.bitwise_and() to apply a mask to an image



### Camera Calibration

80% of challenge is from *perception*

![sensorComp](../Media/sensorComp.png)

#### Distortion Correction

Image distortion occurs when a camera looks at 3D objects in the real world and transforms them into a 2D image.

#### Pinhole Camera model

Real cameras use curved lenses (not pinhole) to form an image, and light rays often bend a little too much or too little at the edges of these lenses. This creates an effect that distorts the edges of images.

- **radial distortion**: lines or objects at the edges of the images appear more or less curved than actual
- **tangential distortion**: when a camera's lens is not aligned perfected parallel to the imaging plane (seems farther away or closer than actual)

distortion values: $[k1, k2, p1, p2, k3]$, for lens with not much distortion, $k3$ sometime can be negligible (close to zero), that why it appears at the end of the array in OpenCV

![cameraDistortion](../Media/cameraDistortion.png)

The distortion correction formulas

Radial

$\begin{align} x_{distorted} = x_{ideal}(1+k_1r^2+k_2r^4+k_3r^6) \\ y_{distorted} = y_{ideal}(1+k_1r^2+k_2r^4+k_3r^6)\end{align}$

Tangential

$\begin{align} x_{corrected} = x+[2p_1xy + p_2(r^2+2x^2)] \\ y_{corrected} = y + [p_1(r^2+2y^2)+2p_2xy]\end{align}$

#### Camera Calibration

we can use a chess board for the calibration;

"corners" are points where two black and two white squares intersect,, thus inside only not outside corners

```python
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
```



![calibrationTestDistorted_corners](../Media/calibrationTestDistorted_corners.png)

color images have 3 dimensions (height, width, and depth/channels) while grayscale image only has 2 dimensions.



Camera calibration and correction for distortion with saved image points and object points extracted

```python
import pickle
import cv2
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.image as mpimg

# Read in the saved objpoints and imgpoints
dist_pickle = pickle.load(open("wide_dist_pickle.p", "rb"))
objpoints = dist_pickle["objpoints"]
imgpoints = dist_pickle["imgpoints"]

# Read in an image
img = cv2.imread("test_image.png")

# takes an image, object points, and image points
# performs the camera calibration, image distortion correction and 
# returns the undistorted image
def cal_undistort(img, objpoints, imgpoints):
    # print(img.shape)
    img_size = img.shape[1:]
    # img_size = (img.shape[1], img.shape[0])
    # img_size = img.shape[::-1]
    ret, mtx, dist, rvecs, tvecs = cv2.calibrateCamera(objpoints, imgpoints, img_size, None, None)
    undist = cv2.undistort(img, mtx, dist, None, mtx)
    # cv2.imwrite("test_image_undist.png", undist)
    return undist

undistorted = cal_undistort(img, objpoints, imgpoints)

f, (ax1, ax2) = plt.subplots(1, 2, figsize=(24, 9))
f.tight_layout()
ax1.imshow(img)
ax1.set_title('Original Image', fontsize=50)
ax2.imshow(undistorted)
ax2.set_title('Undistorted Image', fontsize=50)
plt.subplots_adjust(left=0., right=1, top=0.9, bottom=0.)
plt.show()
```

![calibrationTestCalibrated](../Media/calibrationTestCalibrated.png)

Camera calibration and correction for distortion by extracting object points and image points from raw images

```python
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
```

![calibrationTestCalibrated_2](../Media/calibrationTestCalibrated_2.png)

#### Lane Curvature

To obtain the correct steering angle to turn, we need calculate the curvature of the lane line. One way is to fit a 2nd degree polynomial to that line $f(y) = Ay^2 + By + C$. Performing a perspective transform to get a bird's-eye view of the lane.

#### Perspective Transform

A perspective transform maps the points in a given image to different, desired image points with a new perspective. A bird's eye view transform allows us view a lane from above. It will also allow us to match the car's location directly with a map since map displays roads from a top-down view.

![perspectiveTransformExample](../Media/perspectiveTransformExample.png)

#### Transform a Stop Sign

Use 4 points to define a linear transformation between the original image and the warped iamge

We can choose four source points manually when applying a perspective transform manually. However, it is often not the best option. Many perspective transform algorithms will programmatically detect four source points in an image based on edge or corner detection and analyzing attributes like color and surrounding pixels.

![calibrationDistortionCorrectPerspectTransform](../Media/calibrationDistortionCorrectPerspectTransform.png)



### Gradients and Color Spaces

### Advanced Computer Vision

### Project: Advanced Lane Finding

### Neural Networks

### TensorFlow

### Deep Neural Networks

### LeNet Traffic Signs

### Project: Traffic Sign Classifier

### Keras

### Transfer Learning

### Project: Behavioral Cloning

### Github

### Sensors

### Kalman Filters

### C++ Checkpoint

### Geometry and Trigonometry Refresher

### Extended Kalman Filters

### Project: Extended Kalman Filters

## Part 2. Localization, Path Planning, Control and System Integration

### Introduction to Localization

### Markov Localization

### Motion Models

### Particle Filters

### Implementation of a Particle Filter

### Project: Kidnapped Vehicle Project

### Search

### Prediction

### Behavior Planning

### Trajectory Generation

### Project: Highway Driving

### PID Control

### Project: PID Controller

### Project: Improve LinkedIn/GitHub

### Autonomous Vehicle Architecture

### Introduction to ROS

### Packages and Catkin Workspaces

### Writing ROS Nodes

### Project: Program an Autonomous Vehicle

### 

## Additional Content

### Object Detection

### Unscented KF

### Vehicle Models

### Model Predictive Control

### Fully Convolutional Networks

### Scene Understanding

### Inference Performance

### Introduction to Functional Safety

### Functional Safety: Safety Plan

### Functional Safety: Hazard Analysis and Risk Assessment

### Functional Safety: Functional Safety Concept

### Functional Safety: Technical Safety Concept

### Functional Safety at Software and Hardware Levels
