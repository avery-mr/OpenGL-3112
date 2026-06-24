#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Shader.h>
#include <filesystem>


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
		glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "03 - SHADERS", nullptr, nullptr);

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

	// create shader program from class
	Shader ourShader("../../../weekly/03_shaders/shaders/shader_verticalBounce.vs", 
					 "../../../weekly/03_shaders/shaders/shader_pulsingRed.fs");

	// ======= VERTEX DATA AND BUFFERS ======
	// vertex input data
	float vertices[] = {
		// positions         // colors
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom left
		 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom right
		 0.0f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f  // top
	};

	// create Vertex Buffer Object
	// used to copy vertex data into GPU memory
	unsigned int VBO;
	glGenBuffers(1, &VBO); // openGL creates an ID for a buffer
	// Bind the VBO
	// makes the VBO the current buffer for vertex data ops
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Vertex array object 
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// upload data - copies data from CPU to GPU mem
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(vertices),
		vertices,
		GL_STATIC_DRAW
	); // now the GPU has its own copy of the verts

	// link vertex attributes (which part of input data goes to which vertex attribute)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);



	// while loop keeps the window open, swapping back and forth between buffers
	// continues until explicitely told to stop. without this a single frame would flass and then quit and close.
	// glfwWindowShouldClose checks at the start of each loop for instructions to close
	// glfwPollEvents checks if events are triggerd (e.g. keyboard input)
	while (!glfwWindowShouldClose(window))
	{
		processInput(window); // looking for specific ESC key presses

		// rendering commands here
		//
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ourShader.use();

		float t = (float)glfwGetTime();

		ourShader.setFloat("uTime", t);
		

		//draw triangle with data in VAO
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		// check and call events and swap the buffers
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