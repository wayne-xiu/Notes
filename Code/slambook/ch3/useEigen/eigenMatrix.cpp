#include <iostream>
#include <ctime>
using namespace std;

#include <Eigen/Core>
#include <Eigen/Dense>

#define MATRIX_SIZE 50

int main() {
	Eigen::Matrix<float, 2, 3> matrix_23;
	// Eigen provides many built-in types through typedef, but still Eigen::Matrix in nature
	Eigen::Vector3d v_3d;
	Eigen::Matrix3d matrix_33 = Eigen::Matrix3d::Zero();
	// dynamic size matrix
	Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> matrix_dynamic;
	Eigen::MatrixXd matrix_x;

	matrix_23 << 1, 2, 3, 4, 5, 6;
	cout << matrix_23 << endl;

	for (int i = 0; i < 1; ++i) {
		for (int j = 0; j < 2; ++j)
			cout << matrix_23(i, j) << endl;
	}

	v_3d << 3, 2, 1;

	Eigen::Matrix<double, 2, 1> result = matrix_23.cast<double>() * v_3d;
	cout << result << endl;

	matrix_33 = Eigen::Matrix3d::Random();
	cout << matrix_33 << endl << endl;

	// matrix operations
	cout << "Transpose: " << endl << matrix_33.transpose() << endl;
	cout << "Elements sum: " << endl << matrix_33.sum() << endl;
	cout << "Diagonal trace: " << endl << matrix_33.trace() << endl;
	cout << "Multiplication: " << endl << matrix_33 * 10 << endl;
	cout << "Inverse: " << endl << matrix_33.inverse() << endl;
	cout << "Determinant: " << endl << matrix_33.determinant() << endl;
	
	// Eigen calculations
	Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> eigen_solver(matrix_33.transpose() * matrix_33);
	cout << "Eigen values: " << endl << eigen_solver.eigenvalues() << endl;
	cout << "Eigen vector: " << endl << eigen_solver.eigenvectors() << endl;

	// Solve Ax = b
	Eigen::Matrix<double, MATRIX_SIZE, MATRIX_SIZE> matrix_A;
	matrix_A = Eigen::MatrixXd::Random(MATRIX_SIZE, MATRIX_SIZE);
	Eigen::Matrix<double, MATRIX_SIZE, 1> v_b;
	v_b = Eigen::MatrixXd::Random(MATRIX_SIZE, 1);

	clock_t time_stt = clock();
	// using inverse directly
	Eigen::Matrix<double, MATRIX_SIZE, 1> x = matrix_A.inverse()*v_b;
	cout << "time use in normal inverse is: " << 1000*(clock() - time_stt)/(double)CLOCKS_PER_SEC << "ms" << endl;

	// using matrix decomposition
	time_stt = clock();
	x = matrix_A.colPivHouseholderQr().solve(v_b);
	cout << "time use in QR decompostion is: " << 1000*(clock() - time_stt)/(double)CLOCKS_PER_SEC << "ms" << endl;

	return 0;
}