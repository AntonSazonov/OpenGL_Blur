cmake_minimum_required( VERSION 3.10 )

project( Blur_Example )

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

add_subdirectory( deps/glfw )
add_subdirectory( deps/glm )
add_subdirectory( deps/imgui )

add_executable( Blur_Example src/main.cpp src/san_gl_utils.cpp src/san_gl_window.cpp )

target_include_directories( Blur_Example PRIVATE ${PROJECT_SOURCE_DIR}/deps )
target_include_directories( Blur_Example PRIVATE ${PROJECT_SOURCE_DIR}/deps/src )
target_include_directories( Blur_Example PRIVATE ${PROJECT_SOURCE_DIR}/deps/stb )
target_include_directories( Blur_Example PRIVATE ${PROJECT_SOURCE_DIR}/deps/imgui )

target_link_libraries( Blur_Example PRIVATE imgui glm glfw3 )
