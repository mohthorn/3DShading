#include "pch.h"
#include "Light.h"

Light::Light(char type, glm::vec3 position, glm::vec3 direction, float theta)
{
	this->type = type;
	this->position = position;
	this->direction = normalize(direction);
	this->theta = theta;
}
