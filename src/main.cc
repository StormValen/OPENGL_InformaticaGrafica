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
#include "Camara.h"

const GLuint WIDTH = 1280, HEIGHT = 720;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

GLfloat angleR{ 0.0f };

glm::vec3 posicionCamara = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 apuntaCamara = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 arribaCamara = glm::vec3(0.0f, 1.0f, 0.0f);

GLfloat fov = 45.0f;
GLfloat sensibilidad = 0.05;

Camara MyCamera(posicionCamara, apuntaCamara, sensibilidad,fov);

GLboolean tPos = false;
GLboolean tRot = false;
glm::vec3 movimiento = glm::vec3(0.0f);
glm::vec3 rotacion = glm::vec3(0.0f);

glm::vec3 lightColor = glm::vec3(1.0f);

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
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Informatica_Grafica", nullptr, nullptr);
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
	Shader myShader("./src/TextureVertexShader.vertexshader", "./src/PhongFragmentShader.fragmentshader");
	Shader lampShader("./src/LampVertexShader.vertexshader", "./src/SimpleFragmentShader.fragmentshader");

	//Cubo grande.
	Object greatCube(glm::vec3(0.7f), glm::vec3(0.0f), glm::vec3(0.0f), Object::cube);
	glm::vec3 greatCubeColor = glm::vec3(1.0f, 0.31f, 0.5f);
	glm::mat4 modelGreatCube;
	glm::mat4 tGreatCube;

	//Cubo luz.
	Object lightCube(glm::vec3(0.1f), glm::vec3(1.0f), glm::vec3(1.0f), Object::cube);
	glm::vec3 lightCubeColor = glm::vec3(1.0f);
	glm::mat4 modelLightCube;
	glm::mat4 tLightCube;

	greatCube.Rotate(glm::vec3(1.0f, 1.0f, 1.0f),angleR);

	while (!glfwWindowShouldClose(window))
	{
		//Color de fondo.
		glClearColor(0.1f, 0.1f, 0.1f, 0.0f);

		//Limpiar ColorBuffer y ZBuffer.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Comporavar si alguna de las texlas ha sido pulsada.
		glfwPollEvents();
		
		//Actualizacion camara.
		MyCamera.DoMovement(window);

		if (tPos) {
			greatCube.Move(movimiento);
			tPos = false;
		}

		glm::mat4 view;
		view = MyCamera.lookAt();
		glm::mat4 projection;
		projection = glm::perspective(MyCamera.GetFOV(), (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f);

		myShader.Use();
		
		GLint modelLoc = glGetUniformLocation(myShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(myShader.Program, "view");
		GLint projLoc = glGetUniformLocation(myShader.Program, "projection");

		//Pasar de matrices al shader.
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(greatCube.GetModelMatrix(modelGreatCube)));

		tGreatCube = greatCube.GetTransMatrix();
		glUniformMatrix4fv(glGetUniformLocation(myShader.Program, "transformacion"), 1, GL_FALSE, glm::value_ptr(tGreatCube));

		glUniform3f(glGetUniformLocation(myShader.Program, "cubeColor"),greatCubeColor.x, greatCubeColor.y, greatCubeColor.z);
		glUniform3f(glGetUniformLocation(myShader.Program, "lightColor"), lightColor.x, lightColor.y, lightColor.z);
		greatCube.Draw();

		lampShader.Use();

		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		//Pasar de matrices al shader.
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(lightCube.GetModelMatrix(modelLightCube)));

		glUniform3f(glGetUniformLocation(lampShader.Program, "lightColor"), lightColor.x,  lightColor.y , lightColor.z);
		lightCube.Draw();

		//Swap de buffers.
		glfwSwapBuffers(window);
	}
	//GLFW OFF.
	glfwTerminate();
	return 0;
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
		tPos = true;
		movimiento = glm::vec3(-0.05f, 0.0f, 0.0f);
	}

	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
		tPos = true;
		movimiento = glm::vec3(0.05f, 0.0f, 0.0f);
	}

	if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
		tPos = true;
		movimiento = glm::vec3(0.0f, 0.05f, 0.0f);
	}

	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
		tPos = true;
		movimiento = glm::vec3(0.0f, -0.05f, 0.0f);
	}
	if (key == GLFW_KEY_KP_8 && action == GLFW_PRESS) {
		tRot = true;
		rotacion = glm::vec3(0.0f,1.0f,0.0f);
	}
	if (key == GLFW_KEY_KP_4 && action == GLFW_PRESS) {
		tRot = true;
		angleR = 20.0f;
		rotacion = glm::vec3(1.0f, 0.0f, 0.0f);
	}
	if (key == GLFW_KEY_KP_6 && action == GLFW_PRESS) {
		tRot = true;
		rotacion = glm::vec3(1.0f, 0.0f, 0.0f);
	}
	if (key == GLFW_KEY_KP_2 && action == GLFW_PRESS) {
		tRot = true;
		angleR += 20.0f;
		rotacion = glm::vec3(0.0f, 1.0f, 0.0f);
	}
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	MyCamera.MouseMove(window, xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	MyCamera.MouseScroll(window, xoffset, yoffset);
}