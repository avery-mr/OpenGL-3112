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

add_executable(0201_single_triangle
	weekly/02_triangle/01_single_triangle.cpp
 )

target_link_libraries(0201_single_triangle
	PRIVATE
	glfw
	glad
	opengl32
)

add_executable(0202_two_triangles_singleVBO
	"weekly/02_triangle/02_two_triangles_singleVBO.cpp"
)

target_link_libraries(0202_two_triangles_singleVBO
	PRIVATE
	glfw
	glad
	opengl32
)

add_executable(0203_two_triangles_multipleVBOs
	"weekly/02_triangle/03_two_triangles_multipleVBOs.cpp"
)

target_link_libraries(0203_two_triangles_multipleVBOs
	PRIVATE
	glfw
	glad
	opengl32
)

add_executable(0204_EBO_rectangle_01
	"weekly/02_triangle/04_EBO_rectangle_01.cpp"
)

target_link_libraries(0204_EBO_rectangle_01
	PRIVATE
	glfw
	glad
	opengl32
)