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

Eigen与MATLAB类似，几乎所有的数据都当作矩阵来处理。Eigen不支持自动类型转化（implicit cast），比如float，double相加会报错。

### 旋转向量与欧拉角（Rotation vector & Euler angles）

Rotationi matrix and Transformation matrix are redundance, it's hard to estimate or optimize.

旋转向量（Axis-Angle）实际上就是李代数。从旋转向量到旋转矩阵的转换，通过罗德里格斯公式（Rodrigues's Formula）

​											          $$\boldsymbol{R} = cos(\theta)\boldsymbol{I}+(1-cos(\theta))\boldsymbol{nn}^T+sin\theta \boldsymbol{n}^\wedge$$

​																	$$\theta = arccos(\frac{tr(\boldsymbol{R})-1}{2})$$

​																		$$\boldsymbol{Rn}=\boldsymbol{n}$$

由于转轴上的向量在旋转后不发生改变，转轴$n$是矩阵$R$特征值为1对应的特征向量（求解后再归一化）。

欧拉rpy的旋转顺序是zyx（临时轴）。Euler Angles的一个缺点是万向锁问题Gimbal Lock:在俯仰角为$\pm90^\circ$时，第三次旋转与第一次旋转将使用同一个轴，使得系统丢失了一个自由度-奇异性问题（singularity）。**由于这个原理，欧拉角不适于插值和迭代，也不会在滤波和优化中使用**。

### 四元数

可以证明，我们找不到**不带奇异性的三维向量表示方式**。四元数（Quaternion）既是紧凑的也没有奇异性。

​													$$\boldsymbol{q}=q_0+q_1i+q_2j+q_3k=[s, \boldsymbol{v}]$$

四元数与旋转向量的关系

​													$$\boldsymbol{q}=[cos\frac{\theta}{2}, n_xsin\frac{\theta}{2}, n_ysin\frac{\theta}{2}, n_zsin\frac{\theta}{2}]^T$$

​											    	$$\begin{cases}\theta=2arccos(q_0) \\ [n_x, n_y, n_z]^T = [q_1, q_2, q_3]^T/sin\frac{\theta}{2} \end{cases}$$

”转了一半“。在四元数中，任意的旋转都可以由两个互为相反数的四元数表示。

**四元数表示旋转**

一个三维空间点可以用一个虚四元数表示 $$\boldsymbol{p}=[0, x, y, z]=[0, \boldsymbol{v}]$$, then $p'=qpq^{-1}$, where $q=[cos\frac{\theta}{2}, \boldsymbol{n}sin\frac{\theta}{2}]$

四元数到旋转矩阵

​									$$\boldsymbol{R}=\begin{bmatrix}1-2q_2^2-2q_3^2&2q_1q_2-2q_0q_3&2q_1q_3+2q_0q_2\\2q_1q_2+2q_0q_3&1-2q_1^2-2q_3^2&2q_2q_3-2q_0q_1\\2q_1q_3-2q_0q_2&2q_2q_3+2q_0q_1&1-2q_2^2-2q_2^2\end{bmatrix}$$

​								$$q_0=\frac{\sqrt{tr(\boldsymbol{R})+1}}{2}, q_1=\frac{m_{23}-m_{32}}{4q_0}, q_2=\frac{m_{31}-m_{13}}{4q_0}, q_3=\frac{m_{12}-m_{21}}{4q_0}$$

由于$\boldsymbol{q}， -\boldsymbol{q}$表示同一旋转，事实上$\boldsymbol{R}$对应的四元数并不是唯一的。编程中，当$q_0$接近0时，其余3个分量会非常大，解不稳定。

### 相似、仿射、射影变换

**相似变换**-缩放

$$\boldsymbol{T}_S = \begin{bmatrix}s\boldsymbol{R}&\boldsymbol{t}\\ \boldsymbol{0}^T&1\end{bmatrix}$$

**仿射变换**

$$\boldsymbol{T}_A = \begin{bmatrix}\boldsymbol{A}&\boldsymbol{t}\\ \boldsymbol{0}^T&1\end{bmatrix}$$

仿射变换之要求$\boldsymbol{A}$可逆，而不必正交。仿射变换也叫正交投影。经过仿射变换后，立方体不再是方的，但各个面仍然是平行四边形。

**射影变换**

$$\boldsymbol{T}_P = \begin{bmatrix}\boldsymbol{A}&\boldsymbol{t}\\ \boldsymbol{a}^T&v\end{bmatrix}$$

射影变换是最一般的变换。$\boldsymbol{A}$可逆，$\boldsymbol{t}$为平移，$\boldsymbol{a}^T$为缩放，$v=0/1$。因此，2D有8个自由度，3D有15个自由度。

从真实世界到相机照片的变换可以看成一个射影变换：方形不再是方形，甚至不是平行四边形。而是一个不规则的四边形。

| 变换           | 矩阵形式                                                     | 自由度 | 不变性质             |
| -------------- | ------------------------------------------------------------ | ------ | -------------------- |
| Euler          | $$\begin{bmatrix}\boldsymbol{R}&\boldsymbol{t}\\ \boldsymbol{0}^T&1\end{bmatrix}$$ | 6      | 长度、夹角、体积     |
| 相似           | $$\begin{bmatrix}s\boldsymbol{R}&\boldsymbol{t}\\ \boldsymbol{0}^T&1\end{bmatrix}$$ | 7      | 体积比               |
| 仿射Affine     | $$\begin{bmatrix}\boldsymbol{A}&\boldsymbol{t}\\ \boldsymbol{0}^T&1\end{bmatrix}$$ | 12     | 平行性、体积比       |
| 射影Projective | $$\begin{bmatrix}\boldsymbol{A}&\boldsymbol{t}\\ \boldsymbol{a}^T&v\end{bmatrix}$$ | 15     | 接触平面的相交和相切 |

> 如果相机的焦距为无穷远，变换为仿射变换

> Eigen's Geometry module provides two different kinds of geometric transformations:
>
> - Abstract transformations, such as rotations (angle-aixs, quaternion), translations, scalings. These transformations are not represented as matrices, but we can nevertheless mix them with matrices and vectors in expressions and conversion
> - Projective or affine transformation matrices: Transform class. These are really matrices

https://eigen.tuxfamily.org/dox/group__TutorialGeometry.html#TutorialGeoTransform

实际中，我们至少定义两个坐标系：**世界坐标系**和**相机坐标系**

$$\boldsymbol{p}_c=^cT_w\boldsymbol{p}_w$$

实践当中，$^cT_w$更常见，而$^wT_c$更直观。

## 4. 李群与李代数

把估计**什么样的相机位姿最符合当前观测数据**这样的问题构建为一个**优化**求解问题。旋转矩阵$\boldsymbol{R}$本身是带有约束的，不适合优化。

$\boldsymbol{R}$, $\boldsymbol{T}$对加法是不封闭（$\boldsymbol{R_1}+\boldsymbol{R_2} \notin SO(3)$）的，但是对乘法是封闭的。

群是一种集合加上一种运算的代数结构。满足一下条件：

1. 封闭性：$\forall a_1, a_2 \in A, a_1*a_2 \in A$
2. 结合律：$\forall a_1, a_2, a_3 \in A, (a_1*a_2)*a_3 = a_1*(a_2*a_3)$
3. 幺元：$\exist a_0 \in A, s.t. \forall a \in A, a_0*a=a*a_0=a$
4. 逆：$\forall a \in A, \exist a^{-1} \in A, s.t. a*a^{-1} = a_0$

李群是指具有连续（光滑）性质的群。每个李群都有一个李代数。

对于任意的向量$\boldsymbol{a}$有一个对应的反对称矩阵$\boldsymbol{A}$

$\boldsymbol{a}^\wedge = \boldsymbol{A}; \boldsymbol{A}^\vee = \boldsymbol{a}$

我们可以找到$\phi(t)$

$\dot{R}(t)R(t)^T=\phi(t)^\wedge$

so we further have

$\dot{R}(t)=\phi(t)^\wedge R(t) = \begin{bmatrix} 0 & -\phi_3 & \phi_2 \\\phi_3 & 0 & -\phi_1 \\ -\phi_2 & \phi_1 & 0\end{bmatrix}R(t)$

每对旋转矩阵求一次导数，只需左乘一个$\phi(t)^\wedge$矩阵即可。

把$R(t)$在$t_0=0$附近一阶泰勒展开

$R(t)=R(t_0)+\dot{R}(t)(t-t_0)=R(t_0) +\phi(t_0)^\wedge R(t_0)(t-t_0) = I + \phi(t_0)^\wedge(t)$

$\phi$反映了$R$的导数性质，所以称它在$SO(3)$原点附近的正切空间（Tangent Space）上

令$\phi(t_0)=\phi_0$, we have

$\dot{R}(t)=\phi_0^\wedge R$

求解关于$R$的微分方程，已知$R(0)=I$

$R(t) = exp(\phi_0^\wedge)(t)$

note:以上只对$t=0$附近有效。

旋转矩阵$\boldsymbol{R}$与另一个反对称矩阵$\phi_0^\wedge$通过指数关系发生了联系。$\phi$描述了$\boldsymbol{R}$在局部的导数关系。$\phi$正是对应到$SO(3)$上的李代数$so(3)$。$exp(\phi^\wedge)$反映的正是李群与李代数之间的指数/对数映射。

每个李群都有与之对应的李代数。李代数描述了李群的局部性质。李代数由一个集合$V$，一个数域$F$和一个二元运算$[,]$（李括号）组成。李代数$(V,F,[,])$具有如下性质: 封闭性，双线性，自反性，雅可比等价。三维向量上的叉积x是一个李括号。



$SO(3)$对应的三维向量李代数$\phi$的李括号为

$[\phi_1,\phi_2]=(\Phi_1\Phi_2-\Phi_2\Phi_1)^\vee$

在不引起歧义的情况下，对三维向量和所对应的三维反对称矩阵不加区别

$so(3)=\{\phi\in R^3, \Phi=\phi^\wedge \in R^{3\times3}\}$

$SE(3)$对应的李代数$se(3)$位于$R^6$空间

$se(3)=\{\xi=\begin{bmatrix}\rho \\ \phi \end{bmatrix} \in R^6, \rho \in R^3, \phi \in so(3), \xi^\wedge=\begin{bmatrix} \phi^\wedge & \rho \\ 0^T & 0\end{bmatrix} \in R^{4 \times 4}\}$

$\xi$前三维为平移（但含义与变换矩阵中的平移不同），后三维为旋转。$^\wedge, ^\vee$表示“从向量到矩阵”和“从矩阵到向量“的关系（并不一定是反对称）。$se(3)$的李括号为

$[\xi_1,\xi_2]=(\xi_1^\wedge\xi_2^\wedge-\xi_2^\wedge\xi_1^\wedge)^\vee$



$exp(\phi^\wedge)$被称为指数映射(Exponential Map)。

$$exp(\phi^\wedge)=exp((\theta \boldsymbol{a})^\wedge)=exp(\theta a^\wedge)=\sum_{n=0}^{\infty}\frac{1}{n!}(\theta a^\wedge)^n = I + \theta a^\wedge + \frac{1}{2!}\theta^2a^\wedge a^\wedge+\frac{1}{3!}\theta^3a^\wedge a^\wedge a^\wedge+\frac{1}{4!}\theta^4a^\wedge a^\wedge a^\wedge a^\wedge+...$$

we can prove

$a^\wedge a^\wedge = aa^T-I$

$a^\wedge a^\wedge a^\wedge = -a^\wedge$

$$exp(\phi^\wedge)=aa^T-a^\wedge a^\wedge+(\theta-\frac{1}{3!}\theta^3+\frac{1}{5!}\theta^5-...)-(-\frac{1}{2!}\theta^2+\frac{1}{4!}\theta^4-...)a^\wedge a^\wedge \\ =a^\wedge a^\wedge+I+sin\theta a^\wedge-cos\theta a^\wedge a^\wedge \\ =(1-cos\theta)a^\wedge a^\wedge + I + sin\theta a^\wedge \\ = cos\theta I + (1-cos\theta)aa^T+sin\theta a^\wedge$$

这个式子与罗德里格斯公式一致$R = cos\theta I + (1-cos\theta)aa^T+sin\theta a^\wedge$。这表明$so(3)$实际上就是由所谓的旋转向量组成的空间，而指数映射即罗德里格斯公式。通过它们，我们把$so(3)$中任意一个向量对应到一个位于$SO(3)$的旋转矩阵。反之，通过对数映射也能把$SO(3)$对应到$so(3)$。不过通常不通过泰勒展开来计算对数映射

指数映射是一个满射，存在一对多的情况。如果把旋转角度固定在$\pm\pi$之间，那么李群和李代数是一一对应的。



**SE(3)上的指数映射**

$$exp(\xi^\wedge)=\begin{bmatrix}\sum_{n=0}^{\infty}\frac{1}{n!}(\phi^\wedge)^n & \sum_{n=0}^{\infty}\frac{1}{(n+1)!}(\phi^\wedge)^n\rho \\ O^T & 1\end{bmatrix} \\ \triangleq \begin{bmatrix}R & J\rho \\ 0^T & 1\end{bmatrix}=T$$

where $J=\frac{sin\theta}{\theta}I+(1-\frac{sin\theta}{\theta})aa^T+\frac{1-cos\theta}{\theta}a^\wedge$



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