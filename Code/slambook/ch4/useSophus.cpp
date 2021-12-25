#include <iostream>
#include <cmath>

#include <Eigen/Core>
#include <Eigen/Geometry>

#include "sophus/se3.hpp"
#include "sophus/so3.hpp"

using namespace std;
using namespace Eigen;

int main() {
	// 90deg rotation about z axis
	Matrix3d R = AngleAxisd(M_PI/2, Vector3d(0, 0, 1)).toRotationMatrix();
    // or quaternion
    Quaterniond q(R);
    Sophus::SO3d SO3_R(R);  // construct from rotation matrix
    Sophus::SO3d SO3_q(q);  // construct from quaternion

    cout << "Rotation matrix:\n" << R << endl;
    cout << "SO(3) from matrix:\n" << SO3_R.matrix() << endl;
    cout << "SO(3) from quaternion:\n" << SO3_q.matrix() << endl;
    cout << "they are equal" << endl;

    // log mapping to get Lie algebra
    Vector3d so3 = SO3_R.log();
    cout << "so3 = " << so3.transpose() << endl;
    // hat: skew-symmetric matrix from vector
    cout << "so3 hat = \n" << Sophus::SO3d::hat(so3) << endl;
    // vee: vector from skew-symmetric matrix
    cout << "so3 hat vee = " << Sophus::SO3d::vee(Sophus::SO3d::hat(so3)).transpose() << endl;

    // update of disturbance model
    Vector3d update_so3(1e-4, 0, 0);
    Sophus::SO3d SO3_updated = Sophus::SO3d::exp(update_so3) * SO3_R;
    cout << "SO3_updated: \n" << SO3_updated.matrix() << endl;
    cout << "*******************************" << endl;

    // For SE(3)
    Vector3d t(1, 0, 0);
    Sophus::SE3d SE3_Rt(R, t);  // construct from R, t
    Sophus::SE3d SE3_qt(q, t);  // construct from q, t
    cout << "SE3 from R, t = \n" << SE3_Rt.matrix() << endl;
    cout << "SE3 from q, t = \n" << SE3_qt.matrix() << endl;
    // Lie algebra se(3) is a vector of size 6
    typedef Eigen::Matrix<double, 6, 1>  Vector6d;
    Vector6d se3 = SE3_Rt.log();
    cout << "se3 = " << se3.transpose() << endl;  // translation is before rotation part
    cout << "se3 hat = \n" << Sophus::SE3d::hat(se3) << endl;
    cout << "se3 hat vee = " << Sophus::SE3d::vee(Sophus::SE3d::hat(se3)).transpose() << endl;

    // update with disturbance
    Vector6d update_se3;
    update_se3.setZero();
    update_se3(0, 0) = 1e-4;
    Sophus::SE3d SE3_updated = Sophus::SE3d::exp(update_se3) * SE3_Rt;
    cout << "SE3 updated = \n" << SE3_updated.matrix() << endl;

    return 0;
}
