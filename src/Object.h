#pragma once
#include <GL\glew.h>
#include<glm.hpp>
#include <gtc\matrix_transform.hpp>

#include "Shader.h"

using namespace glm;

enum Direction {
	UP, DOWN, LEFT, RIGHT
};

class Object {

public:
	enum FigureType {
		cube = 0,
	};

	Object(vec3 scale, vec3 rotation, vec3 position, FigureType typef);
	~Object();

	void Draw();
	void Move(vec3 translation);
	void Rotate(Direction Dir);
	void Scale(vec3 scal);
	void Delete();
	mat4 GetModelMatrix(glm::mat4 model);
	vec3 GetPosition();

private:
	GLuint VBO, VAO, EBO;
	vec3 position;
	vec3 scale;
	vec3 rotation;
	GLfloat angleY, angleX;
};