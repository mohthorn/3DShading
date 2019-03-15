#pragma once
#include "Object.h"
class Plane :
	public MyObject
{
public:
	glm::vec3 n0;
	float hit(glm::vec3 npe, glm::vec3 pe);
	Plane(glm::vec3 n0, glm::vec3 p0,glm::vec3 color);
	float diffuse(glm::vec3 &npe, glm::vec3 &pe, float &th, Light& light);
	float specular(glm::vec3 &npe, glm::vec3 &pe, float &th, Light& light);
};

