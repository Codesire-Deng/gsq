cmake_minimum_required(VERSION 3.20.0)

project(GSQ VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

# Debug or Release
set(MY_BUILD_TYPE Debug)
# set(MY_BUILD_TYPE Release)

message("MY_BUILD_TYPE=${MY_BUILD_TYPE}")
message("CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
message("CMAKE_CONFIGURATION_TYPES=${CMAKE_CONFIGURATION_TYPES}")

if(MY_BUILD_TYPE MATCHES Debug)
    add_definitions(-DDEBUG)    # 启用部分调试信息
else(MY_BUILD_TYPE MATCHES Debug)
    add_definitions(-DNDEBUG)
    add_definitions(-DEIGEN_NO_DEBUG)
endif(MY_BUILD_TYPE MATCHES Debug)

add_definitions(-D_CRT_SECURE_NO_WARNINGS) # 禁用MSVC关于fopen的编译警告

# Path to dependencies
if (WIN32)
    set(BOOST_ROOT "C:/libraries/boost_1_77_0")
    set(EIGEN_ROOT "C:/libraries/eigen-3.4.0")
endif(WIN32)

if (UNIX)
    set(BOOST_ROOT "/usr/local/boost_1_77_0")
    set(EIGEN_ROOT "/usr/local/eigen-3.4.0")
endif(UNIX)
# END Path to dependencies

# Boost Library
find_package(Boost REQUIRED)
include_directories("${Boost_INCLUDE_DIR}")
message("Boost include dir=${Boost_INCLUDE_DIR}")
# END Boost Library

# Eigen
include_directories("${EIGEN_ROOT}")
message("Eigen include dir=${EIGEN_ROOT}")
# END Eigen

# Dear ImGui

# END Dear ImGui

# 指定自有头文件搜索目录
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/ThirdParty)

# 指定静态/动态库所在目录
link_directories(${PROJECT_SOURCE_DIR}/lib/${MY_BUILD_TYPE})
message("link_directories=${PROJECT_SOURCE_DIR}/lib/${MY_BUILD_TYPE}")

# add_compile_options(-bigobj)


# 执行src/CMakeLists.txt
add_subdirectory(./src)

