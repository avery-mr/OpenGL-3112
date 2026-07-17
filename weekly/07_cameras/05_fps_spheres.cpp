#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Camera.h>
#include <Shader.h>
#include <filesystem>
#include <stb_image.h>
#include <vector>
#include <cmath>
#include <random>

//glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// Globals
std::random_device rd;
std::mt19937 rng(rd());
std::uniform_real_distribution<float> randomScale(0.1f, 0.9f);
std::uniform_real_distribution<float> randomPos(-6.0f, 6.0f);
std::uniform_real_distribution<float> randomSpin(0.2f, 5.0f);
std::uniform_real_distribution<float> randomAngle(0.0f, 1.0f);




// settings
const unsigned int numSpheres = 100;
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 12.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// use delta time to make camera movement smooth and framerate independent
float deltaTime = 0.0f;
float lastFrame = 0.0f;

struct SphereVertex
{
	glm::vec3 position;
	glm::vec3 color;
	glm::vec2 texCoords;
};

//Sphere
void createSphere(
	std::vector<SphereVertex>& vertices,
	std::vector<unsigned int>& indices,
	unsigned int horizontalSegments,
	unsigned int verticalSegments
)
{
	for (unsigned int y = 0; y <= verticalSegments; y++)
	{
		float v = static_cast<float>(y) / static_cast<float>(verticalSegments);

		float phi = v * glm::pi<float>();

		for (unsigned int x = 0; x <= horizontalSegments; x++)
		{
			float u = static_cast<float>(x) / static_cast<float>(horizontalSegments);

			float theta = u * glm::two_pi<float>();

			glm::vec3 position;

			position.x = std::sin(phi) * std::cos(theta);
			position.y = std::cos(phi);
			position.z = std::sin(phi) * std::sin(theta);

			SphereVertex vertex;

			vertex.position = position;
			vertex.color = glm::vec3(1.0f);
			vertex.texCoords = glm::vec2(u, v);

			vertices.push_back(vertex);

		}
	}

	for (unsigned int y = 0; y < verticalSegments; y++)
	{
		for (unsigned int x = 0; x < horizontalSegments; x++)
		{
			unsigned int topLeft = y * (horizontalSegments + 1) + x;

			unsigned int bottomLeft = (y + 1) * (horizontalSegments + 1) + x;

			unsigned int topRight = topLeft + 1;
			unsigned int bottomRight = bottomLeft + 1;

			indices.push_back(topLeft);
			indices.push_back(bottomLeft);
			indices.push_back(topRight);

			indices.push_back(topRight);
			indices.push_back(bottomLeft);
			indices.push_back(bottomRight);
		}
	}
}

int main()
{
	glfwInit(); // initialize GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // configure GLFW (openGL version 4.6 in this case)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window =
		glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "0701 - Camera Orbit", nullptr, nullptr);

	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
    // ---------------------------------------
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

	// ======= SPHERE DATA AND BUFFERS ====
	// vertex input data
	std::vector<SphereVertex> sphereVertices;
	std::vector<unsigned int> sphereIndices;

	createSphere(
		sphereVertices,
		sphereIndices,
		30,
		12
	);

	unsigned int sphereVAO, sphereVBO, sphereEBO;

	glGenVertexArrays(1, &sphereVAO);
	glGenBuffers(1, &sphereVBO);
	glGenBuffers(1, &sphereEBO);

	glBindVertexArray(sphereVAO);

	glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);

	glBufferData(
		GL_ARRAY_BUFFER,
		sphereVertices.size() * sizeof(SphereVertex),
		sphereVertices.data(),
		GL_STATIC_DRAW
	);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		sphereIndices.size() * sizeof(unsigned int),
		sphereIndices.data(),
		GL_STATIC_DRAW
	);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SphereVertex), (void*)offsetof(SphereVertex, position));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(SphereVertex), (void*)offsetof(SphereVertex, color));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(SphereVertex), (void*)offsetof(SphereVertex, texCoords));
	glEnableVertexAttribArray(2);


	std::vector<glm::vec3> spherePositions;
	std::vector<float> spinSpeeds;
	std::vector<glm::vec3> rotationAxes;
	std::vector<float> sphereScales;
	for (unsigned int i = 0; i < numSpheres; i++) {
		sphereScales.push_back(randomScale(rng));
		rotationAxes.push_back(glm::vec3(randomAngle(rng), randomAngle(rng), randomAngle(rng)));
		spherePositions.push_back(glm::vec3(randomPos(rng), randomPos(rng), randomPos(rng)));
		spinSpeeds.push_back(randomSpin(rng));

	}

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
		glGenerateMipmap(GL_TEXTURE_2D); // AUTOMATICALLY GENERATE ALL REQUIRED MIPMAPS
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	// free image data after generating texture
	stbi_image_free(data);

	
	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// calculate new delta time for this frame
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		processInput(window);

		// rendering commands here
		//
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// bind textures on corresponding texture units
		glBindTexture(GL_TEXTURE_2D, texture); 

		ourShader.use();

		// pass projection matrix to shader (note that in this case it could change every frame)
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		ourShader.setMat4("projection", projection);


		// camera/view transformation
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("view", view);


		// render spheres
		glBindVertexArray(sphereVAO);

		for (unsigned int i = 0; i < numSpheres; i++)
		{
			glm::mat4 sphereModel = glm::mat4(1.0f);


			sphereModel = glm::translate(sphereModel, spherePositions[i]);
			float angle = 30.0f * spinSpeeds[i];
			sphereModel = glm::rotate(sphereModel, (float)glfwGetTime() * glm::radians(angle), rotationAxes[i]);
			float scale = randomScale(rng);
			sphereModel = glm::scale(sphereModel, glm::vec3(sphereScales[i]));
			ourShader.setMat4("model", sphereModel);

			glDrawElements(
				GL_TRIANGLES,
				static_cast<GLsizei>(sphereIndices.size()),
				GL_UNSIGNED_INT,
				0
			);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// DE-ALLOCATION OF MEMORY
	glDeleteVertexArrays(1, &sphereVAO);
	glDeleteBuffers(1, &sphereVBO);
	glDeleteBuffers(1, &sphereEBO);




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

	const float cameraSpeed = 2.0f * deltaTime; // adjust accordingly
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coords go from bottom to top
	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

