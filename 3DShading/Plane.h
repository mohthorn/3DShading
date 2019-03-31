#pragma once
#include "Object.h"
class Plane :
	public MyObject
{
public:
	glm::vec3 n0;
	float hit(glm::vec3 npe, glm::vec3 pe);
	Plane(glm::vec3 n0, glm::vec3 p0,glm::vec3 color);
	//float diffuse(glm::vec3 &npe, glm::vec3 &pe, float &th, Light& light);
	//float specular(glm::vec3 &npe, glm::vec3 &pe, float &th, Light& light);
	float shadowLength(glm::vec3 & npl, Light light, float &ret, glm::vec3 ph);
	float textureMapping(glm::vec3& ph, glm::vec3 &p0, glm::vec3 &nt0, glm::vec3 &nt1, float s0, float s1, glm::vec3 &ret_color);
	float getNormal(glm::vec3 &ph, glm::vec3 &normal);
};

