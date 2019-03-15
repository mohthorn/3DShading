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
	float diffuse(glm::vec3 &npe, glm::vec3 &pe, float &th, Light& light);
	float specular(glm::vec3 &npe, glm::vec3 &pe, float &th, Light& light);
	Sphere();
	Sphere(float r, glm::vec3 color, glm::vec3 p0);
	~Sphere();
};

