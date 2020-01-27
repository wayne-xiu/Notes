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

SLAM方案：通过状态估计理论吧定位和建图的不确定性表达出来，然后采用滤波器或非线性优化，估计状态的均值和不确定性（方差）

回环检测（Loop Closure Detection）- 主要解决位置估计随时间漂移的问题。让机器人具有**识别到过的场景**的能力。可以判断图像的相似性来完成回环检测。视觉回环检测实际上是一种计算图像数据相似性的算法。

建图- 地图的构建没有固定的形式和算法，例如2D栅格，2D拓扑，3D点云，3D网格。可以分为度量地图（Metric）和拓扑地图（Topological）两种：

- 度量地图强调精确地表示地图中物体的位置关系。对于定位，稀疏路标地图足够了，导航则需要稠密的地图。2D小格子Grid，3D小方块Voxel。一个小块有占据、空闲、未知三种状态。A\*, D\* algorithm
- 拓扑地图更强调地图元素之间的关系。Graph，只考虑节点间的连通性

### SLAM问题的数学表述 

运动与观测

​														$$x_k=f(x_{k-1},u_k,\omega_K)$$

​														$$z_{k, j}=h(y_j, x_k, v_{k,j})$$

$u_k$ - 运动传感器输入，$\omega_k$ - 运动噪声， $y_j$ - 路标， $v_{k,j}$ - 观测噪声。描述：当知道运动测试的读数$u$以及传感器的读数$z$时，如何求解定位问题（估计$x$)和建图问题（估计$y$）？SLAM问题建模为一个状态估计问题：如何通过带有噪声的测量数据，估计内部的、隐藏的状态变量。

视觉SLAM，传感器是相机，那么观测方程就是“对路标点拍摄后，得到图像中的元素”的过程。这个过程牵涉到相机模型的描述。

> 按照运动和观测方程是否为线性、噪声是否服从高斯分布来分类，分为线性/非线性和高斯/非高斯系统。
>
> - 线性高斯LG的无偏的最优估计由Kalman Filter给出
> - 非线性非高斯系统，以EKF和非线性优化两大类方法求解。EKF把系统线性化，以预测-更新两大步骤求解。为了克服EKF的缺点（线性化误差和噪声高斯分布的假设），使用粒子滤波Particle Filter
> - 现在，视觉SLAM以图优化Graph Optimization为主流技术进行状态估计。**优化技术已经明显优于滤波器技术**，只要计算资源允许，应该使用优化方法

### 实践：编程基础

Linux中，程序是具有执行权限的文件，可以是脚本或者二进制文件，不过不像windows那样限制后缀名。

CMake: 后面的大多数库都使用cmake来管理源代码。In a cmake project

- 用cmake命令生成一个makefile文件
- 用make命令根据这个makefile文件的内容编译整个工程

步骤

1. 遵循cmake语法，建立CMakeLists.txt文件（我用Sublime CMake package）

```c++
# declare minimum cmake version reqirement
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

# declare a cmake project
#project(helloSLAM VERSION 0.1.0 LANGUAGES CXX and/or C)
project(helloSLAM)

# add executable
add_executable(helloSLAM helloSLAM.cpp)
```

2. 工程分析
   1. 自动生成以下中间文件- CMakeFiles folder, Makefile, cmake_install.cmake, CMakeCache.txt
   2. MakeFile是最重要的-一个自动化编译的脚本，可以理解成系统自动生成的编译指令

```C++
camke .
```

3. 编译 - 生成可执行文件

```c++
make
```

4. 测试执行

```c++
./helloSLAM
```

cmake-make

- cmake过程中处理了工程文件之间的关系
- make过程实际调用了个g++来编译程序

处理中间文件

```c++
mkdir build  // 中间文件都存在build文件夹中，build后可直接删除
cd build
cmake ..
make
```



使用库：只有带有main函数的文件才会生成可执行程序。其他算法、程序的集合代码，往往打包成一个库。

```c++
add_library(hello libHelloSLAM.cpp)  // 静态库
add_library(hello_shared SHARED libHelloSLAM.cpp)  // 共享库（.so in Linux, .dll in Windows)
```

Linux中，库文件分为静态库和共享库（.a, .so; windows中，.lib, .dll)。所有库都是一些函数打包后的集合，差别在于静态库每次被调用都会生成一个副本，而共享库则只有一个副本。

库文件就是一个压缩包，里面有编译好的二进制函数。对于使用者，只要拿到头文件和库文件，就可以调用这个库了。

```c++
add_executable(useHello useHello.cpp)
target_link_libraries(useHello hello_shared)  // 链接可执行到库文件
```

在IDE中调试

```c++
set(CMAKE_BUILD_TYPE "Debug")
```



## 3. 三维空间刚体运动

Eigen提供了C++中矩阵计算，它的Geometry模块提供了四元数等刚体运动的描述。

$$a \times b = a^\wedge b$$

相机运动是一个刚体运动，它保证了同一个向量在各个坐标系内的长度和夹角都不会发生变化。具状矩阵$R$ 由两组基之间的内积组成，刻画了旋转前后同一个向量的坐标变换关系。

$SO(n)$是特殊正交群（Special Orthogonal Group）。$SO(3)$就是三维空间的旋转。

齐次坐标的变换矩阵$T$, 引入了一个额外的自由度，但是把非线性变换变成了线性计算。特殊欧式群（Speical Euclidean Group）

**Eigen**

Eigen是一个纯头文件搭建起来的库，不需要链接库文件（没有）



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