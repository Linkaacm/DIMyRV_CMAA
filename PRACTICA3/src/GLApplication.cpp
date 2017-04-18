/*
 *		GLApplication.cpp
 *		(Programa principal)
 *		
 *		Pr�ctica 3. EJERCICIO N�MERO 3
 *		Fecha oficial de entrega: 18 de Abril del 2017
 *		Autor: Armando Arturo Cruz Mendoza
 */

#include "Headers/GLApplication.h"

GLApplication::GLApplication() :
		windowManager(nullptr), camera(nullptr) {
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
			|| !windowManager->initialize(800, 700, "Window GLFW", false)) {
		this->destroy();
		exit(-1);
	}

	glViewport(0, 0, WindowManager::screenWidth, WindowManager::screenHeight);
	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);

	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	shader.initialize("Shaders/shader.vs", "Shaders/shader.fs");

	objModel.loadModel("objects/nanosuit/nanosuit.obj");
	objModel2.loadModel("objects/new/Pepsi/Pepsi_Can.obj");
	objModel3.loadModel("objects/new/dead/dead_man.obj");

}
void GLApplication::applicationLoop() {
	bool processInput = true;

	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

	while (processInput) {
		processInput = windowManager->processInput(true);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.turnOn();

		// Transformation matrices
		glm::mat4 view = camera->GetViewMatrix();
		glm::mat4 projection;
		projection = glm::perspective(45.0f,
				(GLfloat) WindowManager::screenWidth
						/ (GLfloat) WindowManager::screenHeight, 0.1f, 100.0f);
		// Get the uniform locations
		GLint modelLoc = shader.getUniformLocation("model");
		GLint viewLoc = shader.getUniformLocation("view");
		GLint projLoc = shader.getUniformLocation("projection");
		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// Draw the loaded model (ROBOT)
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		objModel.render(&shader);

		//Dibujar el modelo cargado (Pepsi)
		model = glm::translate(model, glm::vec3(3.10f, 7.1f, 1.2f));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		objModel2.render(&shader);

		//Dibujar el modelo cargado (Pepsi)
		model = glm::scale(model, glm::vec3(32.0f, 32.0f, 32.0f));
		model = glm::translate(model, glm::vec3(-5.0f, -11.5f, -8.0f));
		model = glm::scale(model, glm::vec3(3.8f, 3.85f, 3.8f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		objModel3.render(&shader);

		shader.turnOff();

		windowManager->swapTheBuffers();
	}
}

void GLApplication::destroy() {
	if (windowManager) {
		delete windowManager;
		windowManager = nullptr;
	}

	objModel.destroy();
	objModel2.destroy();
	objModel3.destroy();

}
