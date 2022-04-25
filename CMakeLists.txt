
cmake_minimum_required( VERSION 3.10 )

project( Blur_Example )

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

add_subdirectory( deps/glfw )
add_subdirectory( deps/glm )
add_subdirectory( deps/imgui )

add_executable( Blur_Example src/main.cpp src/san_gl_utils.cpp src/san_gl_window.cpp )
add_dependencies( Blur_Example glfw glm imgui )

target_link_libraries( Blur_Example PRIVATE glfw3 glm imgui ${CMAKE_DL_LIBS} )

target_include_directories( imgui PRIVATE ${GLFW_SOURCE_DIR}/include )

target_include_directories( Blur_Example PRIVATE ${GLFW_SOURCE_DIR}/include )
target_include_directories( Blur_Example PRIVATE deps )

target_link_directories( Blur_Example PRIVATE ${GLFW_BINARY_DIR}/src )

# For MSVS
target_link_directories( Blur_Example PRIVATE ${GLFW_BINARY_DIR}/src/Debug )
target_link_directories( Blur_Example PRIVATE ${GLFW_BINARY_DIR}/src/Release )
