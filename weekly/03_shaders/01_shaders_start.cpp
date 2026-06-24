#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


// callback for resizing viewer window
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// ESC input control
void processInput(GLFWwindow* window);

// VERTEX SHADER!!!
// this is the most simple possible vertex shader, it is just taking the input
// data and forwarding it to the shader's output.
// In real applications the inpput data is usually not already normalized.

const char* vertexShaderSource = "#version 460 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.2f, 0.9f, 0.6f, 1.0f);\n"
"}\n\0";

const char* frag_uniform_example_01 = "#version 460 core\n"
"out vec4 FragColor;\n"
"uniform vec4 ourColor;\n"
"void main()\n"
"{\n"
"   FragColor = ourColor;\n"
"}\n\0";

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
int main()
{
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

	// ===== SHADER OBJECTS ======
	// 
	// create shader object with type vertex shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// attach shader source code to shader object and compile shader
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);

	// CHECK IF VERTEX SHADER IS COMPILED CORRECTLY
	int vert_success;
	char vert_infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vert_success);

	if (!vert_success)
	{
		glGetShaderInfoLog(vertexShader, 512, nullptr, vert_infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILE\n" << vert_infoLog << std::endl;
	}

	// FRAGMENT SHADER
	// calculates color output of pixels
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &frag_uniform_example_01, nullptr);
	glCompileShader(fragmentShader);

	// CHECK IF FRAGMENT SHADER IS COMPILE CORRECTLY
	int frag_success;
	char frag_infoLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &frag_success);

	if (!frag_success)
	{
		glGetShaderInfoLog(fragmentShader, 512, nullptr, frag_infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION FAILED\n" << frag_infoLog << std::endl;
	}

	// ----- SHADER PROGRAM ------
	// -final linked version of multiple shaders combined
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	// now attach previously compiled shaders and link them
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	int shaderProgram_success;
	char shaderProgram_infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &shaderProgram_success);
	if (!shaderProgram_success)
	{
		glGetProgramInfoLog(shaderProgram, 512, nullptr, shaderProgram_infoLog);
		std::cout << "ERROR::SHADER PROGRAM FAILED\n" << shaderProgram_infoLog << std::endl;
	}

	glUseProgram(shaderProgram);

	// dont need these anymore
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	// ======= VERTEX DATA AND BUFFERS ======
	// vertex input data
	float bigTriangle[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
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
		sizeof(bigTriangle),
		bigTriangle,
		GL_STATIC_DRAW
	); // now the GPU has its own copy of the verts

	// link vertex attributes (which part of input data goes to which vertex attribute)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	



	// while loop keeps the window open, swapping back and forth between buffers
	// continues until explicitely told to stop. without this a single frame would flass and then quit and close.
	// glfwWindowShouldClose checks at the start of each loop for instructions to close
	// glfwPollEvents checks if events are triggerd (e.g. keyboard input)
	while(!glfwWindowShouldClose(window))
	{
		processInput(window); // looking for specific ESC key presses

		// rendering commands here
		//
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		// ===========================================

		double timeValue = glfwGetTime();
		float redValue = abs(sin(timeValue));
		float greenValue = abs(cos(timeValue));

		// query the location of the uniform variable in the shader program
		// note that this can be done before the rendering loop ?
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");

		glUseProgram(shaderProgram);

		// set uniform value - updating a uniform requires you call glUseProgram first
		glUniform4f(vertexColorLocation, redValue, greenValue, 0.0f, 1.0f);

		// ===========================================
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
	glDeleteProgram(shaderProgram);


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