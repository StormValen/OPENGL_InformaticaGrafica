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
#include "material.h"
#include "Light.h"

const GLuint WIDTH = 1280, HEIGHT = 720;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

glm::vec3 posicionCamara = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 apuntaCamara = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 arribaCamara = glm::vec3(0.0f, 1.0f, 0.0f);

GLfloat fov = 45.0f;
GLfloat sensibilidad = 0.05;

Camara MyCamera(posicionCamara, apuntaCamara, sensibilidad,fov);

Object *greatCube;
Object *miniCube;

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
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Informatica_Grafica_ValentinGutierrez", nullptr, nullptr);
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
	Shader myShader("./src/VertexShaderPhongTexture.vs", "./src/FragmentShaderPhongTexture.fs");
	Shader lampShader("./src/LampVertexShader.vertexshader", "./src/SimpleFragmentShader.fragmentshader");

	//Cubo grande.
	greatCube = new Object(glm::vec3(0.7f), glm::vec3(0.0f), glm::vec3(0.0f,-1.0f,0.0f), Object::cube);
	glm::vec3 greatCubeColor = glm::vec3(1.0f, 0.31f, 0.5f);
	glm::mat4 modelGreatCube;

	//Mini cube.
	miniCube = new Object(glm::vec3(0.4f), glm::vec3(0.0f), glm::vec3(1.0f, 0.0f, -0.5f), Object::miniCube);
	glm::vec3 miniCubeColor = glm::vec3(0.0f,0.31f,0.5f);
	glm::mat4 modelMiniCube;

	//Cubos luz.
	Object lightCube(glm::vec3(0.1f), glm::vec3(1.0f), glm::vec3(0.0f), Object::cube);
	glm::mat4 modelLightCube;

	Object greenLightCube(glm::vec3(0.1f), glm::vec3(1.0f), glm::vec3(0.5f,-0.5f,0.5f), Object::cube);
	glm::mat4 modelGreenLightCube;

	Object yellowLightCube(glm::vec3(0.1f), glm::vec3(1.0f), glm::vec3(0.5f, 1.f, -1.0f), Object::cube);
	glm::mat4 modelYellowLightCube;

	Object redLightCube(glm::vec3(0.1f), glm::vec3(1.0f), glm::vec3(0.0f, 3.f, 0.0f), Object::cube);
	glm::mat4 modelRedLightCube;

	Object placeHolderDirectionalLight(glm::vec3(0.1f),glm::vec3(1.0f),glm::vec3(-7.0f,-7.0f,1.0f), Object::cube);
	glm::mat4 modelPlaceHolderDirectionalLight;

	//Objetos Luz.
	Light LuzAzul(lightCube.GetPosition(), glm::vec3(0.0f, -1.0f, 0.0f), 0.1f, 5.0f, 5.0f, Light::POINT, 0, glm::vec3(0.0f, 0.5f, 1.0f));
	Light LuzVerde(greenLightCube.GetPosition(), glm::vec3(0.0f, -1.0f, 0.0f), 0.1f, 5.0f, 5.0f, Light::POINT, 1, glm::vec3(0.0f, 1.0f, 0.5f));

	Light LuzAmarilla(yellowLightCube.GetPosition(), glm::vec3(0.0f,-1.0f,0.0f), 0.1f, 20.0f, 20.0f, Light::SPOT, 0 , glm::vec3(1.0f,1.0f,0.0f));
	Light LuzRoja(redLightCube.GetPosition(), glm::vec3(0.0f, -1.0f, 0.0f), 0.1f, 20.0f, 5.0f, Light::SPOT, 2, glm::vec3(1.0f, 0.0f, 0.0f));

	Light LuzBlancaDireccional(placeHolderDirectionalLight.GetPosition(), glm::vec3(1.0f, 1.0f, 0.0f), 0.1f, 0.5f, 5.0f, Light::DIRECTIONAL, 0, glm::vec3(1.0f, 1.0f, 1.0f));

	Material material("./gph/Materials/difuso.png", "./gph/Materials/especular.png", 200.0f);
	Material materialA("./gph/Materials/camo.jpg", "./gph/Materials/especular2.png", 200.0f);

	while (!glfwWindowShouldClose(window)) {
		//Color de fondo.
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

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

		myShader.Use(); //DRAW BIG BUE

		material.ActivateTextures();
		material.SetMaterial(&myShader);
		material.SetShininess(&myShader);
		
		GLint modelLoc = glGetUniformLocation(myShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(myShader.Program, "view");
		GLint projLoc = glGetUniformLocation(myShader.Program, "projection");

		//Pasar de matrices al shader.
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(greatCube->GetModelMatrix(modelGreatCube)));

		glUniform3f(glGetUniformLocation(myShader.Program, "cubeColor"),greatCubeColor.x, greatCubeColor.y, greatCubeColor.z);

		LuzAzul.SetAtt(1.0f, 0.7f, 1.8f);
		LuzAzul.SetAperture(20.0f, 45.0f);
		LuzAzul.SetLight(&myShader, MyCamera.GetPosition());

		LuzVerde.SetAtt(1.0f, 0.7f, 1.8f);
		LuzVerde.SetAperture(20.0f, 45.0f);
		LuzVerde.SetLight(&myShader, MyCamera.GetPosition());

		LuzAmarilla.SetAtt(1.0f, 0.7f, 1.8f);
		LuzAmarilla.SetAperture(20.0f, 45.0f);
		LuzAmarilla.SetLight(&myShader, MyCamera.GetPosition());

		LuzRoja.SetAtt(1.0f, 0.7f, 1.8f);
		LuzRoja.SetAperture(10.0f, 20.0f);
		LuzRoja.SetLight(&myShader, MyCamera.GetPosition());

		LuzBlancaDireccional.SetAtt(1.0f, 0.7f, 1.8f);
		LuzBlancaDireccional.SetAperture(20.0f, 45.0f);
		LuzBlancaDireccional.SetLight(&myShader, MyCamera.GetPosition());

		greatCube->Draw();

		myShader.Use(); //DRAW MINI CUBE

		materialA.ActivateTextures();
		materialA.SetMaterial(&myShader);
		materialA.SetShininess(&myShader);

		modelLoc = glGetUniformLocation(myShader.Program, "model");
		viewLoc = glGetUniformLocation(myShader.Program, "view");
		projLoc = glGetUniformLocation(myShader.Program, "projection");

		//Pasar de matrices al shader.
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(miniCube->GetModelMatrix(modelMiniCube)));

		glUniform3f(glGetUniformLocation(myShader.Program, "cubeColor"), miniCubeColor.x, miniCubeColor.y, miniCubeColor.z);

		LuzAzul.SetAtt(1.0f, 0.7f, 1.8f);
		LuzAzul.SetAperture(20.0f, 45.0f);
		LuzAzul.SetLight(&myShader, MyCamera.GetPosition());

		LuzVerde.SetAtt(1.0f, 0.7f, 1.8f);
		LuzVerde.SetAperture(20.0f, 45.0f);
		LuzVerde.SetLight(&myShader, MyCamera.GetPosition());

		LuzAmarilla.SetAtt(1.0f, 0.7f, 1.8f);
		LuzAmarilla.SetAperture(20.0f, 45.0f);
		LuzAmarilla.SetLight(&myShader, MyCamera.GetPosition());

		LuzRoja.SetAtt(1.0f, 0.7f, 1.8f);
		LuzRoja.SetAperture(10.0f, 20.0f);
		LuzRoja.SetLight(&myShader, MyCamera.GetPosition());

		LuzBlancaDireccional.SetAtt(1.0f, 0.7f, 1.8f);
		LuzBlancaDireccional.SetAperture(20.0f, 45.0f);
		LuzBlancaDireccional.SetLight(&myShader, MyCamera.GetPosition());

		miniCube->Draw();

		lampShader.Use(); //DRAW BLUE LIGHT CUBE

		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		//Pasar de matrices al shader.
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(lightCube.GetModelMatrix(modelLightCube)));

		glUniform3f(glGetUniformLocation(lampShader.Program, "lightColor"), LuzAzul.GetColor().x, LuzAzul.GetColor().y, LuzAzul.GetColor().z);
		lightCube.Draw();

		lampShader.Use(); //DRAW RED LIGHT CUBE

		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		//Pasar de matrices al shader.
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(redLightCube.GetModelMatrix(modelRedLightCube)));

		glUniform3f(glGetUniformLocation(lampShader.Program, "lightColor"), LuzRoja.GetColor().x, LuzRoja.GetColor().y, LuzRoja.GetColor().z);
		redLightCube.Draw();

		lampShader.Use(); //DRAW GREEN LIGHT CUBE

		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		//Pasar de matrices al shader.
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(greenLightCube.GetModelMatrix(modelGreenLightCube)));

		glUniform3f(glGetUniformLocation(lampShader.Program, "lightColor"), LuzVerde.GetColor().x, LuzVerde.GetColor().y, LuzVerde.GetColor().z);
		greenLightCube.Draw();

		lampShader.Use(); //DRAW YELLOW LIGHT CUBE

		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		//Pasar de matrices al shader.
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(yellowLightCube.GetModelMatrix(modelYellowLightCube)));

		glUniform3f(glGetUniformLocation(lampShader.Program, "lightColor"), LuzAmarilla.GetColor().x, LuzAmarilla.GetColor().y, LuzAmarilla.GetColor().z);
		yellowLightCube.Draw();

		lampShader.Use(); //DRAW DIRECTIONAL LIGHT CUBE

		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		//Pasar de matrices al shader.
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(placeHolderDirectionalLight.GetModelMatrix(modelPlaceHolderDirectionalLight)));

		glUniform3f(glGetUniformLocation(lampShader.Program, "lightColor"), LuzBlancaDireccional.GetColor().x, LuzBlancaDireccional.GetColor().y, LuzBlancaDireccional.GetColor().z);
		placeHolderDirectionalLight.Draw();

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
		glm::vec3 movimiento = glm::vec3(-0.025f, 0.0f, 0.0f);
		greatCube->Move(movimiento);
	}

	if (key == GLFW_KEY_RIGHT && action == GLFW_REPEAT) {
		glm::vec3 movimiento = glm::vec3(0.025f, 0.0f, 0.0f);
		greatCube->Move(movimiento);
	}

	if (key == GLFW_KEY_UP && action == GLFW_REPEAT) {
		glm::vec3 movimiento = glm::vec3(0.0f, 0.025f, 0.0f);
		greatCube->Move(movimiento);
	}

	if (key == GLFW_KEY_DOWN && action == GLFW_REPEAT) {
		glm::vec3 movimiento = glm::vec3(0.0f, -0.025f, 0.0f);
		greatCube->Move(movimiento);
	}
	if (key == GLFW_KEY_KP_8 && action == GLFW_REPEAT) {
		greatCube->Rotate(UP);
	}
	if (key == GLFW_KEY_KP_4 && action == GLFW_REPEAT) {
		greatCube->Rotate(LEFT);
	}
	if (key == GLFW_KEY_KP_6 && action == GLFW_REPEAT) {
		greatCube->Rotate(RIGHT);
	}
	if (key == GLFW_KEY_KP_2 && action == GLFW_REPEAT) {
		greatCube->Rotate(DOWN);
	}
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	MyCamera.MouseMove(window, xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	MyCamera.MouseScroll(window, xoffset, yoffset);
}