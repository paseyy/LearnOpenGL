#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Shader.h"
#include "stb_image.h"


float faceAlpha = 0.5;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Tutorial 2", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glClearColor(0.05f, 0.1f, 0.1f, 1.0f);
	glViewport(0, 0, 800, 600);

	Shader shaderProgram1(
		"C:/Users/freib/Documents/OpenGL/LearnOpenGL/3_Textures/vertex.vert",
		"C:/Users/freib/Documents/OpenGL/LearnOpenGL/3_Textures/fragment.frag");

	//---------------------GETTING VERTEX DATA TO THE GPU--------------------------//
	float vertices1[] = {
		// positions		// colors			// texture coords
		 0.5f,-0.5f, 0.0f,	1.0f, 0.0f, 0.0f,	2.0f, 0.0f,
		-0.5f,-0.5f, 0.0f,	0.0f, 1.0f, 0.0f,	0.0f, 0.0f,
		 0.5f, 0.5f, 0.0f,	0.0f, 0.0f, 1.0f,	2.0f, 2.0f,
		-0.5f, 0.5f, 0.0f,	1.0f, 1.0f, 0.0f,	0.0f, 2.0f
	};

	unsigned int indices1[]{
		0,1,2,
		2,1,3
	};


	// generate texture ids
	unsigned int texture1, texture2;
	glGenTextures(1, &texture1);
	glGenTextures(1, &texture2);

	//-------------------------------------TEXTURE 1----------------------------------------//
	// first, set one of OpenGLs texture unit active,
	// then bind our texture to the active unit
	glBindTexture(GL_TEXTURE_2D, texture1);

	// set texture wrapping options: for axes S=x and T=y, mirrored repeat the texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// set filtering options for magnifying and minifying
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// load texture image from disk
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(
		"C:/Users/freib/Documents/OpenGL/LearnOpenGL/3_Textures/resources/container.jpg",
		&width, &height, &nrChannels, 0);
	if (data) {
		// set the texture data for the identifier
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		std::cout << "Failed to load texture" << std::endl;
	}
	// free the image data
	stbi_image_free(data);

	


	//-------------------------------------TEXTURE 2----------------------------------------//
	// do the same thing for the second texture...
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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices1), indices1, GL_STATIC_DRAW);

	// position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);
	// color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);



	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);


	//--------------------------------------RENDER LOOP----------------------------------------//
	while (!glfwWindowShouldClose(window))
	{
		//-----------------------------USER INPUTS------------------------------//
		processInput(window);

		//-------------------------SHADER PREPROCESSING-------------------------//
		//....
		shaderProgram1.use();
		shaderProgram1.setInt("tWood", 0);
		shaderProgram1.setInt("tFace", 1);
		shaderProgram1.setFloat("faceAlpha", faceAlpha);

		//------------------------------RENDERING-------------------------------//
		glClear(GL_COLOR_BUFFER_BIT);


		shaderProgram1.use();
		glBindVertexArray(VAO1);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		faceAlpha += 0.005f;

		if (faceAlpha > 1.0f)
			faceAlpha = 1.0f;
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		faceAlpha -= 0.005f;


		if (faceAlpha < 0.0f)
			faceAlpha = 0.0f;
	}

}