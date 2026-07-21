cmake_minimum_required(VERSION 3.20)

project(OpenGL_3112)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(external/glfw)

add_library(common
	src/stb_image.cpp
)

add_library(glad
	external/glad/src/glad.c
)

target_include_directories(common
	PUBLIC 
	${CMAKE_SOURCE_DIR}/include
	${CMAKE_SOURCE_DIR}/external/stb
)

target_include_directories(glad
	PUBLIC 
	external/glad/include 
)

target_link_libraries(common
	PUBLIC 
	glfw
	glad
	opengl32
)

function(add_demo TARGET_NAME SOURCE_FILE)
	add_executable(${TARGET_NAME}
		weekly/${SOURCE_FILE}
	)

	target_link_libraries(${TARGET_NAME}
		PRIVATE
		common
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
	0308_mouse_position_uniform
	03_shaders/08_mouse_position_uniform_test.cpp
)


add_demo(
	0401_transform_matrix_01
	04_transformations/01_transform_matrix_01.cpp
)

add_demo(
	0402_transform_matrix_02
	04_transformations/02_transform_matrix_02.cpp
)

add_demo(
	0403_transform_matrix_03
	04_transformations/03_transform_matrix_03.cpp
)

add_demo(
	0501_texture_start
	05_textures/01_texture_start.cpp
)

add_demo(
	0502_texture_vertColor_blend
	05_textures/02_texture_vertColor_blend.cpp
)

add_demo(
	0503_texture_vertColor_alpha
	05_textures/03_texture_vertColor_alpha.cpp
)

add_demo(
	0504_texture_lerp
	05_textures/04_texture_lerp.cpp
)

add_demo(
	0505_texture_lerp_userCtrl
	05_textures/05_texture_lerp_userCtrl.cpp
)

add_demo(
	0601_coordinate_systems_start
	06_coordinate_systems/01_coordSys_start.cpp
)

add_demo(
	0602_coordinate_systems_cube
	06_coordinate_systems/02_coordSys_cube.cpp
)

add_demo(
	0603_multiple_cubes
	06_coordinate_systems/03_coordSys_multipleCubes.cpp
)

add_demo(
	0701_cameras_orbit
	07_cameras/01_cameras_orbit.cpp
)

add_demo(
	0702_cameras_walkAround
	07_cameras/02_cameras_walkaround.cpp
)

add_demo(
	0703_cameras_lookaround
	07_cameras/03_cameras_lookaround.cpp
)

add_demo(
	0704_Camera_Class
	07_cameras/04_camera_class.cpp
)

add_demo(
	0705_FPS_spheres
	07_cameras/05_FPS_spheres.cpp
)

add_demo(
	0801_colors_start
	08_lighting/01_colors_start.cpp
)