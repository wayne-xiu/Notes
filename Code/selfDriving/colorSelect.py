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
