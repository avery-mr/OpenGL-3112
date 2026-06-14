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

function(add_demo TARGET_NAME SOURCE_FILE)
	add_executable(${TARGET_NAME}
		weekly/${SOURCE_FILE}
	)

	target_link_libraries(${TARGET_NAME}
		PRIVATE
		glfw
		glad
		opengl32
	)
endfunction()

# -------- ADD DEMOS ---------------
add_demo(
	0101_Render_Window
	01_environment_setup/01_render_window.cpp
)

add_demo(
	0201_single_triangle
	02_triangle/01_single_triangle.cpp
)

add_demo(
	0202_two_triangles_singleVBO
	02_triangle/02_two_triangles_singleVBO.cpp
)

add_demo(
	0203_two_triangles_multipleVBOs
	02_triangle/03_two_triangles_multipleVBOs.cpp
)

add_demo(
	0204_EBO_rectangle_01
	02_triangle/04_EBO_rectangle_01.cpp
)

add_demo(
	0205_two_triangles_multiple_shaders
	02_triangle/05_two_triangles_multiple_shaders.cpp
)


