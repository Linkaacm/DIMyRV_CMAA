/*
 * GLApplication.cpp
 *
 *  Created on: 06/Marzo/2017
 *      Author: Linkaacm
 */

#include "Headers/GLApplication.h"

// Shaders
const GLchar* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 position;\n"
		"void main()\n"
		"{\n"
		"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
		"}\0";
const GLchar* fragmentShaderSource = "#version 330 core\n"
		"out vec4 color;\n"
		"void main()\n"
		"{\n"
		"color = vec4(0.811f, 0.972f, 0.709f, 1.0f);\n"
		"}\n\0";

const GLchar* fragmentShaderSAUCE2 = "#version 330 core\n"
		"out vec4 color;\n"
		"void main()\n"
		"{\n"
		"color = vec4(0.850f, 0.709f, 0.972f, 1.0f);\n"
		"}\n\0";

//Para poder crear m�s de un VAO, o, VBO, simplemente se declaran como arreglos.

GLuint VBO[] = { 0,0 };
GLuint VAO[] = { 0,0 };
GLuint EBO;
GLint vertexShader, fragmentShader, fragmentShader2, shaderProgram, shaderPROG2;

GLApplication::GLApplication() :
		windowManager(nullptr) {
}

GLApplication::~GLApplication() {
	destroy();
}

void GLApplication::GLMain() {
	initialize();
	applicationLoop();
}

void GLApplication::initialize() {
	if (!windowManager
			|| !windowManager->initialize(800, 700, "Triangles", false)) {
		this->destroy();
		exit(-1);
	}

	glViewport(0, 0, WindowManager::screenWidth, WindowManager::screenHeight);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Build and compile our shader program
	// Vertex shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// Check for compile time errors
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog
				<< std::endl;
	}
	// Fragment shader
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// Check for compile time errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog
				<< std::endl;
	}

	//		Fragment shader 2!
	fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader2, 1, &fragmentShaderSAUCE2, NULL);
	glCompileShader(fragmentShader2);
	//		Se revisa la compilacion
	glGetShaderiv(fragmentShader2, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader2, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog
			<< std::endl;
	}

	// Link shaders
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// Check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog
				<< std::endl;
	}

	//		Enlazamiento del fragmentShader2 !!
	shaderPROG2 = glCreateProgram();
	glAttachShader(shaderPROG2, vertexShader);
	glAttachShader(shaderPROG2, fragmentShader2);
	glLinkProgram(shaderPROG2);
	// Check for linking errors
	glGetProgramiv(shaderPROG2, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderPROG2, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog
			<< std::endl;
	}

	GLfloat vertices[] = { -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f,
		0.0f, 0.5f, -0.5f, 0.0f, 1.0f, 0.5f, 0.0f, 0.0f, 0.5f,
		0.0f };				//V�rtices totales

	GLfloat vertexTri1[] = { -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f,
		0.0f};	//Tri�ngulo 1
	GLfloat vertexTri2[] = { 0.5f, -0.5f, 0.0f, 1.0f, 0.5f, 0.0f, 0.0f, 0.5f,
		0.0f };	//Tri�ngulo 2

	// This is for the render with index element
	/*GLfloat vertices[] = { 0.5f, 0.5f, 0.0f,  // Top Right
	 0.5f, -0.5f, 0.0f,  // Bottom Right
	 -0.5f, -0.5f, 0.0f,  // Bottom Left
	 -0.5f, 0.5f, 0.0f   // Top Left
	 };*/
	GLuint indices[] = {  // Note that we start from 0!
			0, 1, 3,  // First Triangle
					1, 2, 3   // Second Triangle
			};

	glGenVertexArrays(2, VAO);
	glGenBuffers(2, VBO);
	// This is for the render with index element
	//glGenBuffers(1, &EBO);



	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// This is for the render with index element
	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	 glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
	 GL_STATIC_DRAW);*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
			(GLvoid*) 0);
	glEnableVertexAttribArray(0);



	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// This is for the render with index element
	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
	GL_STATIC_DRAW);*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
		(GLvoid*)0);
	glEnableVertexAttribArray(0);



	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void GLApplication::applicationLoop() {
	bool processInput = true;
	while (processInput) {
		processInput = windowManager->processInput(true);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);				//Color de fondo

		// Draw our first triangle
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);				//ESTA ES LA FUNCI�N QUE DIBUJA DEPENDIENDO DE CU�NTOS V�RTICES HAY
		glUseProgram(shaderPROG2);
		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_TRIANGLES, 3, 3);				//ESTA ES LA FUNCI�N QUE DIBUJA DEPENDIENDO DE CU�NTOS V�RTICES HAY
		// This is for the render with index element
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		windowManager->swapTheBuffers();
	}
}

void GLApplication::destroy() {
	if (windowManager) {
		delete windowManager;
		windowManager = nullptr;
	}

	glUseProgram(0);

	glDetachShader(shaderProgram, vertexShader);
	glDetachShader(shaderProgram, fragmentShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glDeleteProgram(shaderProgram);

	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(2, VBO);

	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	 glDeleteBuffers(1, &EBO);*/

	glBindVertexArray(0);
	glDeleteVertexArrays(2, VAO);
}
