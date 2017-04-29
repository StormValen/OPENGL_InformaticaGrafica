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

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void DoMovement();
const GLuint WIDTH = 800, HEIGHT = 800;

float transparencia;
float angle{ 0.0f };
float angleX{ 0.0f };
float angleY{ 0.0f };

glm::vec3 posicionCamara = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 apuntaCamara = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 arribaCamara = glm::vec3(0.0f, 1.0f, 0.0f);


bool firstMouse = true;
GLfloat sensibilidad = 0.05;

Camara MyCamera(posicionCamara, apuntaCamara, sensibilidad,fov);

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

	//Object *myObj = new Object(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f), Object::cube);
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

		MyCamera.DoMovement(window);

		
		//Enlace de las 2 texturas.
		/*glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texturaA);
		glUniform1i(glGetUniformLocation(myShader.Program, "ourTexture1"), 0);*/

		//Paso transparencia para cambio de textura.
		//glUniform1f(glGetUniformLocation(myShader.Program, "transparencia"), transparencia);

		glm::mat4 view;
		view = MyCamera.lookAt();
		glm::mat4 proyeccion;
		proyeccion = glm::perspective(MyCamera.GetFOV(), (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f);

		GLint modelLoc = glGetUniformLocation(myShader.Program, "matrix");
		GLint viewLoc = glGetUniformLocation(myShader.Program, "vista");
		GLint projLoc = glGetUniformLocation(myShader.Program, "proyeccion");

		//Pasar de matrices al shader.
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proyeccion));

		myShader.Use();
		//glm::mat4 model = myObj->GetModelMatrix();
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
		glUniformMatrix4fv(glGetUniformLocation(myShader.Program, "matrix"), 1, GL_FALSE, glm::value_ptr(model));
		SpiderModel.Draw(myShader);
		//myObj->Draw();

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
	MyCamera.MouseMove(window, xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	MyCamera.MouseScroll(window, xoffset, yoffset);
}