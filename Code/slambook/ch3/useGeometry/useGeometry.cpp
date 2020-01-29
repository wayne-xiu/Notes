#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>

using namespace std;

#include <Eigen/Core>
#include <Eigen/Geometry>

int main()
{

    cout.precision(3);
    // 3D rotation matrix
    Eigen::Matrix3d rotation_matrix = Eigen::Matrix3d::Identity();

    // AngleAxis, it's not Matrix, but could be used as matrix in calculation
    // due to operator overloading
    // rotate around z by 45deg
    Eigen::AngleAxisd rotation_vector(M_PI/4, Eigen::Vector3d(0, 0, 1));
    // cout << rotation_vector << endl;  // this is invalid
    cout << "Angle-axis representation:\n" << "rotation angle:\n";
    cout << rotation_vector.angle()/M_PI*180 << endl;
    cout << "rotation axis:\n";
    cout << rotation_vector.axis() << endl;
    // print in matrix form
    cout << "rotation matrix:\n" << rotation_vector.matrix() << endl;
    // convert to rotation matrix form
    rotation_matrix = rotation_vector.toRotationMatrix();
    // cout << rotation_matrix << endl;
    // AngleAxis can be used for rotation directly
    Eigen::Vector3d v(1, 0, 0);
    Eigen::Vector3d v_rotated = rotation_vector * v;  // operator overloading
    cout << "(1, 0, 0) after rotation:\n" << v_rotated.transpose() << endl;
    // or use rotation matrix
    v_rotated = rotation_vector.toRotationMatrix() * v;
    cout << "(1, 0, 0) after rotation:\n" << v_rotated.transpose() << endl;

    // Euler angles
    Eigen::Vector3d euler_angles = rotation_matrix.eulerAngles(2, 1, 0);  // zyx order, i.e. yaw, pitch, roll
    cout << "yaw pitch roll:\n" << euler_angles.transpose() << endl;

    // Transformation with Eigen::Isometry
    Eigen::Isometry3d T = Eigen::Isometry3d::Identity();  // called 3d, but actually 4x4
    // cout << "Identity transformation:\n" << T << endl;  // this is invalid
    T.rotate(rotation_vector);
    T.pretranslate(Eigen::Vector3d(1, 3, 4));
    cout << "Transformation matrix after rotation and translation:\n" << T.matrix() << endl;
    // Transformation matrix can be used for transformation directly without worrying about actual size of T
    Eigen::Vector3d v_transformed = T * v;
    cout << "(1, 0, 0) after transformation:\n" << v_transformed.transpose() << endl;

    // Quaternion (notice the order is x, y, z, w) where w is the real part - counter-intuitive
    // construct Quatersion from AngleAxis representation
    Eigen::Quaterniond q = Eigen::Quaterniond(rotation_vector);
    cout << "quaternion:\n" << q.coeffs().transpose() << endl;
    // from rotation_matrix
    q = Eigen::Quaterniond(rotation_matrix);
    cout << "quaternion:\n" << q.coeffs().transpose() << endl;
    // Quaternion can be used for rotation directly
    v_rotated = q * v;  // operation overoading; note: it's qvq^{-1} mathematically
    cout << "(1, 0, 0) after rotation:\n" << v_rotated.transpose() << endl;

    // Affine3d: 4x4
    Eigen::Affine3d T_affine = Eigen::Affine3d::Identity();
    T_affine.rotate(rotation_vector);
    // T_affine.translate(Eigen::Vector3d(1, 3, 4));
    T_affine.pretranslate(Eigen::Vector3d(1, 3, 4));
    // cout << T_affine.affine() << endl;
    cout << "Invertible matrix A of Affine:\n" << T_affine.linear() << endl;
    cout << "Translation t of Affine:\n" << T_affine.translation().transpose() << endl;
    v_rotated = T_affine * v;
    cout << "(1, 0, 0) after affine transformation:\n" << v_rotated.transpose() << endl;
    v_rotated = T_affine.linear() * v;
    cout << "(1, 0, 0) after rotation:\n" << v_rotated.transpose() << endl;

    // Projective3d: 4x4

    return 0;
}
