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

const char* fragmentShaderSrc1 = "#version 460 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.2f, 0.9f, 0.6f, 1.0f);\n"
"}\n\0";

const char* fragmentShaderSrc2 = "#version 460 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.9f, 0.6f, 0.2f, 1.0f);\n"
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
		glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "02_05 - MULTIPLE FRAGMENT SHADERS", nullptr, nullptr);

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
	// fragment shader 1
	unsigned int fragmentShader1;
	fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader1, 1, &fragmentShaderSrc1, nullptr);
	glCompileShader(fragmentShader1);

	// CHECK IF FRAGMENT SHADER IS COMPILE CORRECTLY
	int frag1_success;
	char frag1_infoLog[512];
	glGetShaderiv(fragmentShader1, GL_COMPILE_STATUS, &frag1_success);

	if (!frag1_success)
	{
		glGetShaderInfoLog(fragmentShader1, 512, nullptr, frag1_infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION FAILED\n" << frag1_infoLog << std::endl;
	}

	// fragment shader 2
	unsigned int fragmentShader2;
	fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader2, 1, &fragmentShaderSrc2, nullptr);
	glCompileShader(fragmentShader2);

	int frag2_success;
	char frag2_infoLog[512];
	glGetShaderiv(fragmentShader2, GL_COMPILE_STATUS, &frag2_success);

	if (!frag2_success)
	{
		glGetShaderInfoLog(fragmentShader2, 512, nullptr, frag2_infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION FAILED\n" << frag2_infoLog << std::endl;
	}




	// ----- SHADER PROGRAM ------
	// need 2 different shader programs for two colors
	unsigned int shaderProgram1, shaderProgram2;
	shaderProgram1 = glCreateProgram();
	shaderProgram2 = glCreateProgram();

	// attach shader1 
	glAttachShader(shaderProgram1, vertexShader);
	glAttachShader(shaderProgram1, fragmentShader1);
	glLinkProgram(shaderProgram1);

	// attach shader2 using different fragment shader but same vertex shader
	glAttachShader(shaderProgram2, vertexShader);
	glAttachShader(shaderProgram2, fragmentShader2);
	glLinkProgram(shaderProgram2);


	// dont need these anymore
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader1);
	glDeleteShader(fragmentShader2);


	// ======= VERTEX DATA AND BUFFERS ======
	// vertex input data
	float triangle1[] = {
		-0.9f, -0.5f, 0.0f,
		-0.1f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f
	};

	float triangle2[] = {
		0.9f, -0.5f, 0.0f,
		0.1f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.0f
	};

	// create multiple VBOs and VAOs at the same time
	unsigned int VBOs[2], VAOs[2];
	glGenBuffers(2, VBOs);
	glGenVertexArrays(2, VAOs);

	// triangle1 setup
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle1), triangle1, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//triangle2 setup
	glBindVertexArray(VAOs[1]); // different array and buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle2), triangle2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


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
		glClearColor(0.1f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw triangle1 with data from first VAO
		glUseProgram(shaderProgram1); // use shader1
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		// draw triangle2 with data from second VAO
		glUseProgram(shaderProgram2);
		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// DE-ALLOCATION OF MEMORY
	glDeleteVertexArrays(1, VAOs);
	glDeleteBuffers(1, VBOs);
	glDeleteProgram(shaderProgram1);
	glDeleteProgram(shaderProgram2);


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