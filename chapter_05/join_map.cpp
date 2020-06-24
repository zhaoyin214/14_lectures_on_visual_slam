#include <iostream>
#include <fstream>
using namespace std;

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <Eigen/Geometry>
#include <boost/format.hpp>
#include <pcl/io/pcd_io.h>
#include <pcl/visualization/pcl_visualizer.h>

int main(int argc, char const *argv[])
{
    // 彩色图和深度图
    vector<cv::Mat> color_images, depth_images;
    vector<Eigen::Isometry3d> poses;

    ifstream fin("../pose.txt");
    if (!fin)
    {
        cerr << "pose.txt路径错误" << endl;
        return 1;
    }

    for (size_t i = 0; i < 5; i++)
    {
        // 图像文件命名格式
        boost::format fmt("../%s/%d.%s");\
        color_images.push_back(
            cv::imread((fmt % "color" % (i + 1) % "png").str())
        );
        // `cv::IMREAD_UNCHANGED`读取原始图像
        depth_images.push_back(
            cv::imread((fmt % "depth" % (i + 1) % "pgm").str(), cv::IMREAD_UNCHANGED)
        );

        // (7, )数组，相机位姿｀[x, y, z, q_{x}, q_{y}, q_{z}, q_{w}]｀
        double data[7] = {0};
        for (auto & d: data)
        {
            fin >> d;
        }
        Eigen::Quaterniond q(data[6], data[3], data[4], data[5]);
        Eigen::Isometry3d T(q);
        T.pretranslate(Eigen::Vector3d(data[0], data[1], data[2]));
        poses.push_back(T);
    }

    // 计算点云并拼接
    // 相机内参
    double cx = 325.5;
    double cy = 253.5;
    double fx = 518.0;
    double fy = 519.0;
    double depth_scale = 1000.0;

    cout << "将图像转换为点云..." << endl;
    // 定义点云使用的格式：`XYZRGB`
    typedef pcl::PointXYZRGB PointT;
    typedef pcl::PointCloud<PointT> PointCloud;

    // 新建一个点云
    PointCloud::Ptr point_cloud(new PointCloud);
    for (size_t i = 0; i < 5; i++)
    {
        cout << "转换图像：" << i + 1 << endl;
        cv::Mat color = color_images[i];
        cv::Mat depth = depth_images[i];
        Eigen::Isometry3d T = poses[i];

        for (size_t v = 0; v < color.rows; v++)
        {
            for (size_t u = 0; u < color.cols; u++)
            {
                // 深度值
                unsigned int d = depth.ptr<unsigned short>(v)[u];
                // 0表示测量值无效
                if (d == 0)
                {
                    continue;
                }
                Eigen::Vector3d point;
                point[2] = double(d) / depth_scale;
                point[0] = (u - cx) * point[2] / fx;
                point[1] = (v - cy) * point[2] / fy;
                Eigen::Vector3d point_world = T * point;

                PointT p;
                p.x = point_world[0];
                p.y = point_world[1];
                p.z = point_world[2];
                p.b = color.data[v * color.step + u * color.channels()];
                p.g = color.data[v * color.step + u * color.channels() + 1];
                p.r = color.data[v * color.step + u * color.channels() + 2];
                point_cloud->points.push_back(p);
            }
        }
    }
    point_cloud->is_dense = false;
    cout << "点云共有" << point_cloud->size() << "个点。" << endl;
    pcl::io::savePCDFileBinary("map.pcd", *point_cloud);

    return 0;
}