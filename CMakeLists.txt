
cmake_minimum_required( VERSION 3.10 )

set( TOP_LEVEL_PROJECT_NAME Blur_Example )

project( ${TOP_LEVEL_PROJECT_NAME} )

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( THREADS_PREFER_PTHREAD_FLAG ON )

find_package( Threads REQUIRED )

add_subdirectory( deps/glfw )
add_subdirectory( deps/glm )
add_subdirectory( deps/imgui )

add_executable( ${TOP_LEVEL_PROJECT_NAME}
	src/main.cpp
	src/san_gl_framebuffer.hpp
	src/san_gl_shader.hpp
	src/san_gl_utils.cpp
	src/san_gl_utils.hpp
	src/san_gl_vao.hpp
	src/san_gl_window.cpp
	src/san_gl_window.hpp )

add_dependencies( ${TOP_LEVEL_PROJECT_NAME} glfw glm imgui )

# Output executable to root directory
set_target_properties( ${TOP_LEVEL_PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}" )

target_link_libraries( ${TOP_LEVEL_PROJECT_NAME} PRIVATE glfw3 glm imgui ${CMAKE_DL_LIBS} Threads::Threads )

target_include_directories( imgui PRIVATE ${GLFW_SOURCE_DIR}/include )

target_include_directories( ${TOP_LEVEL_PROJECT_NAME} PRIVATE ${GLFW_SOURCE_DIR}/include )
target_include_directories( ${TOP_LEVEL_PROJECT_NAME} PRIVATE deps )

target_link_directories( ${TOP_LEVEL_PROJECT_NAME} PRIVATE ${GLFW_BINARY_DIR}/src )

# For MSVS
target_link_directories( ${TOP_LEVEL_PROJECT_NAME} PRIVATE ${GLFW_BINARY_DIR}/src/Debug )
target_link_directories( ${TOP_LEVEL_PROJECT_NAME} PRIVATE ${GLFW_BINARY_DIR}/src/Release )
