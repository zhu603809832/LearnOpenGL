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

bool g_sKey[1024];
glm::vec3 g_CameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 g_CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 g_CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

GLfloat g_fDeltaTime = 0.0f;
GLfloat g_fLastFrame = 0.0f;

void mouse_callback(GLFWwindow* pWindow, double dPosX, double dPosY)
{

}

void key_callback(GLFWwindow* pWindow, int nKey, int nScanCode, int nAction, int nMode)
{
	GLfloat fCameraSpeed = 0.05;
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
		g_CameraPos += cameraSpeed * g_CameraFront;
	if (g_sKey[GLFW_KEY_S])
		g_CameraPos -= cameraSpeed * g_CameraFront;
	if (g_sKey[GLFW_KEY_A])
		g_CameraPos -= glm::normalize(glm::cross(g_CameraFront, g_CameraUp)) * cameraSpeed;
	if (g_sKey[GLFW_KEY_D])
		g_CameraPos += glm::normalize(glm::cross(g_CameraFront, g_CameraUp)) * cameraSpeed;
}

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

	GLFWwindow* pWindow = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
	if (pWindow == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(pWindow);
	glfwSetKeyCallback(pWindow, key_callback);
	glfwSetCursorPosCallback(pWindow, mouse_callback);

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
	const GLchar* vertexShaderFile = "vertex.glsl";
	const GLchar* fragShaderFile = "frag.glsl";
	Shader ourShader(vertexShaderFile, fragShaderFile);
	if (ourShader.IsFailed())
	{
		std::cout << "Failed to Use Shader File, Error:" << ourShader.GetErrorLog() << std::endl;
		glfwTerminate();
		return -1;
	}
	// Set up vertex data 
	GLfloat vertices[] = {
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

	GLuint indices[] = { 
		0, 1, 3, 
		1, 2, 3
	};

	glm::vec3 cubePosition[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
	    glm::vec3(2.0f,  5.0f, -15.0f),
	    glm::vec3(-1.5f, -2.2f, -2.5f),
	    glm::vec3(-3.8f, -2.0f, -12.3f),
	    glm::vec3(2.4f, -0.4f, -3.5f),
	    glm::vec3(-1.7f,  3.0f, -7.5f),
	    glm::vec3(1.3f, -2.0f, -2.5f),
	    glm::vec3(1.5f,  2.0f, -2.5f),
	    glm::vec3(1.5f,  0.2f, -1.5f),
	    glm::vec3(-1.3f,  1.0f, -1.5f)
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Color attribute
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	//glEnableVertexAttribArray(1);

	//TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)
	
	GLuint texture1;
	GLuint texture2;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set the tex wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texure filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int nImageWidth = 0;
	int nImageHeight = 0;
	unsigned char* image = SOIL_load_image("wood.jpg", &nImageWidth, &nImageHeight, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, nImageWidth, nImageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load, create texture and generate mipmaps
	image = SOIL_load_image("awesomeface.png", &nImageWidth, &nImageHeight, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, nImageWidth, nImageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	while (!glfwWindowShouldClose(pWindow))
	{
		//Event
		glfwPollEvents();
		do_camera_movement();
		GLfloat fCurrentFrame = glfwGetTime();
		g_fDeltaTime = fCurrentFrame - g_fLastFrame;
		g_fLastFrame = fCurrentFrame;
		//Clear
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Render
		ourShader.Use();

		 // Bind Textures using texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(glGetUniformLocation(ourShader.GetProgram(), "ourTexture1"), 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(glGetUniformLocation(ourShader.GetProgram(), "ourTexture2"), 1);

		GLint modelLoc = glGetUniformLocation(ourShader.GetProgram(), "model");
		GLint viewLoc = glGetUniformLocation(ourShader.GetProgram(), "view");
		GLint projectionLoc = glGetUniformLocation(ourShader.GetProgram(), "projection");
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;
		//model = glm::rotate(model, -55.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, (GLfloat)glfwGetTime() * 50.0f / 50, glm::vec3(0.5f, 1.0f, 0.0f));
		view = glm::lookAt(g_CameraPos, g_CameraPos + g_CameraFront, g_CameraUp);
		projection = glm::perspective(45.0f, (GLfloat)nScreenWidth / (GLfloat)nScreenHeight, 0.1f, 100.0f);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		//Draw
		//glBindVertexArray(VAO);
		////glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glBindVertexArray(0);

		glBindVertexArray(VAO);
		for (GLuint i = 0; i < 10; i++)
		{
			glm::mat4 model;
			model = glm::translate(model, cubePosition[i]);
			GLfloat angle = 20.0f * i;
			model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
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
