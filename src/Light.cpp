#include "Light.h"

Light::Light(vec3 pos, vec3 dir, float ambient, float diffuse, float specular, LType lType, int number, vec3 lightColor) {
	this->Lambient = ambient;
	this->Ldiffuse = diffuse;
	this->Lspecular = specular;
	this->LightType = lType;
	
	switch (lType) {
	case DIRECTIONAL:
		this->Lpos = pos;
		this->lightColor = lightColor;
		this->lightNumber = number;
		break;
	case POINT:
		this->Lpos = pos;
		this->lightColor = lightColor;
		this->lightNumber = number;
		break;
	case SPOT:
		this->Lpos = pos;
		this->lightColor = lightColor;
		this->Ldirection = dir;
		this->lightNumber = number;
		break;
	default:
		break;
	}
}


void Light::SetAtt(float constant, float lineal, float quadratic) {
	this->c1 = constant;
	this->c2 = lineal;
	this->c3 = quadratic;
}

void Light::SetAperture(float min, float max) {
	this->MaxAperture = max;
	this->MinAperture = min;
}

void Light::SetLight(Shader *shad, vec3 CamPos) {
	std::string variable;
	glUniform3f(glGetUniformLocation(shad->Program, "viewPos"), CamPos.x, CamPos.y, CamPos.z);

	switch (LightType){
	case DIRECTIONAL:
		glUniform3f(glGetUniformLocation(shad->Program,  "dlight.lightDirection"), Lpos.x, Lpos.y, Lpos.z);
		glUniform3f(glGetUniformLocation(shad->Program,  "dlight.lightColor"), lightColor.x, lightColor.y, lightColor.z);
		glUniform1f(glGetUniformLocation(shad->Program,  "dlight.c1"), c1);
		glUniform1f(glGetUniformLocation(shad->Program,  "dlight.c2"), c2);
		glUniform1f(glGetUniformLocation(shad->Program,  "dlight.c3"), c3);
		glUniform1f(glGetUniformLocation(shad->Program, "dlight.ambientStr"), Lambient);
		glUniform1f(glGetUniformLocation(shad->Program, "dlight.difuseStr"), Ldiffuse);
		glUniform1f(glGetUniformLocation(shad->Program, "dlight.specularStr"), Lspecular);
		break;
	case POINT:		
		variable = "plight[" + std::to_string(lightNumber) + "]";		
		glUniform3f(glGetUniformLocation(shad->Program, (variable + ".lightPos").c_str()), Lpos.x, Lpos.y, Lpos.z);
		glUniform3f(glGetUniformLocation(shad->Program, (variable + ".lightColor").c_str()), lightColor.x, lightColor.y, lightColor.z);
		glUniform1f(glGetUniformLocation(shad->Program, (variable + ".c1").c_str()), c1);
		glUniform1f(glGetUniformLocation(shad->Program, (variable + ".c2").c_str()), c2);
		glUniform1f(glGetUniformLocation(shad->Program, (variable + ".c3").c_str()), c3);
		glUniform1f(glGetUniformLocation(shad->Program, (variable + ".ambientStr").c_str()), Lambient);
		glUniform1f(glGetUniformLocation(shad->Program, (variable + ".difuseStr").c_str()), Ldiffuse);
		glUniform1f(glGetUniformLocation(shad->Program, (variable + ".specularStr").c_str()), Lspecular);
		break;
	case SPOT:
		variable = "slight[" + std::to_string(lightNumber) + "]";
		glUniform3f(glGetUniformLocation(shad->Program, (variable + ".lightPos").c_str()), Lpos.x, Lpos.y, Lpos.z);
		glUniform3f(glGetUniformLocation(shad->Program, (variable + ".lightDirection").c_str()), Ldirection.x, Ldirection.y, Ldirection.z);
		glUniform3f(glGetUniformLocation(shad->Program, (variable + ".lightColor").c_str()), lightColor.x, lightColor.y, lightColor.z);
		glUniform1f(glGetUniformLocation(shad->Program, (variable + ".c1").c_str()), c1);
		glUniform1f(glGetUniformLocation(shad->Program, (variable + ".c2").c_str()), c2);
		glUniform1f(glGetUniformLocation(shad->Program, (variable + ".c3").c_str()), c3);
		glUniform1f(glGetUniformLocation(shad->Program, (variable + ".maxAngle").c_str()), MaxAperture);
		glUniform1f(glGetUniformLocation(shad->Program, (variable + ".minAngle").c_str()), MinAperture);
		glUniform1f(glGetUniformLocation(shad->Program, (variable + ".ambientStr").c_str()), Lambient);
		glUniform1f(glGetUniformLocation(shad->Program, (variable + ".difuseStr").c_str()), Ldiffuse);
		glUniform1f(glGetUniformLocation(shad->Program, (variable + ".specularStr").c_str()), Lspecular);
		break;
	default:
		break;
	}	
}

Light::~Light() {
}

void Light::Rotate(vec3 rotation) {	
//opcional
}

void Light::SetDirection(vec3 dir) {
	this->Ldirection = dir;
}

void Light::SetPosition(vec3 pos) {
	this->Lpos = pos;
}

vec3 Light::GetColor() {
	return this->lightColor;
}