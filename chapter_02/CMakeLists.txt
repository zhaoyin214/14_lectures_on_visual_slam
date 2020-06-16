# 声明要求的 cmake 最低版本
cmake_minimum_required( VERSION 2.8 )

# 声明一个 cmake 工程
project( HelloSLAM )

# 添加一个可执行程序
# 语法：add_executable( 程序名 源代码文件 ）
add_executable( hello_slam hello_slam.cpp )
add_executable( use_hello use_hello.cpp )
target_link_libraries( use_hello hello_shared )

# 静态库
add_library( hello lib_hello_slam.cpp )

# 共享库
add_library( hello_shared SHARED lib_hello_slam.cpp )

# 设置编译模式
set( CMAKE_BUILD_TYPE "Debug" )
