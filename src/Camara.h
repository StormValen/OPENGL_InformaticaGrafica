#pragma once
#include <GL/glew.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <GLFW\glfw3.h>

using namespace std;

class Camara {
private:
	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	GLfloat Deltatime;
	GLfloat Lastframe;
	GLfloat LastMx; //Mouse position x
	GLfloat LastMy; //Mouse position y
	GLfloat Sensivity;
	GLboolean firstMouse;
	GLfloat PITCH;
	GLfloat YAW;
	GLfloat FOV;
public:
	Camara(glm::vec3 position, glm::vec3 direction, GLfloat sensivity, GLfloat fov);
	void DoMovement(GLFWwindow * window);
	void MouseMove(GLFWwindow* window, double xpos, double ypos);
	void MouseScroll(GLFWwindow *window, double xScroll, double yScroll);
	glm::mat4 lookAt();
	GLfloat GetFOV();
	glm::vec3 GetPosition();
};