cmake_minimum_required( VERSION 3.10 )

project( Blur_Example )

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED True )

find_package( glm REQUIRED )
find_package( glfw3 REQUIRED )

add_subdirectory( imgui )
add_executable( Blur_Example src/main.cpp src/san_gl_utils.cpp src/san_gl_window.cpp )

target_include_directories( Blur_Example PRIVATE ${PROJECT_SOURCE_DIR} )
target_include_directories( Blur_Example PRIVATE ${PROJECT_SOURCE_DIR}/src )
target_include_directories( Blur_Example PRIVATE ${PROJECT_SOURCE_DIR}/stb )
target_include_directories( Blur_Example PRIVATE ${PROJECT_SOURCE_DIR}/imgui )

target_link_libraries( Blur_Example PRIVATE imgui glm glfw3 )
