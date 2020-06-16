#include <iostream>
#include <ctime>

using namespace std;

// eigen
#include <Eigen/Core>
// 稠密矩阵代数运算（逆，特征值等）
#include <Eigen/Dense>

#define MATRIX_SIZE 100

/******************************
* 本程序演示了 Eigen 基本类型的使用
******************************/

int main(int argc, char const *argv[])
{
    /* code */
    // Eigen以矩阵为基本数据单元。Matrix是一个模板类。前三个参数为：数据类型，行，列
    Eigen::Matrix<float, 2, 3> matrix_23;
    // Eigen通过`typedef`定义了许多内置类型，其底层仍为｀Eigen::Matrix｀
    // 例如`Vector3d`实质上是`Eigen::Matrix<double, 3, 1>`
    Eigen::Vector3d v_3d;
    Eigen::Matrix3d matrix_33 = Eigen::Matrix3d::Zero();    // 初始化为零

    // 使用动态尺寸矩阵时，可以不指定矩阵大小
    Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> matrix_dynamic;
    Eigen::MatrixXd matrix_x;

    // 矩阵操作
    // 输入数据
    matrix_23 << 1, 2, 3, 4, 5, 6;
    cout << matrix_23 << endl;

    // 用()访问矩阵中的元素
    for (int i = 0; i < 1; i++)
    {
        for (int j = 0; j < 2; j ++)
        {
            cout << matrix_23(i, j) << endl;
        }
    }

    v_3d << 3, 2, 1;

    // 矩阵和向量相乘（实际上仍是矩阵和矩阵）
    // 但是不能混合数据类型不同的矩阵，需要显式转换
    Eigen::Matrix<double, 2, 1> result = matrix_23.cast<double>() * v_3d;
    cout << result << endl;

    // 矩阵运算
    matrix_33 = Eigen::Matrix3d::Random();
    cout << matrix_33 << endl;

    // 转置
    cout << matrix_33.transpose() << endl;
    // 各元素和
    cout << matrix_33.sum() << endl;
    // 迹
    cout << matrix_33.trace() << endl;
    // 标量相乘
    cout << 10 * matrix_33 << endl;
    // 逆
    cout << matrix_33.inverse() << endl;
    // 行列式
    cout << matrix_33.determinant() << endl;

    // 特征值
    // 实对称矩阵可以保证成功对角化
    Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> eigen_solver ( matrix_33.transpose() * matrix_33 );
    cout << "eigen values = " << eigen_solver.eigenvalues() << endl;
    cout << "eigen vectors = " << eigen_solver.eigenvectors() << endl;

    // 解方程
    // 求解`matrix_NN * x = v_Nd`
    // 直接求逆运算量大
    Eigen::Matrix<double, MATRIX_SIZE, MATRIX_SIZE> matrix_NN;
    matrix_NN = Eigen::MatrixXd::Random(MATRIX_SIZE, MATRIX_SIZE);
    Eigen::Matrix<double, MATRIX_SIZE, 1> v_Nd;
    v_Nd = Eigen::VectorXd::Random(MATRIX_SIZE);

    // 计时
    clock_t time_stt = clock();
    // 直接求逆
    Eigen::Matrix<double, MATRIX_SIZE, 1> x = matrix_NN.inverse() * v_Nd;
    cout << "time use in normal inverse is " << 1000 * (clock() - time_stt) / (double)CLOCKS_PER_SEC << "ms" << endl;

    // 通常用矩阵分解求逆，例如`QR`分解，速度会快很多
    time_stt = clock();
    x = matrix_NN.colPivHouseholderQr().solve(v_Nd);
    cout << "time use in Qr compsition is " << 1000 * (clock() - time_stt) / (double)CLOCKS_PER_SEC << "ms" << endl;

    return 0;
}
