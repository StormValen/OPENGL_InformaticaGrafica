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

float transparencia;
float angle{ 0.0f };
float angleX{ 0.0f };
float angleY{ 0.0f };

glm::vec3 posicionCamara = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 apuntaCamara = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 arribaCamara = glm::vec3(0.0f, 1.0f, 0.0f);

GLfloat fov = 45.0f;
GLfloat sensibilidad = 0.05;

Camara MyCamera(posicionCamara, apuntaCamara, sensibilidad,fov);

//Object *random = new Object(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f), Object::cube);

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
	Shader myShader("./src/TextureVertexShader.vertexshader", "./src/SimpleFragmentShader.fragmentshader");

	//Cubo grande.
	Object greatCube(glm::vec3(0.5f), glm::vec3(0.0f), glm::vec3(0.0f), Object::cube);
	glm::vec4 greatCubeColor(0.4f, 0.2f, 0.5f, 0.5f);
	glm::mat4 modelGreatCube;
	glm::mat4 tGreatCube;

	//Cubo luz.
	Object lightCube(glm::vec3(0.1f), glm::vec3(0.00001f), glm::vec3(0.0f), Object::cube);
	glm::vec4 lightCubeColor(1.0f);
	glm::mat4 modelLightCube;
	glm::mat4 tLightCube;

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

		glm::mat4 view;
		view = MyCamera.lookAt();
		glm::mat4 projection;
		projection = glm::perspective(MyCamera.GetFOV(), (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f);

		GLint modelLoc = glGetUniformLocation(myShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(myShader.Program, "view");
		GLint projLoc = glGetUniformLocation(myShader.Program, "projection");

		//Pasar de matrices al shader.
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		greatCube.Rotate(glm::vec3(1.0f, 0.0f, 0.0f));
		lightCube.Rotate(glm::vec3(0.0f,0.0f,0.0f));

		myShader.Use();
		
		greatCube.GetModelMatrix(modelGreatCube);
		tGreatCube = greatCube.GetPosition();
		glUniform4f(glGetUniformLocation(myShader.Program, "cubeColor"),greatCubeColor.x, greatCubeColor.y, greatCubeColor.z, greatCubeColor.w);
		glUniformMatrix4fv(glGetUniformLocation(myShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelGreatCube));
		glUniformMatrix4fv(glGetUniformLocation(myShader.Program, "transformacion"), 1, GL_FALSE, glm::value_ptr(tGreatCube));
		greatCube.Draw();

		lightCube.GetModelMatrix(modelLightCube);
		tLightCube= lightCube.GetPosition();
		glUniform4f(glGetUniformLocation(myShader.Program, "cubeColor"), lightCubeColor.x, lightCubeColor.y, lightCubeColor.z, lightCubeColor.w);
		glUniformMatrix4fv(glGetUniformLocation(myShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelLightCube));
		glUniformMatrix4fv(glGetUniformLocation(myShader.Program, "transformacion"), 1, GL_FALSE, glm::value_ptr(tLightCube));
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

	if (key == GLFW_KEY_LEFT && action == GLFW_REPEAT) {
		
	}

	if (key == GLFW_KEY_RIGHT && action == GLFW_REPEAT) {
	
	}

	if (key == GLFW_KEY_O && action == GLFW_PRESS) {
		glDisable(GL_DEPTH_TEST);
	}

	if (key == GLFW_KEY_P && action == GLFW_PRESS) {
		glEnable(GL_DEPTH_TEST);
	}
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	MyCamera.MouseMove(window, xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	MyCamera.MouseScroll(window, xoffset, yoffset);
}