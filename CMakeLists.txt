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

	target_include_directories(${TARGET_NAME}
		PRIVATE
		${CMAKE_SOURCE_DIR}/include
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
	scratch
	scratch/scratch1.cpp
)

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

add_demo(
	0301_shaders_start 
	03_shaders/01_shaders_start.cpp
)

add_demo(
	0302_vertex_color_attribute 
	03_shaders/02_vertex_color_attribute.cpp
)

add_demo(
	0303_vertex_color_with_uniform_time
	03_shaders/03_vertex_color_attribute_uniformTime.cpp
)

add_demo(
	0304_vertex_shader_translate_01
	03_shaders/04_vertex_shader_translate_01.cpp
)

add_demo(
	0305_shader_class_test
	03_shaders/05_shader_class_test.cpp
)

add_demo(
	0306_vert_pos_to_color
	03_shaders/06_vert_pos_to_color.cpp
)

add_demo(
	0307_pulsing_bouncing_example
	03_shaders/07_pulsing_bouncing_example.cpp
)

add_demo(
	0401_transform_matrix_01
	04_transformations/01_transform_matrix_01.cpp
)