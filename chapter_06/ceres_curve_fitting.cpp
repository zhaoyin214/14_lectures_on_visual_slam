// ceres_curve_fitting/main.cpp

#include <iostream>
#include <opencv2/core/core.hpp>
#include <ceres/ceres.h>
#include <chrono>

using namespace std;

// 代价函数的计算模型
struct CURVE_FITTING_COST
{
    CURVE_FITTING_COST(double x, double y) : _x(x), _y(y) {}
    // 残差计算
    template <typename T> bool operator() (
        // 模型参数，3维
        const T * const abc,
        // 残差
        T * residual
    ) const
    {
        // y - exp(ax^2 + bx + c)
        residual[0] = T(_y) - ceres::exp(
            abc[0] * T(_x) * T(_x) +
            abc[1] * T(_x) +
            abc[2]
        );
        return true;
    }
    // x, y
    const double _x;
    const double _y;
};

int main(int argc, char ** argv)
{
    // 真实参数值
    double a = 1.0;
    double b = 2.0;
    double c = 1.0;
    // 数据点
    int N = 100;
    // 噪声sigma
    double w_sigma = 1.0;
    // opencv随机数生成
    cv::RNG rng;
    // 参数估计
    double abc[3] = {0, 0, 0};
    // 数据
    vector <double> x_data;
    vector <double> y_data;

    cout << "generating data: " << endl;
    for (size_t i = 0; i < N; i++)
    {
        double x = i / 100.0;
        x_data.push_back(x);
        y_data.push_back(
            exp(a * x * x + b * x + c) + rng.gaussian(w_sigma)
        );
        cout << x_data[i] << " " << y_data[i] << endl;
    }

    // 构建最小二乘问题
    ceres::Problem problem;
    for (size_t i = 0; i < N; i++)
    {
        problem.AddResidualBlock(
            // 添加误差项
            // 自动求导，模板参数：误差类型，输出维度，输入维度
            // 数值参照struct
            new ceres::AutoDiffCostFunction <CURVE_FITTING_COST, 1, 3> (
                new CURVE_FITTING_COST(x_data[i], y_data[i])
            ),
            // 核函数
            nullptr,
            // 待估计参数
            abc
        );
    }
    // 配置求解器
    ceres::Solver::Options options;
    // 增量方程求解器
    options.linear_solver_type = ceres::DENSE_QR;
    // cout输出
    options.minimizer_progress_to_stdout = true;
    // 优化信息
    ceres::Solver::Summary summary;
    chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
    // 开始优化
    ceres::Solve(options, & problem, & summary);
    chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
    chrono::duration <double> time_used = chrono::duration_cast <chrono::duration <double>> (t2 - t1);
    cout << "solve time cost = " << time_used.count() << " seconds" << endl;

    // 输出结果
    cout << summary.BriefReport() << endl;
    cout << "estimated a, b, c = ";
    for (auto a : abc)
    {
        cout << a << " ";
    }
    cout << endl;

    return 0;
}
