# 声明要求的 cmake 最低版本
cmake_minimum_required( VERSION 2.8 )

# 声明一个 cmake 工程
project( Sophus )

# 添加 c++ 11 标准支持
set( CMAKE_CXX_FLAGS "-std=c++11" )

# OpenCV库
find_package( OpenCV REQUIRED )
# 添加头文件
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable( image_basics image_basics.cpp )
# 链接OpenCV库
target_link_libraries( image_basics ${OpenCV_LIBS} )
