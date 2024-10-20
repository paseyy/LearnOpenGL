#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Shader.h"
#include "Camera.h"
#include "Cube.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 1000;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// light
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
glm::vec3 lightColor(1.0f, 1.0f, 2.0f);


int main() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Tutorial 2", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	
	glClearColor(0.05f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);


	// define shader program
	Shader cubeShader(
		"C:/Users/freib/Documents/OpenGL/LearnOpenGL/5_Colors/cube.vert",
		"C:/Users/freib/Documents/OpenGL/LearnOpenGL/5_Colors/cube.frag"
	);

	Shader lightShader(
		"C:/Users/freib/Documents/OpenGL/LearnOpenGL/5_Colors/light.vert",
		"C:/Users/freib/Documents/OpenGL/LearnOpenGL/5_Colors/light.frag"
	);

	//-----------------------------CUBE object VAO---------------------------//
	unsigned int cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glBindVertexArray(cubeVAO);

	unsigned int cubeVBO;
	glGenBuffers(1, &cubeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);

	glBufferData(GL_ARRAY_BUFFER, Cube::size(), Cube::vertices, GL_STATIC_DRAW);
	// position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);
	// normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//------------------------------LIGHT source VAO--------------------------//
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	
	// reuse the cube VBO so that we have a cube-shaped light source
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);

	// position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	//------------------------------SHADER CONSTANTS--------------------------//
	cubeShader.use();
	cubeShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
	cubeShader.setVec3("lightColor", lightColor);

	lightShader.use();
	lightShader.setVec3("lightColor", lightColor);

	//--------------------------------------RENDER LOOP----------------------------------------//
	// keep window open until signal to close
	while (!glfwWindowShouldClose(window))
	{
		//---------------------------PER-FRAME TIME-----------------------------//
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//-----------------------------USER INPUTS------------------------------//
		processInput(window);

		//-----------------------PROJECTION & VIEW MATRIX-----------------------//
		// pass projection matrix to shader (note that in this case it could change every frame)
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		
        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        

		//-----------------------CUBE SHADER PREPROCESSING----------------------//
		cubeShader.use();
        cubeShader.setMat4("projection", projection);
        cubeShader.setMat4("view", view);

		// model transformation
		glm::mat4 cubeModel = glm::identity<glm::mat4>();
		cubeShader.setMat4("model", cubeModel);

		// set view position in fragment shader
		cubeShader.setVec3("viewPos", camera.Position);
		
		// set the moving light position
		glm::vec3 movingLightPos = glm::vec3(sin(currentFrame/2), cos(currentFrame/3), sin(currentFrame/2.7));
		cubeShader.setVec3("lightPos", movingLightPos);

		//-----------------------LIGHT SHADER PREPROCESSING---------------------//
		lightShader.use();
        lightShader.setMat4("projection", projection);
        lightShader.setMat4("view", view);
		
		// model transformation
		glm::mat4 lightModel = glm::mat4(1.0);
		lightModel = glm::translate(lightModel, movingLightPos);
		lightModel = glm::scale(lightModel, glm::vec3(0.2f));
		lightShader.setMat4("model", lightModel);
		
		//------------------------------RENDERING-------------------------------//
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		cubeShader.use();
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, Cube::size());

		lightShader.use();
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, Cube::size());


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &cubeVBO);

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
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
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	const float sensitivity = 0.3f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
