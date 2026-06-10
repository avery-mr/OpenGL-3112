cmake_minimum_required(VERSION 3.20)

project(OpenGL_3112)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(external/glfw)
add_library(glad
	external/glad/src/glad.c
)

add_executable(01_window
	src/01_window/main.cpp
)

target_include_directories(glad
	PUBLIC 
	external/glad/include 
)

target_link_libraries(01_window
	PRIVATE
	glfw
	glad
	opengl32
)