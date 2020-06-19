# 声明要求的 cmake 最低版本
cmake_minimum_required( VERSION 2.8 )

# 声明一个 cmake 工程
project( Sophus )

# 使用find_package命令查找sophus的头文件与库文件
find_package( Sophus REQUIRED )
include_directories( ${Sophus_INCLUDE_DIRS} )

# 添加可执行程序
add_executable( use_sophus use_sophus.cpp )
target_link_libraries( use_sophus ${Sophus_LIBRARIES} )

# 设置编译模式
set( CMAKE_BUILD_TYPE "Debug" )
