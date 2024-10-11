#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "shader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main() {
	// initialize glfw
	glfwInit();

	// specify OpenGL version as 3.3 CORE
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	// create a new window...
	GLFWwindow* window = glfwCreateWindow(800, 600, "Tutorial 2", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// ...and make it active
	glfwMakeContextCurrent(window);

	// register framebuffer_size_callback() to be called every time the window is resized
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// load function addresses using GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// set window background color 
	glClearColor(0.05f, 0.1f, 0.1f, 1.0f);

	// set the dimension of the screen space, can be equal or lower to window size
	glViewport(0, 0, 800, 600);

	// define shader program
	Shader shaderProgram1(
		"C:/Users/freib/Documents/OpenGL/LearnOpenGL/2_Shaders/2_Shaders/flip.vert",
		"C:/Users/freib/Documents/OpenGL/LearnOpenGL/2_Shaders/2_Shaders/interp.frag");


	//---------------------GETTING VERTEX DATA TO THE GPU--------------------------//
	// define a set of vertices in the scene
	float vertices1[] = {
		// positions		// colors
		 0.5f,-0.5f, 0.0f,	1.0f, 0.0f, 0.0f,
		-0.5f,-0.5f, 0.0f,	0.0f, 1.0f, 0.0f,
		 0.0f, 0.5f, 0.0f,	0.0f, 0.0f, 1.0f
	};

	// define order of indices for triangle
	unsigned int indices1[]{
		0,1,2
	};


	// declare a Vertex Array Object
	unsigned int VAO1;
	glGenVertexArrays(1, &VAO1);
	// Important that we do this before all further buffer manipulation!
	// all VBO, VEO, attrib calls from now on will be stored in VAO
	glBindVertexArray(VAO1);

	// then declare a Vertex Buffer Object and use it as the GL_ARRAY_BUFFER
	unsigned int VBO1;
	glGenBuffers(1, &VBO1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);

	// now the Element Buffer Object which is used to index the vertices for drawing
	unsigned int EBO1;
	glGenBuffers(1, &EBO1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);

	// send the vertex and index data to the GPU
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices1), indices1, GL_STATIC_DRAW);

	// specify the position attribute as position 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);
	// and the color attribute as position 1
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// optional: unbind the VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// ATTENTION: YOU MUST NOT UNBIND THE EBO!
	// NO! glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); NO!

	// finally unbind the vertex array
	// all scene data is now stored in "VAO"
	// this is also optional
	// glBindVertexArray(0);



	// this is where we can define further VAOs for drawing a different scene
	// ....



	//--------------------------------------RENDER LOOP----------------------------------------//
	// keep window open until signal to close
	while (!glfwWindowShouldClose(window))
	{
		//-----------------------------USER INPUTS------------------------------//
		processInput(window);

		
		//-------------------------SHADER PREPROCESSING-------------------------//
		float timeValue = glfwGetTime();
		float offsetX = sin(timeValue * 3.0f) / 2.0f;
		shaderProgram1.use();
		shaderProgram1.setFloat("offsetX", offsetX);

		//------------------------------RENDERING-------------------------------//
		glClear(GL_COLOR_BUFFER_BIT);


		shaderProgram1.use();
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBindVertexArray(VAO1);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);


		glBindVertexArray(0);

		// swap buffers and poll I/O events
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
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}