
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

const char* fragmentShaderSource1 = 
	"#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"    FragColor = vec4(0.2f, 0.9f, 0.7f, 1.0f);\n"
	"}\0";

const char* fragmentShaderSource2 =
	"#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"    FragColor = vec4(0.6f, 0.7f, 0.4f, 1.0f);\n"
	"}\0";


unsigned int vertexShader;

unsigned int fragmentShader1;
unsigned int fragmentShader2;

unsigned int shaderProgram1;
unsigned int shaderProgram2;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
unsigned int compileVertexShader();
unsigned int compileFragmentShader();
unsigned int compileFragmentShader2();
unsigned int createAndLinkShaderProgram();
unsigned int createAndLinkShaderProgram2();
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
	fragmentShader1 = compileFragmentShader();
	fragmentShader2 = compileFragmentShader2();


	// link to shader program
	shaderProgram1 = createAndLinkShaderProgram();
	shaderProgram2 = createAndLinkShaderProgram2();


	// now we don't need our shaders anymore
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader1);

	//---------------------GETTING VERTEX DATA TO THE GPU--------------------------//
	// define a set of vertices in the scene
	float vertices1[] = {
		 0.5f, 0.5f, 0.0f,
		 0.5f,-0.5f, 0.0f,
		-0.5f,-0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f
	};

	// define order of indices for triangles
	unsigned int indices1[]{
		0,1,2,
		0,2,3
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

	// specify the attribute as slot 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	// optional: unbind the VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	// ATTENTION: YOU MUST NOT UNBIND THE EBO!
	// NO! glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); NO!

	// finally unbind the vertex array
	// all scene data is now stored in "VAO"
	// this is also optional
	glBindVertexArray(0);




	// this is where we can define further VAOs for drawing a different scene
	// ....
	float vertices2[]{
		 0.0f, 0.6f, 0.2f,
		 0.0f,-0.6f, 0.2f,
		-0.5f,-0.6f,-0.2f,
		-0.5f, 0.6f,-0.2f
	};

	unsigned int indices2[]{
		0,1,2,
		0,2,3
	};

	unsigned int VAO2, VBO2, EBO2;
	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &EBO2);

	glBindVertexArray(VAO2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2), indices2, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);
	

	//--------------------------------------RENDER LOOP----------------------------------------//
	// keep window open until signal to close
	while (!glfwWindowShouldClose(window))
	{
		//-----------------------------USER INPUTS------------------------------//
		processInput(window);

		//-----------------------------RENDERING--------------------------------//
		glClearColor(0.6f, 0.1f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		glUseProgram(shaderProgram1);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBindVertexArray(VAO1);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glUseProgram(shaderProgram2);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		glBindVertexArray(0);

		// swap buffers and poll I/O events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO1);
	glDeleteBuffers(1, &VBO1);
	glDeleteBuffers(1, &EBO1);
	glDeleteProgram(shaderProgram1);

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
	glShaderSource(fragmentShader, 1, &fragmentShaderSource1, NULL);
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

unsigned int compileFragmentShader2() {
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource2, NULL);
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
	shaderProgram1 = glCreateProgram();

	glAttachShader(shaderProgram1, vertexShader);
	glAttachShader(shaderProgram1, fragmentShader1);
	glLinkProgram(shaderProgram1);

	// check for successful linking
	int success;
	char infoLog[LOG_BUF_SIZE];
	glGetProgramiv(shaderProgram1, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram1, LOG_BUF_SIZE, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	return shaderProgram1;
}

unsigned int createAndLinkShaderProgram2() {
	shaderProgram2 = glCreateProgram();

	glAttachShader(shaderProgram2, vertexShader);
	glAttachShader(shaderProgram2, fragmentShader2);
	glLinkProgram(shaderProgram2);

	// check for successful linking
	int success;
	char infoLog[LOG_BUF_SIZE];
	glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram1, LOG_BUF_SIZE, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	return shaderProgram2;
}

