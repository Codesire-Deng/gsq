cmake_minimum_required(VERSION 3.10.0)


# 设置exe输出目录为./build
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})

add_subdirectory(./ThirdParty)

# main.cpp -> main.exe （当然还需要链接各种库）
add_executable(main main.cpp glad.c convex.cpp shader.cpp)

# 链接自定义库
# target_link_libraries(main PRIVATE common object)

# 链接第三方库
target_link_libraries(main PRIVATE glfw3 imgui)

# -------------------------------------------------
# 到这里 Exe 已经生成完了。 接下来将运行时需要的文件复制一下
# -------------------------------------------------

# 拷贝data到build/Debug/data
add_custom_command(TARGET main POST_BUILD  
    COMMAND ${CMAKE_COMMAND} -E copy_directory 
    "${PROJECT_SOURCE_DIR}/data"     
    $<TARGET_FILE_DIR:main>/data
)    

# 拷贝shaders到build/Debug/shader
add_custom_command(TARGET main POST_BUILD  
    COMMAND ${CMAKE_COMMAND} -E copy_directory 
    "${PROJECT_SOURCE_DIR}/src/shader"     
    $<TARGET_FILE_DIR:main>/shader
)   

# 拷贝动态库文件到build/Debug
# add_custom_command(TARGET main POST_BUILD       # Adds a post-build event to main
#     COMMAND ${CMAKE_COMMAND} -E copy_directory  # which executes "cmake - E copy_if_different..."
#     "${PROJECT_SOURCE_DIR}/lib"                 # <--this is in-file
#     $<TARGET_FILE_DIR:main>                     # <--this is out-file path
# )                


# # 拷贝assets到build/Debug/assets
# add_custom_command(TARGET main POST_BUILD   
#     COMMAND ${CMAKE_COMMAND} -E copy_directory  
#     "${PROJECT_SOURCE_DIR}/assets"      
#     $<TARGET_FILE_DIR:main>/assets
# )    

# # 拷贝capture到build/Debug/assets
# add_custom_command(TARGET main POST_BUILD   
#     COMMAND ${CMAKE_COMMAND} -E copy_directory  
#     "${PROJECT_SOURCE_DIR}/capture"      
#     $<TARGET_FILE_DIR:main>/capture
# )