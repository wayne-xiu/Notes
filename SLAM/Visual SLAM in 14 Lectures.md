# Visual SLAM in 14 Lectures

[toc]

## 1. 预备知识

SLAM:搭载特定传感器的主体，在没有环境先验信息的情况下，于运动过程中建立环境的模型，同时估计自己的运动。If the sensor is camera, then it's called Visual SLAM.

1. Real-time
2. No prior knowledge

背景知识：射影几何，计算机视觉，状态估计理论，李群李代数

Related books

- Probabilistic robotics
- Multi View Geometry in Computer Vision
- State Estiamtion for Robotics: A Matrix-Lie-Group Approach

SLAM系统模块

- 视觉里程计 - Visual Odometry
- 后端优化
- 建图
- 回环检测

有用的库

- Eigen
- OpenCV
- PCL
- g2o
- Ceres

## 2. 初识SLAM

传感器

- 携带于机器人本体：轮式编码器（轮子转动角度），相机，激光雷达，GPS，IMU（角速度和加速度）
- 安装于环境中：导轨，二维码

SLAM中非常强调未知环境

相机

- 单目（Monocular)
- 双目（stereo)
- 深度（RGB-D）

照片本质上是拍照时的场景（Scene）在相机的成像平面上留下的投影。它*以二维的形式反映了三维的世界*。这个过程中丢掉了*深度*。

在Monocular SLAM，移动相机来估计它的运动（motion)，同时估计场景中物体的远近和大小（structure)。相机移动时，物体在图像上的运动形成了*视差*。

Monocular SLAM估计的轨迹和地图与真实的轨迹和地图相差一个因子，即所谓的尺度(Scale)。由于单目SLAM无法仅凭图像确定真实的尺度，又成为*尺度不确定性*。平移计算深度，无法确定尺度。

双目与深度相机测量深度的原理不同：

- 双目相机由两个单目组成，两个相机之间的距离-基线（Baseline）是已知的。通过这个基线来估计每个像素的空间位置，与人眼相似。多目相机与双目没有本质区别。双目需要大量计算才能（不太可靠的）估计每个像素点的深度。双目相机测量的深度与基线相关。基线距离越大，能够测量的范围越远，所以无人车上的双目都很大。

  双目（多目）的主要缺点是配置与标定复杂，深度量程和精度受双目的基线与分辨率所限。

  计算非常消耗资源。*计算量是双目的主要问题之一*

- RGB-D通过红外结构光或Time-of-Flight(ToF)原理，像激光传感器那样，通过主动向物体发射光并接收返回的光，测出物体与相机之间的距离。

  Kinect/Kinect v2, Xtion Pro Live, RealSense

  缺点：测量范围窄、噪声大、视野小、易受日光干扰、无法测量透射材质

### Visual SLAM Framework

![vSLAMWorkflow](../Media/vSLAMWorkflow.png)

1. 传感器信息读取 - 相机图像读取和预处理；码盘，IMU
2. 视觉里程计（Visual Odometry）- 估算相邻图像间相机的运动，以及局部地图的样子 Front End
3. 后端优化（Optimization）- 后端接受不同时刻视觉里程计测量的相机位姿，以及回环检测的信息，对它们进行优化，得到全局一致的轨迹和地图 Back End
4. 回环检测 （Loop Closing）- 判断机器人是否到达过先前的位置。如果检测到回环，它会把信息提供给后端处理
5. 建图（Mapping）- 根据估计的轨迹，建立与任务要求对应的地图

经典算法已定型，并且在许多视觉程序库和机器人程序库中提供。**如果把工作环境限定在静态、刚体，光照变化不明显、没有人为干扰的场景**，SLAM已经相当成熟。

图像在计算机里只是一个数值矩阵（理解图像-Machine Learning）。为了定量的估计相机运动，必须先了解相机与空间点的几何关系。

Visual Odometry是SLAM的关键。然而，通过VO来估计轨迹，将不可避免的出现**累计漂移（Accumulating Drift）**

后端优化主要处理SLAM过程中的**噪声**问题。如何从有噪声的数据中估计整个系统的状态，以及这个状态估计的不确定性有多大 - 最大后概率估计（Maximum-a-Posteriori， MAP）

在视觉SLAM中，前端和计算机视觉更相关，比如图像的特征提取与匹配，后端则主要是滤波和非线性优化算法。

后端SLAM是个状态估计问题 - **对运动主体自身和周围环境空间不确定性的估计**



## 3. 三维空间刚体运动

## 4. 李群与李代数

## 5. 相机与图像

## 6. 非线性优化

## 7. 视觉里程计1

## 8. 视觉里程计2

## 9. 实践：设计前端

## 10. 后端1

## 11. 后段2

## 12. 回环检测

## 13. 建图

## 14. SLAM：现在与未来

## 附录