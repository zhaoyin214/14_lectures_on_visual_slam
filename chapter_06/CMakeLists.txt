# 声明要求的 cmake 最低版本
cmake_minimum_required( VERSION 2.8 )

# 声明一个 cmake 工程
project( Ceres )

# 添加 c++ 14 标准支持
set( CMAKE_CXX_FLAGS "-std=c++14" )

# 添加cmake模块以使用ceres库
list( APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/../cmake_modules )

# ceres
find_package( Ceres REQUIRED )
include_directories( ${Ceres_INCLUDE_DIRS} )

# opencv
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

# eigen
include_directories( "/usr/include/eigen3" )

add_executable( ceres_curve_fitting ceres_curve_fitting.cpp )
target_link_libraries( ceres_curve_fitting ${Ceres_LIBS} ${OpenCV_LIBS})
