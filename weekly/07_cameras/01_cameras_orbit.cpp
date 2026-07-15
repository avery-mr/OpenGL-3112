#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Shader.h>
#include <filesystem>
#include <stb_image.h>

//glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// callback for resizing viewer window
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// ESC input control
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
int main()
{
	std::cout << std::filesystem::current_path() << std::endl;

	glfwInit(); // initialize GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // configure GLFW (openGL version 4.6 in this case)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create window object
	// this holds all windowing data - required by most of GLFW's other functions
	// requires width and height as first 2 args, then optional window name.  
	// returns a GLFWwindow object

	GLFWwindow* window =
		glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "0701 - Camera Orbit", nullptr, nullptr);

	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// register callback function for window resize
	// other callbacks possible for e.g. joystic input, process error messages, etc
	// register callbacks after creating the window and before initiating render loop.
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// GLAD - manages function pointers for OpenGL
	// need to initialize before calling any OpenGL funciton
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	std::cout << "OpenGL Version: "
		<< glGetString(GL_VERSION)
		<< std::endl;

	// --- BLENDING ------
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST); // enable depth testing for 3D rendering)


	// create shader program from class
	Shader ourShader("../../../assets/shaders/utility/view_xform_texture.vs",
		"../../../assets/shaders/utility/texture_simple.fs");

	// ======= VERTEX DATA AND BUFFERS ======
	// vertex input data
	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f
	};

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	unsigned int VBO, VAO;
	glGenBuffers(1, &VBO);

	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(vertices),
		vertices,
		GL_STATIC_DRAW
	);


	// link vertex attributes (which part of input data goes to which vertex attribute)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// ----- TEXTURES ---------

	// gen texture id
	stbi_set_flip_vertically_on_load(true);
	unsigned int texture;
	glGenTextures(1, &texture);  // takes how many textures we want to generate, stores them in an int array
	glBindTexture(GL_TEXTURE_2D, texture); // bind the texture to the GL_TEXTURE_2D target

	// set the texture wrapping parameters and filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // repeat in x direction
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // repeat in y direction
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // minification filter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // magnification filter

	// load IMAGE with stbi and generate texture
	int width, height, nrChannels;
	unsigned char* data = stbi_load("../../../assets/textures/colorfulTiles.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		// - TEXTURE TARGET: GL_TEXTURE_2D (2D texture)
		// - MIPMAP LEVEL: 0 (base level)
		// - INTERNAL FORMAT: GL_RGB (how OpenGL should store the texture)
		// - WIDTH: width of the texture
		// - HEIGHT: height of the texture
		// - BORDER: 0 (must be 0)
		// - FORMAT: GL_RGB (format of the pixel data)
		// - TYPE: GL_UNSIGNED_BYTE (data type of the pixel data)
		// - DATA: pointer to the image data

		glGenerateMipmap(GL_TEXTURE_2D); // AUTOMATICALLY GENERATE ALL REQUIRED MIPMAPS
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	// free image data after generating texture
	stbi_image_free(data);

	ourShader.use(); // activate shader before setting uniforms
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	ourShader.setMat4("projection", projection);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		processInput(window); // looking for specific ESC key presses

		// rendering commands here
		//
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindTexture(GL_TEXTURE_2D, texture); // bind the texture to the GL_TEXTURE_2D target

		ourShader.use();

		// create transformations
		glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
		// right axis is the cross product of the up vector and the camera direction
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));

		glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
		
		const float radius = 10.0f;
		float camX = sin(glfwGetTime()) * radius;
		float camz = cos(glfwGetTime()) * radius;

		glm::mat4 view = glm::lookAt(glm::vec3(camX, 0.0f, camz), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		ourShader.setMat4("view", view);

		glBindVertexArray(VAO);

		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 10.0f * (i + 1);
			model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			ourShader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// DE-ALLOCATION OF MEMORY
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);




	// properly clean/delete GLFW's resources
	glfwTerminate();
	std::cout << "Goodbye!" << std::endl;
	return 0;

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}