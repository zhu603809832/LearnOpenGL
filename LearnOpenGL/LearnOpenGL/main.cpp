#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include "App/Shader.h"

const GLchar* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in vec3 color;\n"
"out vec3 vertexColor;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
"vertexColor = color;\n"
"}\0";

const GLchar* fragmentShaderSource = "#version 330 core\n"
"in vec3 vertexColor;\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(vertexColor, 1.0f);\n"
"}\n\0";

void key_callback(GLFWwindow* pWindow, int nKey, int nScanCode, int nAction, int nMode)
{
	if (nKey == GLFW_KEY_ESCAPE && nAction == GLFW_PRESS)
		glfwSetWindowShouldClose(pWindow, GL_TRUE);
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

	glewExperimental = GL_TRUE;
	GLenum nGlewRetCode = glewInit();
	if (GLEW_OK != nGlewRetCode)
	{
		std::cout << "Failed to initialize GLEW, Error: " << glewGetErrorString(nGlewRetCode) << std::endl;
		glfwTerminate();
		return -1;
	}

	int nWidth = 0;
	int nHeight = 0;
	glfwGetFramebufferSize(pWindow, &nWidth, &nHeight);
	glViewport(0, 0, nWidth, nHeight);

	//Shader
	// Build and compile our shader program
	const GLchar* vertexShaderFile = "C:\\Users\\zhu60\\Documents\\LearnOpenGL\\LearnOpenGL\\LearnOpenGL\\Shader\\vertex.glsl";
	const GLchar* fragShaderFile = "C:\\Users\\zhu60\\Documents\\LearnOpenGL\\LearnOpenGL\\LearnOpenGL\\Shader\\frag.glsl";
	Shader ourShader(vertexShaderFile, fragShaderFile);

	//for test
	//GLint nrAttributes;
	//glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	//std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
	// Set up vertex data 
	GLfloat vertices[] = {
		//vertex			 //color
		0.5f, -0.5f, 0.0f,    1.0f, 0.0f, 0.0f,   
		-0.5f, -0.5f, 0.0f,	 0.0f, 1.0f, 0.0f,
		0.0f, 0.5f, 0.0f,  0.0f, 0.0f, 1.0f,
		//-0.5f, 0.5f, 0.0f,	 0.0f, 0.5f, 1.0f,
	};

	GLuint indices[] = { 
	0, 1, 2, 
	//1, 2, 3  
	};

	GLuint VBO, VAO;
	GLuint EBO;
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)
	
	while (!glfwWindowShouldClose(pWindow))
	{
		//Event
		glfwPollEvents();
		
		//Clear
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Render
		ourShader.Use();
		
		/*GLfloat timeValue = glfwGetTime();
		GLfloat greenValue = (sin(timeValue) / 2) + 0.5;
		GLint vertexColorLocation = glGetUniformLocation(shaderProgram, "ourUniformColor");
		if (vertexColorLocation >= 0)
			glUniform4f(vertexColorLocation, 0.0f, greenValue, 1.0 - greenValue, 1.0f);*/

		glBindVertexArray(VAO);
		//Draw
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		//Swap
		glfwSwapBuffers(pWindow);
	}
	glfwTerminate();

	return 0;
}
