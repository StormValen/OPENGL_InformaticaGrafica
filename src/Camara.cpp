#include "Camara.h"
#include <GLFW\glfw3.h>


Camara::Camara(glm::vec3 position, glm::vec3 direction, GLfloat sensivity, GLfloat fov) {
	this->cameraPos = position;
	this->cameraFront = glm::normalize(this->cameraPos-direction);
	glm::vec3 cameraRight = glm::normalize(glm::cross(cameraFront, glm::vec3(0.f, 1.f, 0.f)));
	this->cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
	this->Sensivity = sensivity;
	this->FOV = fov;

	this->Deltatime = 0.0f;
	this->Lastframe = 0.0f;
	this->LastMx = 400;
	this->LastMy = 400;
	this->firstMouse = true;
	this->YAW = -90.0f;
	this->PITCH = 0.0f;
}

glm::vec3 Camara::GetPosition() {
	return this->cameraPos;
}

void Camara::DoMovement(GLFWwindow * window) {
	GLfloat currTime = glfwGetTime();
	Deltatime = currTime - Lastframe;
	Lastframe = currTime;
	GLfloat cameraSpeed = 2.0f*Deltatime;

	if (glfwGetKey(window, GLFW_KEY_W))
		this->cameraPos += cameraSpeed * this->cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S))
		this->cameraPos -= cameraSpeed * this->cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A))
		this->cameraPos -= glm::normalize(glm::cross(this->cameraFront, this->cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D))
		this->cameraPos += glm::normalize(glm::cross(this->cameraFront, this->cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_Q))
		this->cameraPos -= cameraSpeed* this->cameraUp;
	if (glfwGetKey(window, GLFW_KEY_E))
		this->cameraPos += cameraSpeed* this->cameraUp;
}

void Camara::MouseMove(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse)
	{
		this->LastMx = xpos;
		this->LastMy = ypos;
		this->firstMouse = false;
	}

	GLfloat xoffset = xpos - LastMx;
	GLfloat yoffset = LastMy - ypos;
	this->LastMx = xpos;
	this->LastMy = ypos;

	GLfloat sensitivity = 0.05f;
	xoffset *= this->Sensivity;
	yoffset *= this->Sensivity;

	this->YAW += xoffset;
	this->PITCH += yoffset;

	if (this->PITCH > 89.0f)
		this->PITCH = 89.0f;
	if (this->PITCH < -89.0f)
		this->PITCH = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(this->YAW)) * cos(glm::radians(this->PITCH));
	front.y = sin(glm::radians(this->PITCH));
	front.z = sin(glm::radians(this->YAW)) * cos(glm::radians(this->PITCH));
	this->cameraFront = glm::normalize(front);
}

void Camara::MouseScroll(GLFWwindow *window, double xScroll, double yScroll) {
	if (FOV >= 1.0f && FOV <= 45.0f)
		FOV -= yScroll;
	if (FOV <= 1.0f)
		FOV = 1.0f;
	if (FOV >= 45.0f)
		FOV = 45.0f;
}

glm::mat4 Camara::lookAt() {
	return glm::lookAt(this->cameraPos, this->cameraPos + this->cameraFront, this->cameraUp);
}

GLfloat Camara::GetFOV() {
	return this->FOV;
}