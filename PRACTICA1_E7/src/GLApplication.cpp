//
//**
		//PRÁCTICA 1. EJERCICIO 7
		//ARMANDO ARTURO CRUZ MENDOZA
		//FECHA DE CREACIÓN: 6 DE MAYO DEL 2017
//**
//

#include "Headers/GLApplication.h"

// Shaders
const GLchar* vertexShaderSource =
		"#version 330 core\n"
		"layout (location = 0) in vec3 posicionVBO;\n"
		"layout (location = 1) in vec3 colorVBO;\n"
		"out vec4 usaEsteColor;\n"
		"void main()\n"
		"{\n"
		"gl_Position = vec4(posicionVBO, 1.0f);\n"
		"usaEsteColor = vec4(colorVBO, 1.0f);"
		"}\0"
		;
const GLchar* fragmentShaderSource =
		"#version 330 core\n"
		"in vec4 usaEsteColor;\n"
		"out vec4 colorFinal;\n"
		"void main()\n"
		"{\n"
		"colorFinal = usaEsteColor;\n"
		"}\0"
		;

GLuint VBO, VAO, EBO;
GLint vertexShader, fragmentShader, shaderProgram;

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
			|| !windowManager->initialize(800, 800, "Triangles", false)) {
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

	//El formato del VBO, por cada vértice es:
	// 3 valores de la posición + 3 valores del color
	GLfloat DatosBuffer[] =
	{
		//Vértices de frente
		-0.5f, -0.5f, 0.5f,		1.0f, 1.0f, 1.0f,				//Punto 0. Abajo-Izquierda
		0.5f, -0.5f, 0.5f,		1.0f, 0.843f, 0.0f,				//Punto 1. Abajo-Derecha
		0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 0.0f,				//Punto 2. Arriba-Derecha
		-0.5f, 0.5f, 0.5f,		0.294f, 0.0f, 0.509f,			//Punto 3. Arriba-Izquierda
		//Vértices de atrás
		-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 1.0f,				//Punto 4. Abajo-Izquierda
		0.5f, -0.5f, -0.5f,		1.0f, 0.843f, 0.0f,				//Punto 5. Abajo-Derecha
		0.5f, 0.5f, -0.5f,		0.0f, 0.0f, 0.0f,				//Punto 6. Arriba-Derecha
		-0.5f, 0.5f, -0.5f,		0.294f, 0.0f, 0.509f,			//Punto 7. Arriba-Izquierda
	};

	// ORO 1.0f, 0.843f, 0.0f
	// INDIGO 0.294f, 0.0f, 0.509f

	GLuint indices[] =
	{
		//Frente
		0, 1, 2,
		0, 2, 3,
		//Cara derecha
		1, 5, 6,
		1, 6, 2,
		//Atrás
		5, 4, 7,
		5, 7, 6,
		//Cara izquierda
		4, 0, 3,
		4, 3, 7,
		//Abajo
		4, 5, 1,
		4, 1, 0,
		//Arriba
		3, 2, 6,
		3, 6, 7,
	};


	//Aquí empieza la sección de enlazamientos (bindings)
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//El VAO se enlaza a toda la configuración más adelante
	glBindVertexArray(VAO);
	
	//Se enlaza y configura el VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(DatosBuffer), DatosBuffer, GL_STATIC_DRAW);
	//Se enlaza y configura el EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Se espesifican todos los atributos de vértice necesarios
	//	(En este caso, son dos atributos)
	
	//Formato de glVertexAttribPointer
	//	1. Indice de atributo
	//	2. Tamaño del atributo
	//	3. El tipo (variable) del atributo
	//	4. FALSE --> datos YA normalizados
	//	5. Tamaño del stride (distancia entre datos consecutivos)
	//	6. Offset. Distancia para leer el primer componente

	//Atributo de posición
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Atributo de color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);

	//Finalmente, por recomendación, se desenlaza VAO para usarlo más adelante.
	glBindVertexArray(0);

}

void GLApplication::applicationLoop() {
	bool processInput = true;
	while (processInput) {
		processInput = windowManager->processInput(true);
		//Limpia el buffer de color y le asigna un color
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

		//Sección de dibujo

		//Se usa este shader
		glUseProgram(shaderProgram);
		//Se enlaza el VAO
		glBindVertexArray(VAO);
		//Función de dibujo
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		//Se desenlaza el VAO
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
	glDeleteBuffers(1, &VBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &EBO);

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAO);
}
