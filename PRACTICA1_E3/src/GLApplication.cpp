/*
 * GLApplication.cpp
 *
 *  Created on: 6/Marzo/2017
 *      Author: Linkaacm
 */

#include "Headers/GLApplication.h"

GLuint VBO, VAO;
Shader shader1;
Shader shader2;

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


	//Paso necesario para inicializar todos los shaders que existan
	shader1.initialize("Shaders/basic.vs", "Shaders/basic.fs");
	shader1.initialize("Shaders/basic.vs", "Shaders/basic.fs");


	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] = {
	// Posiciones        // Colores
			-0.6667f, -0.3333f, 0.0f, 1.0f, 0.0f, 0.0f,  // Abajo Izquierda
			-0.3333f, -0.3333f, 0.0f, 1.0f, 0.0f, 0.0f,  // Abajo Derecha
			-0.6667f, 0.3333f, 0.0f, 1.0f, 0.0f, 0.0f   // Arriba


			};
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
			(GLvoid*) 0);
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
			(GLvoid*) (3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO
}

void GLApplication::applicationLoop() {
	bool processInput = true;
	while (processInput) {
		processInput = windowManager->processInput(true);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		shader1.turnOn();

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		shader1.turnOff();

		windowManager->swapTheBuffers();
	}
}

void GLApplication::destroy() {
	if (windowManager) {
		delete windowManager;
		windowManager = nullptr;
	}

	shader1.destroy();

	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &VBO);

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAO);
}
