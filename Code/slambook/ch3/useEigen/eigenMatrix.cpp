#include <iostream>
#include <ctime>
using namespace std;

#include <Eigen/Core>
#include <Eigen/Dense>

#define MATRIX_SIZE 50

int main() {
	Eigen::Matrix<float, 2, 3> matrix_23;
	Eigen::Vector3d v_3d;
	Eigen::Matrix3d mamtrix_33 = Eigen::Matrix3d::Zero();
	Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> matrix_dynamic;
	Eigen::MatrixXd matrix_x;

	matrix_23 << 1, 2, 3, 4, 5, 6;
	cout << matrix_23 << endl;

	return 0;
}