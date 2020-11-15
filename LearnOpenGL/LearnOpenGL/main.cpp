#include <iostream>
//GLEW
#define GLEW_STATIC
#include <GL/glew.h>

//GLFW
#include <GLFW/glfw3.h>

//Other libs
#include <SOIL/SOIL.h>
//GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Other includes
#include "App/Shader.h"
#include "App/Camera.h"

void mouse_callback(GLFWwindow* pWindow, double dPosX, double dPosY);
void key_callback(GLFWwindow* pWindow, int nKey, int nScanCode, int nAction, int nMode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void do_camera_movement();

const GLuint WIDTH = 800, HEIGHT = 600;

Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat g_LastX = WIDTH / 2.0f;
GLfloat g_LastY = HEIGHT / 2.0f;
bool g_sKey[1024];

GLfloat g_fDeltaTime = 0.0f;
GLfloat g_fLastFrame = 0.0f;

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main()
{
	if (GLFW_TRUE != glfwInit())
	{
		std::cout << "Failed to initialize GLFW" << std::endl;
		glfwTerminate();
		return -1;
	}
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* pWindow = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
	if (pWindow == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(pWindow);

	glfwSetKeyCallback(pWindow, key_callback);
	glfwSetCursorPosCallback(pWindow, mouse_callback);
	glfwSetScrollCallback(pWindow, scroll_callback);

	glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glewExperimental = GL_TRUE;
	GLenum nGlewRetCode = glewInit();
	if (GLEW_OK != nGlewRetCode)
	{
		std::cout << "Failed to initialize GLEW, Error: " << glewGetErrorString(nGlewRetCode) << std::endl;
		glfwTerminate();
		return -1;
	}

	int nScreenWidth = 0;
	int nScreenHeight = 0;
	glfwGetFramebufferSize(pWindow, &nScreenWidth, &nScreenHeight);
	glViewport(0, 0, nScreenWidth, nScreenHeight);
	glEnable(GL_DEPTH_TEST);

	//Shader
	// Build and compile our shader program
	Shader lightShader("lighting.vs", "lighting.frag");
	Shader lampShader("lamp.vs", "lamp.frag");

	if (lampShader.IsFailed())
	{
		std::cout << "Failed to Use Shader File, Error:" << lampShader.GetErrorLog() << std::endl;
		glfwTerminate();
		return -1;
	}
	// Set up vertex data 
	GLfloat vertices[] = {
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	GLuint indices[] = { 
		0, 1, 3, 
		1, 2, 3
	};

	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//desc layout
	//position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//diffuse attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)
	
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(pWindow))
	{
		GLfloat fCurrentFrame = glfwGetTime();
		g_fDeltaTime = fCurrentFrame - g_fLastFrame;
		g_fLastFrame = fCurrentFrame;

		//Event
		glfwPollEvents();
		do_camera_movement();

		//Clear
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Render
		lightShader.Use();

		GLint lightPosLoc = glGetUniformLocation(lightShader.GetProgram(), "light.position");
		GLint viewPosLoc = glGetUniformLocation(lightShader.GetProgram(), "viewPos");
		glm::vec3 cameraPos = camera.GetPosition();
		glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(viewPosLoc, cameraPos.x, cameraPos.y, cameraPos.z);

		// Set lights properties
		glm::vec3 lightColor;
		lightColor.x = sin(glfwGetTime() * 2.0f);
		lightColor.y = sin(glfwGetTime() * 0.7f);
		lightColor.z = sin(glfwGetTime() * 1.3f);
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // Decrease the influence
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // Low influence
		glUniform3f(glGetUniformLocation(lightShader.GetProgram(), "light.ambient"), ambientColor.x, ambientColor.y, ambientColor.z);
		glUniform3f(glGetUniformLocation(lightShader.GetProgram(), "light.diffuse"), diffuseColor.x, diffuseColor.y, diffuseColor.z);
		glUniform3f(glGetUniformLocation(lightShader.GetProgram(), "light.specular"), 1.0f, 1.0f, 1.0f);
		
		// Set material properties
		glUniform3f(glGetUniformLocation(lightShader.GetProgram(), "material.ambient"), 1.0f, 0.5f, 0.31f);
		glUniform3f(glGetUniformLocation(lightShader.GetProgram(), "material.diffuse"), 1.0f, 0.5f, 0.31f);
		glUniform3f(glGetUniformLocation(lightShader.GetProgram(), "material.specular"), 0.5f, 0.5f, 0.5f); // Specular doesn't have full effect on this object's material
		glUniform1f(glGetUniformLocation(lightShader.GetProgram(), "material.shininess"), 32.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(camera.Zoom, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightShader.GetProgram(), "model");
		GLint viewLoc = glGetUniformLocation(lightShader.GetProgram(), "view");
		GLint projLoc = glGetUniformLocation(lightShader.GetProgram(), "projection");
		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(VAO);
		glm::mat4 model;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		lampShader.Use();

		// // Bind Textures using texture units
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture1);
		//glUniform1i(glGetUniformLocation(lampShader.GetProgram(), "ourTexture1"), 0);

		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, texture2);
		//glUniform1i(glGetUniformLocation(lampShader.GetProgram(), "ourTexture2"), 1);

		modelLoc = glGetUniformLocation(lampShader.GetProgram(), "model");
		viewLoc = glGetUniformLocation(lampShader.GetProgram(), "view");
		projLoc = glGetUniformLocation(lampShader.GetProgram(), "projection");

		model = glm::mat4();
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		//Draw
		//glBindVertexArray(VAO);
		////glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glBindVertexArray(0);
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		//Swap
		glfwSwapBuffers(pWindow);
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glfwTerminate();

	return 0;
}


void key_callback(GLFWwindow* pWindow, int nKey, int nScanCode, int nAction, int nMode)
{
	GLfloat fCameraSpeed = 0.05f;
	if (nKey == GLFW_KEY_ESCAPE && nAction == GLFW_PRESS)
		glfwSetWindowShouldClose(pWindow, GL_TRUE);
	if (nKey >= 0 && nKey < 1024)
	{
		if (nAction == GLFW_PRESS)
			g_sKey[nKey] = true;
		else if (nAction == GLFW_RELEASE)
			g_sKey[nKey] = false;
	}
}

void do_camera_movement()
{
	// Camera controls
	GLfloat cameraSpeed = 5.0f * g_fDeltaTime;
	if (g_sKey[GLFW_KEY_W])
		camera.ProcessKeyboard(FORWARD, g_fDeltaTime);
	if (g_sKey[GLFW_KEY_S])
		camera.ProcessKeyboard(BACKWARD, g_fDeltaTime);
	if (g_sKey[GLFW_KEY_A])
		camera.ProcessKeyboard(LEFT, g_fDeltaTime);
	if (g_sKey[GLFW_KEY_D])
		camera.ProcessKeyboard(RIGHT, g_fDeltaTime);
}

void mouse_callback(GLFWwindow* pWindow, double dPosX, double dPosY)
{
	static bool sbFirstMouse = true;
	if (sbFirstMouse)
	{
		g_LastX = dPosX;
		g_LastY = dPosY;
		sbFirstMouse = false;
	}
	GLfloat fOffsetX = dPosX - g_LastX;
	GLfloat fOffsetY = g_LastY - dPosY;
	g_LastX = dPosX;
	g_LastY = dPosY;

	camera.ProcessMouseMovement(fOffsetX, fOffsetY);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}