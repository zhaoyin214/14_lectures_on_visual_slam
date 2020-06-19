#include <iostream>
#include <cmath>

using namespace std;

#include <Eigen/Core>
#include <Eigen/Geometry>

/**********************************
* 本程序演示了 Eigen 几何模块的使用方法
**********************************/

int main(int argc, char const *argv[])
{
    /* code */
    // `Eigen/Geometry`模块提供各种旋转和平移表示
    // 3D旋转矩阵直接使用`Matrix3d`或`Matrix3f`
    Eigen::Matrix3d rotation_matrix = Eigen::Matrix3d::Identity();

    // 旋转向量使用`AngleAxis`，其底层不｀Matrix｀，但通过重载运算符，可以进行矩阵运算可
    // 沿Z轴旋转45度
    Eigen::AngleAxisd rotation_vector(M_PI / 4, Eigen::Vector3d(0, 0, 1));
    cout.precision(3);
    // 用`matrix()`转换成矩阵
    cout << "rotation matrix = \n" << rotation_vector.matrix() << endl;
    // 直接赋值
    rotation_matrix = rotation_vector.toRotationMatrix();

    // 用`AngleAxis`进行坐标变换
    Eigen::Vector3d v(1, 0, 0);
    Eigen::Vector3d v_rotated = rotation_vector * v;
    cout << "(1,0,0) after rotation = " << v_rotated.transpose() << endl;
    // 用旋转矩阵
    v_rotated = rotation_matrix * v;
    cout << "(1,0,0) after rotation = " << v_rotated.transpose() << endl;

    // 欧拉角：将旋转矩阵转换成欧拉角
    // ZYX顺序，`yaw-pitch-roll`
    Eigen::Vector3d euler_angle = rotation_matrix.eulerAngles(2, 1, 0);
    cout << "yaw pitch roll = " << euler_angle.transpose() << endl;

    // 欧氏变换矩阵使用`Eigen::Isometry`
    // 实质是4＊4的矩阵
    Eigen::Isometry3d T = Eigen::Isometry3d::Identity();
    // 按rotation_vector旋转
    T.rotate(rotation_vector);
    // 平移向量设成(1,3,4)
    T.pretranslate(Eigen::Vector3d(1, 3, 4));

    // 用变换矩阵进行坐标变换
    // R * v + t
    Eigen::Vector3d v_transformed = T * v;
    cout << "v tranformed = " << v_transformed.transpose() << endl;

    // 仿射和射影变换，`Eigen::Affine3d`和`Eigen::Projective3d`

    // 四元数
    // `AngleAxis`可直接赋值四元数，反之亦然
    Eigen::Quaterniond q = Eigen::Quaterniond(rotation_vector);
    // coeffs的顺序是`(x,y,z,w)`，`w`为实部，前三者为虚部
    cout << "quaternion = \n" << q.coeffs() << endl;
    // 旋转矩阵赋值四元数
    q = Eigen::Quaterniond(rotation_matrix);
    cout << "quaternion = \n" << q.coeffs() << endl;

    // 使用四元数旋转一个向量，用重载的乘法
    // 数学表达式为｀qvq^{-1}｀
    v_rotated = q * v;
    cout << "(1,0,0) after rotation = " << v_rotated.transpose() << endl;


    return 0;
}
