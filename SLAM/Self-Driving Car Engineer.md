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



### Project: Finding Lane Lines

### Camera Calibration

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

