cmake_minimum_required( VERSION 3.10 )

project( Blur_Example )

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

add_subdirectory( deps/glfw )
add_subdirectory( deps/glm )
add_subdirectory( deps/imgui )

add_executable( Blur_Example src/main.cpp src/san_gl_utils.cpp src/san_gl_window.cpp )

target_include_directories( imgui PRIVATE ${PROJECT_SOURCE_DIR}/deps/glfw/include )
target_include_directories( Blur_Example PRIVATE ${PROJECT_SOURCE_DIR}/deps/glfw/include )
target_include_directories( Blur_Example PRIVATE ${PROJECT_SOURCE_DIR}/deps )

target_link_directories( Blur_Example PUBLIC ${PROJECT_BINARY_DIR}/deps/glfw/src )

# For MSVS
target_link_directories( Blur_Example PUBLIC ${PROJECT_BINARY_DIR}/deps/glfw/src/Debug )
target_link_directories( Blur_Example PUBLIC ${PROJECT_BINARY_DIR}/deps/glfw/src/Release )

target_link_libraries( Blur_Example imgui glm glfw3 )
