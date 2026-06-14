cmake_minimum_required(VERSION 3.20)

project(OpenGL_3112)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(external/glfw)
add_library(glad
	external/glad/src/glad.c
 )

target_include_directories(glad
	PUBLIC 
	external/glad/include 
)

add_executable(01_environment_setup
	weekly/01_environment_setup/main.cpp
 )

target_link_libraries(01_environment_setup
	PRIVATE
	glfw
	glad
	opengl32
)

add_executable(02_single_triangle
	"weekly/02_triangle/01_single_triangle.cpp"
 )

target_link_libraries(02_single_triangle
	PRIVATE
	glfw
	glad
	opengl32
)