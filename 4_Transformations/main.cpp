#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "Shader.h"
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

int window_width = 800;
int window_height = 800;

float fov = 50.0f;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float pitch = 0.0f;
float yaw = -90.0f;

float lastX = 400;
float lastY = 300;

bool firstMouse = true;

const int NUM_CUBES = 10;

int main() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow* window = glfwCreateWindow(window_width, window_height, "Tutorial 2", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// input mode: this CAPTURES the mouse 
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glClearColor(0.05f, 0.1f, 0.1f, 1.0f);

	glViewport(0, 0, window_width, window_height);

	Shader shaderProgram1(
		"C:/Users/freib/Documents/OpenGL/LearnOpenGL/4_Transformations/vertex.vert",
		"C:/Users/freib/Documents/OpenGL/LearnOpenGL/4_Transformations/fragment.frag");


	//---------------------GETTING VERTEX DATA TO THE GPU--------------------------//
	float vertices1[] = {
		// positions		 // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	// Define the positions in world space
	glm::vec3 cubePositions[NUM_CUBES] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -10.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	glm::vec3 cubeRotations[NUM_CUBES];

	for (int i = 0; i < 10; i++) {
		for (int dir = 0; dir < 3; dir++) {
			float r = static_cast<float>(rand() / static_cast<float>(RAND_MAX)) * 20 + 10;
			float mult = rand() % 2 ? 1.0 : -1.0;

			cubeRotations[i][dir] = r * mult;
		}
	}


	unsigned int texture1, texture2;
	glGenTextures(1, &texture1);
	glGenTextures(1, &texture2);


	//-------------------------------------TEXTURE 1----------------------------------------//
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(
		"C:/Users/freib/Documents/OpenGL/LearnOpenGL/3_Textures/resources/container.jpg",
		&width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);


	//-------------------------------------TEXTURE 2----------------------------------------//
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	data = stbi_load(
		"C:/Users/freib/Documents/OpenGL/LearnOpenGL/3_Textures/resources/awesomeface.png",
		&width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	//------------------------------------TEXTURES END-------------------------------------//

	unsigned int VAO1;
	glGenVertexArrays(1, &VAO1);
	glBindVertexArray(VAO1);

	unsigned int VBO1;
	glGenBuffers(1, &VBO1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);

	unsigned int EBO1;
	glGenBuffers(1, &EBO1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);



	// set textures
	shaderProgram1.use();
	shaderProgram1.setInt("wood", 0);
	shaderProgram1.setInt("face", 1);


	// Enable depth testing!
	glEnable(GL_DEPTH_TEST);


	//--------------------------------------RENDER LOOP----------------------------------------//
	// keep window open until signal to close
	while (!glfwWindowShouldClose(window))
	{
		//-----------------------------USER INPUTS------------------------------//
		processInput(window);


		//-------------------------SHADER PREPROCESSING-------------------------//
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// MODEL MATRIX
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(lastFrame * 31), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(lastFrame * 27), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(lastFrame * 24), glm::vec3(0.0f, 0.0f, 1.0f));


		// VIEW MATRIX / CAMERA POSITION
		glm::mat4 view;
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);


		// PROJECTION MATRIX
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(fov), (float)window_width / (float)window_height, 0.1f, 100.0f);

		unsigned int modelLoc = glGetUniformLocation(shaderProgram1.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		unsigned int viewLoc = glGetUniformLocation(shaderProgram1.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		unsigned int projectionLoc = glGetUniformLocation(shaderProgram1.ID, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		//------------------------------RENDERING-------------------------------//
		glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);


		shaderProgram1.use();
		glBindVertexArray(VAO1);
		

		for (int i = 0; i < 10; i++) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);

			model = glm::rotate(model, glm::radians(lastFrame * cubeRotations[i].x), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(lastFrame * cubeRotations[i].y), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(lastFrame * cubeRotations[i].z), glm::vec3(0.0f, 0.0f, 1.0f));

			unsigned int modelLoc = glGetUniformLocation(shaderProgram1.ID, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		
		
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO1);
	glDeleteBuffers(1, &VBO1);
	glDeleteBuffers(1, &EBO1);

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	window_width = width;
	window_height = height;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse) {
		lastX = (float) xpos;
		lastY = (float) ypos;
		firstMouse = false;
	}
	
	float xoffset = (float) xpos - lastX;
	float yoffset = lastY - (float) ypos;

	lastX = (float) xpos;
	lastY = (float) ypos;

	const float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	cameraFront = glm::normalize(direction);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	fov -= (float)yoffset;

	fov = glm::clamp(fov, 1.0f, 60.0f);
}

void processInput(GLFWwindow* window) {
	float cameraSpeed = 3.0f * deltaTime;

	glm::vec3 projectedForward = glm::normalize(glm::vec3(cameraFront.x, 0.0f, cameraFront.z));
	glm::vec3 right = glm::cross(cameraFront, cameraUp);
	glm::vec3 projectedRight = glm::normalize(glm::vec3(right.x, 0.0f, right.z));

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cameraPos += projectedForward * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cameraPos -= projectedForward * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cameraPos -= projectedRight * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cameraPos += projectedRight * cameraSpeed;
	}
}