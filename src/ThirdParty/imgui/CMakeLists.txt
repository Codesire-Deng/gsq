cmake_minimum_required(VERSION 3.10.0)

include_directories(AFTER ${PROJECT_SOURCE_DIR}/include/ThirdParty/imgui)


set(imgui_SRCS
    imgui_demo.cpp
    imgui_draw.cpp
    imgui_impl_glfw.cpp
    imgui_impl_opengl3.cpp
    imgui_tables.cpp
    imgui_widgets.cpp
    imgui.cpp
)


if (WIN32)
    set(imgui_SRCS ${imgui_SRCS} imgui_impl_win32.cpp)
endif(WIN32)

if (UNIX)
    message("WARN: untested platform UNIX")
endif(UNIX)

if (APPLE)
    set(imgui_SRCS ${imgui_SRCS} imgui_impl_osx.mm)
endif(APPLE)


add_library(imgui STATIC ${imgui_SRCS})