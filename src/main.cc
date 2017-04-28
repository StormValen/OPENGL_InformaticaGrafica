// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
// SOIL
#include <SOIL.h>
// ASSIMP
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "Shader.h"
#include "Object.h"
#include "Model.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void DoMovement();
const GLuint WIDTH = 800, HEIGHT = 800;

float transparencia;
float angle{ 0.0f };
float angleX{ 0.0f };
float angleY{ 0.0f };

glm::vec3 posicionCamara = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 apuntaCamara = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 arribaCamara = glm::vec3(0.0f, 1.0f, 0.0f);

bool keys[1024];
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
GLfloat lastX = 400;
GLfloat lastY = 400;
GLfloat Yaw = -90.0f;
GLfloat Pitch = 0.0f;
GLfloat fov = 45.0f;
bool firstMouse = true;

int main()
{
	//GLFW
	glfwInit();

	//Version OpenGL - Reescalado pantalla.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//Crear objeto ventana
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL 3", nullptr, nullptr);
	glfwMakeContextCurrent(window);


	//Raton habilitado sin mostrarse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Set callbacks utilizados.
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	//GLEW
	glewInit();

	//Dimensiones del viewport.
	glViewport(0, 0, WIDTH, HEIGHT);

	//Activar ZBuffer.
	glEnable(GL_DEPTH_TEST);

	//Variable Shader.
	Shader myShader("./src/TextureVertexShader.vertexshader", "./src/TextureFragmentShader.fragmentshader");



	//Datos de los vertices VBO.
	/*GLfloat vertices[] = {
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

	//Posiciones cubos en espacio escena.
	glm::vec3 cubePositions[] = {
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

	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	*/
	//Carga textura 1.
	/*GLuint texturaA;
	glGenTextures(1, &texturaA);

	glBindTexture(GL_TEXTURE_2D, texturaA);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //Unwraping de textura.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //Filtro de textura.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("./gph/spider/SpiderTex.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);*/

	/*
	//Carga textura 2.
	GLuint texturaB;
	glGenTextures(1, &texturaB);

	glBindTexture(GL_TEXTURE_2D, texturaB);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //Unwraping de textura.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //Filtro de textura.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	image = SOIL_load_image("./gph/Piedra.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);*/

	//Object *myObj = new Object(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f), Object::cube);
	//Model *SpiderModel = new Model("./gph/spider/spider.obj");
	Model SpiderModel("./gph/spider/spider.obj");

	//PruebaGitKraken

	while (!glfwWindowShouldClose(window))
	{
		//Color de fondo.
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

		//Limpiar ColorBuffer y ZBuffer.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Comporavar si alguna de las texlas ha sido pulsada.
		glfwPollEvents();

		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		DoMovement();

		
		//Enlace de las 2 texturas.
		/*glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texturaA);
		glUniform1i(glGetUniformLocation(myShader.Program, "ourTexture1"), 0);*/
		
		/*
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texturaB);
		glUniform1i(glGetUniformLocation(myShader.Program, "ourTexture2"), 1);*/

		//Paso transparencia para cambio de textura.
		glUniform1f(glGetUniformLocation(myShader.Program, "transparencia"), transparencia);

		//Activar shader.


		//CAMARA FPS.
		//////////////////////////////////////////////////////////////

		//Eje Z
		glm::vec3 zCamara = glm::normalize(posicionCamara - apuntaCamara); //El vector direccion apunta al contrario de donde esta mirando.

																		   //Eje X
		glm::vec3 xCamara = glm::normalize(glm::cross(arribaCamara, zCamara));

		//Eje Y
		glm::vec3 yCamara = glm::cross(zCamara, xCamara);

		//Matriz LookAt
		glm::mat4 view;
		view = glm::lookAt(posicionCamara, posicionCamara + apuntaCamara, arribaCamara);

		//Codigo para que la camara rote toda la escena automaticamente.
		/*GLfloat radius = 10.0f;
		GLfloat camX = sin(glfwGetTime())*radius;
		GLfloat camZ = cos(glfwGetTime())*radius;
		view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));*/

		//////////////////////////////////////////////////////////////

		//Camara.
		glm::mat4 vista;
		glm::mat4 proyeccion;
		vista = glm::translate(vista, glm::vec3(0.0f, 0.0f, -3.0f));
		proyeccion = glm::perspective(glm::radians(fov), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

		GLint modelLoc = glGetUniformLocation(myShader.Program, "matrix");
		GLint viewLoc = glGetUniformLocation(myShader.Program, "vista");
		GLint projLoc = glGetUniformLocation(myShader.Program, "proyeccion");

		//Pasar de matrices al shader.
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proyeccion));

		myShader.Use();
		//glm::mat4 model = myObj->GetModelMatrix();
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(glGetUniformLocation(myShader.Program, "matrix"), 1, GL_FALSE, glm::value_ptr(model));
		SpiderModel.Draw(myShader);
		//myObj->Draw();

		//Aplicar transformaciones.
		//glBindVertexArray(VAO);

		//Matriz de transformacion para el cubo [0] -> Interaccion usuario.
		/*glm::mat4 model1;
		model1 = glm::translate(model1, cubePositions[0]); //Recolocacion en posicion cubo.
		model1 = glm::rotate(model1, glm::radians(angleY), glm::vec3(1.0f, 0.0f, 0.0f)); //Rotacion en Y.
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model1)); //Paso al shader.
		model1 = glm::rotate(model1, glm::radians(angleX), glm::vec3(0.0f, 1.0f, 0.0f)); //Rotacion en X.
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model1)); //Paso al shader.
		glDrawArrays(GL_TRIANGLES, 0, 36); //Pintado del cubo.

		//Bucle trasformaciones cubos.
		for (GLuint i = 1; i < 10; i++)
		{
		//Matriz de transformacion para cubos automaticos.
		glm::mat4 movementCubesXY;
		movementCubesXY = glm::translate(movementCubesXY, cubePositions[i]);//Recolocacion en posicion cubo.
		movementCubesXY = glm::rotate(movementCubesXY, (GLfloat)glfwGetTime() * 1, glm::vec3(1.0f, 1.0f, 0.0f)); //Rotacion en X e Y progresiva.
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(movementCubesXY)); //Paso al shader.
		glDrawArrays(GL_TRIANGLES, 0, 36); //Pintado del cubo
		}
		glBindVertexArray(0);*/

		//Swap de buffers.
		glfwSwapBuffers(window);
	}

	//Eliminacion de memoria reservada.
	/*glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);*/

	//GLFW OFF.
	glfwTerminate();
	return 0;
}

void DoMovement() {
	GLfloat cameraSpeed = 5.0f*deltaTime;
	if (keys[GLFW_KEY_W])
		posicionCamara += cameraSpeed * apuntaCamara;
	if (keys[GLFW_KEY_S])
		posicionCamara -= cameraSpeed * apuntaCamara;
	if (keys[GLFW_KEY_A])
		posicionCamara -= glm::normalize(glm::cross(apuntaCamara, arribaCamara)) * cameraSpeed;
	if (keys[GLFW_KEY_D])
		posicionCamara += glm::normalize(glm::cross(apuntaCamara, arribaCamara)) * cameraSpeed;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
		transparencia = 1;
	}

	if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
		transparencia = 0;
	}

	if (key == GLFW_KEY_LEFT && action == GLFW_REPEAT) {
		angleX -= 1.0f;
	}

	if (key == GLFW_KEY_RIGHT && action == GLFW_REPEAT) {
		angleX += 1.0f;
	}

	if (key == GLFW_KEY_UP && action == GLFW_REPEAT) {
		angleY -= 1.0f;
	}

	if (key == GLFW_KEY_DOWN && action == GLFW_REPEAT) {
		angleY += 1.0f;
	}

	if (action == GLFW_PRESS)
		keys[key] = true;

	if (action == GLFW_RELEASE)
		keys[key] = false;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	GLfloat sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	Yaw += xoffset;
	Pitch += yoffset;

	if (Pitch > 89.0f)
		Pitch = 89.0f;
	if (Pitch < -89.0f)
		Pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	apuntaCamara = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= yoffset;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
}