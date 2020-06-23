#include <iostream>
#include <chrono>

using namespace std;

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

int main(int argc, char const *argv[])
{
    // 读取argv[1]指定的图像
    cv::Mat image;
    // `cv::imread`函数读取给定路径的图像
    image = cv::imread(argv[1]);

    // 判断图像文件是否正确读取
    if (image.data == nullptr)
    {
        // 数据不存在，可能是文件不存在
        cerr << "文件" << argv[1] << "不存在" << endl;
        return 0;
    }

    // 文件顺利读取，输出基本信息
    cout << "图像宽为" << image.cols << ",高为" << image.rows << ",通道数为" << image.channels() << endl;
    // `cv::imshow`显示图像
    cv::imshow("image", image);
    // 暂停程序，等待按键输入
    cv::waitKey(0);

    // 判断 image 的类型
    if (image.type() != CV_8UC1 && image.type() != CV_8UC3)
    {
        // 图像类型不符合要求
        cout << "请输入一张彩色图或灰度图" << endl;
        return 0;
    }

    // 遍历图像，请注意以下遍历方式亦可使用于随机访问
    // 使用`std::chrono`为算法计时
    chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
    for (size_t y = 0; y < image.rows; y++)
    {
        for (size_t x = 0; x < image.cols; x++)
        {
            // 访问(x, y)处的像素
            // 用`cv::Mat::ptr`获得图像行指针
            // row_ptr是第y行的头指针
            unsigned char * row_ptr = image.ptr<unsigned char>(y);
            // data_ptr指向待访问的像素数据
            unsigned char * data_ptr = &row_ptr[x * image.channels()];
            // 输出该像素的每个通道，如果是灰度图就只有一个通道
            for (int c = 0; c != image.channels(); c++)
            {
                // data为I(x,y)第c个通道的值
                unsigned char data = data_ptr[c];
            }

        }

    }

    chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
    chrono::duration<double> time_used = chrono::duration_cast<chrono::duration<double>>(t2 - t1);
    cout << "遍历图像用时：" << time_used.count() << "秒。" << endl;

    // 关于`cv::Mat`的拷贝
    // 直接赋值并不会拷贝数据
    cv::Mat image_another = image;
    // 修改`image_another`会导致`image`发生变化
    // 将左上角100*100的块置零
    image_another(cv::Rect(0, 0, 100, 100)).setTo(0);
    cv::imshow("image", image);
    cv::waitKey(0);

    // 使用`clone`函数拷贝数据
    cv::Mat image_clone = image.clone();
    image_clone(cv::Rect(0, 0, 100, 100)).setTo(255);
    cv::imshow("image", image);
    cv::imshow("image_clone", image_clone);
    cv::waitKey(0);

    cv::destroyAllWindows();

    return 0;
}
