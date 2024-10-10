
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>


const int LOG_BUF_SIZE = 512;

const char* vertexShaderSource =
	"#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

const char* fragmentShaderSource = 
	"#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"    FragColor = vec4(0.2f, 0.9f, 0.7f, 1.0f);\n"
	"}\0";


unsigned int vertexShader;
unsigned int fragmentShader;

unsigned int shaderProgram;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
unsigned int compileVertexShader();
unsigned int compileFragmentShader();
unsigned int createAndLinkShaderProgram();
void renderScene();

int main() {
	// initialize glfw
	glfwInit();

	// specify OpenGL version as 3.3 CORE
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	
	// create a new window...
	GLFWwindow* window = glfwCreateWindow(800, 600, "Tutorial 1", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// ...and make it active
	glfwMakeContextCurrent(window);
	
	// load function addresses using GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// set the dimension of the screen space, can be equal or lower to window size
	glViewport(0, 0, 800, 600);

	// register framebuffer_size_callback() to be called every time the window is resized
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	// compile shaders
	vertexShader = compileVertexShader();
	fragmentShader = compileFragmentShader();


	// link to shader program
	shaderProgram = createAndLinkShaderProgram();

	// now we don't need our shaders anymore
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//---------------------GETTING VERTEX DATA TO THE GPU--------------------------//
	// define a set of vertices in the scene
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};

	// declare a Vertex Array Object
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO); // all VBO, attrib etc. calls from now on will be stored in VAO

	// then declare a Vertex Buffer Object and use it as the GL_ARRAY_BUFFER
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);


	// send the data to the GPU
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// specify the attribute as slot 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	// unbind the VBO and VAO, maybe define new VBO and VAOs
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);



	//--------------------------------------RENDER LOOP----------------------------------------//
	// keep window open until signal to close
	while (!glfwWindowShouldClose(window))
	{
		//-----------------------------USER INPUTS------------------------------//
		processInput(window);

		//-----------------------------RENDERING--------------------------------//
		glClearColor(0.6f, 0.1f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);


		// swap buffers and poll I/O events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

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

unsigned int compileVertexShader() {
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int success;
	char infoLog[LOG_BUF_SIZE];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(vertexShader, LOG_BUF_SIZE, NULL, infoLog);
		std::cout << "ERROR::VERTEX::SHADER::COMPILATION::FAILED\n" << infoLog << std::endl;
	}

	return vertexShader;
}

unsigned int compileFragmentShader() {
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	int success;
	char infoLog[LOG_BUF_SIZE];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(fragmentShader, LOG_BUF_SIZE, NULL, infoLog);
		std::cout << "ERROR::FRAGMENT::SHADER::COMPILATION::FAILED\n" << infoLog << std::endl;
	}

	return fragmentShader;
}

unsigned int createAndLinkShaderProgram() {
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// check for successful linking
	int success;
	char infoLog[LOG_BUF_SIZE];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, LOG_BUF_SIZE, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	return shaderProgram;
}
