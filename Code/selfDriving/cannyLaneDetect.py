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