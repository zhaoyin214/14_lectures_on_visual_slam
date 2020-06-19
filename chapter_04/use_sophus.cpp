#include <iostream>
#include <cmath>

using namespace std;

#include <Eigen/Core>
#include <Eigen/Geometry>

#include "sophus/so3.h"
#include "sophus/se3.h"

int main(int argc, char const *argv[])
{
    // 沿Z轴转90度的旋转矩阵
    Eigen::Matrix3d R = Eigen::AngleAxisd(M_PI / 2, Eigen::Vector3d(0, 0, 1)).toRotationMatrix();

    // Sophus::SO(3)可以由旋转矩阵构造
    Sophus::SO3 SO3_R(R);
    // 也可由旋转向量构造
    Sophus::SO3 SO3_v(0, 0, M_PI / 2);
    // 或者由四元数构造
    Eigen::Quaterniond q(R);
    Sophus::SO3 SO3_q(q);

    // SO(3)的输出形式为so(3)
    cout << "SO(3) from matrix: " << SO3_R << endl;
    cout << "SO(3) from vector: " << SO3_v << endl;
    cout << "SO(3) from quaternion: " << SO3_q << endl;

    // 使用对数映射获取其李代数
    Eigen::Vector3d so3 = SO3_R.log();
    cout << "so3 = " << so3.transpose() << endl;
    // hat为向量到反对称矩阵
    cout << "so3 hat = " << Sophus::SO3::hat(so3) << endl;
    // vee为反对称矩阵到向量
    cout << "so3 hat = " << Sophus::SO3::vee(Sophus::SO3::hat(so3)).transpose() << endl;

    // 增量扰动模型
    // 扰动
    Eigen::Vector3d update_so3(1e-4, 0, 0);
    // 左乘
    Sophus::SO3 SO3_updated = Sophus::SO3::exp(update_so3) * SO3_R;
    cout << "SO3 updated = " << SO3_updated << endl;

    /************************************************************************/
    cout << "********************************************************" << endl;

    // SE(3)操作
    // 沿X轴平移1
    Eigen::Vector3d t(1, 0, 0);
    // 由R,t构造SE(3)
    Sophus::SE3 SE3_Rt(R, t);
    // 由q,t构造SE(3)
    Sophus::SE3 SE3_qt(q, t);
    cout << "SE3 from R,t= " << SE3_Rt << endl;
    cout << "SE3 from q,t= " << SE3_qt << endl;

    // 李代数se(3)是六维向量
    typedef Eigen::Matrix<double, 6, 1> Vector6d;
    Vector6d se3 = SE3_Rt.log();
    // 在Sophus中，se(3)平移在前，旋转在后
    cout << "se3 = " << se3 << endl;

    // hat，vee运算符
    cout << "se3 hat = " << Sophus::SE3::hat(se3) << endl;
    cout << "se3 hat vee = " << Sophus::SE3::vee(Sophus::SE3::hat(se3)).transpose() << endl;

    // 增量扰动模型
    // 扰动
    Vector6d update_se3;
    update_se3.setZero();
    update_se3(0, 0) = 1e-4;
    Sophus::SE3 SE3_updated = Sophus::SE3::exp(update_se3) * SE3_Rt;
    cout << "SE3 updated = " << SE3_updated.matrix() << endl;

    return 0;

}
