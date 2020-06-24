# 声明要求的 cmake 最低版本
cmake_minimum_required( VERSION 2.8 )

# 声明一个 cmake 工程
project( Sophus )

# 添加 c++ 14 标准支持
set( CMAKE_CXX_FLAGS "-std=c++14" )

# opencv
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

# eigen
include_directories( "/usr/include/eigen3" )

# pcl
find_package( PCL REQUIRED COMPONENT common io )
include_directories( ${PCL_INCLUDE_DIRS} )
add_definitions( ${PCL_DEFINITIONS} )

add_executable( image_basics image_basics.cpp )
target_link_libraries( image_basics ${OpenCV_LIBS} )

add_executable( join_map join_map.cpp )
target_link_libraries( join_map ${OpenCV_LIBS} ${PCL_LIBRARIES} )
