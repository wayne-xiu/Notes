#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>

using namespace std;

#include <Eigen/Core>
#include <Eigen/Geometry>

int main()
{
    cout.precision(3);

    // robot1 pose
    Eigen::Quaterniond robot1_rot = Eigen::Quaterniond(0.35, 0.2, 0.3, 0.1);
    robot1_rot.normalize();
    Eigen::Vector3d robot1_trans = Eigen::Vector3d(0.3, 0.1, 0.1);
    robot1_rot = robot1_rot.inverse();
    robot1_trans = robot1_rot * (-1*robot1_trans);

    // robot2 pose
    Eigen::Quaterniond robot2_rot = Eigen::Quaterniond(-0.5, 0.4, -0.1, 0.2);
    robot2_rot.normalize();
    Eigen::Vector3d robot2_trans = Eigen::Vector3d(-0.1, 0.5, 0.3);


    Eigen::Vector3d pInRobot1 = Eigen::Vector3d(0.5, 0, 0.2);
    Eigen::Vector3d pInRobot2 = robot1_rot * pInRobot1 + robot1_trans;
    pInRobot2 = robot2_rot * pInRobot2 + robot2_trans;

    cout << "p point in robot2 frame:\n" << pInRobot2.transpose() << endl;

    return 0;
}
