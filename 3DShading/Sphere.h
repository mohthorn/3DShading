#pragma once
#include<glm/glm.hpp>
#include "Object.h"
using namespace glm;
class Sphere :
	public MyObject
{
public:
	float r;
	float hit(glm::vec3 npe, glm::vec3 pe);
	float diffuse(glm::vec3 npe, glm::vec3 pe, glm::vec3 L, float theta);
	float specular(glm::vec3 npe, glm::vec3 pe, glm::vec3 L, float theta);
	Sphere();
	Sphere(float r, glm::vec3 color, glm::vec3 p0);
	~Sphere();
};

