cmake_minimum_required( VERSION 3.10 )

project( main )

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED True )

find_package( glm REQUIRED )
find_package( glfw3 REQUIRED )

add_subdirectory( imgui )
add_executable( main main.cpp san_gl_utils.cpp san_gl_window.cpp )

target_include_directories( main PRIVATE ${PROJECT_SOURCE_DIR} )
target_include_directories( main PRIVATE ${PROJECT_SOURCE_DIR}/imgui )
target_include_directories( main PRIVATE ${PROJECT_SOURCE_DIR}/stb )

target_link_libraries( main PRIVATE imgui glm glfw3 )
