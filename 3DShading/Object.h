#pragma once
#include<glm/glm.hpp>
#include "Light.h"
#define FAILCODE -1.0f
#define MISS 0.0f
#define HIT 1.0f
#define BOUNDARY -2.0f
using namespace glm;
class MyObject
{
public:
	//glm::vec3 n0;
	glm::vec3 p0;
	glm::vec3 color;
	glm::vec3 color_dark;
	int shape;
	virtual float hit(glm::vec3 npe, glm::vec3 pe) =0;	//returns th
	virtual float diffuse(glm::vec3 &npe, glm::vec3 &pe, float &th, Light& light) = 0;
	virtual float specular(glm::vec3 &npe, glm::vec3 &pe, float &th, Light& light) = 0;
	virtual float shadowLength(glm::vec3 & npl, Light light, float &ret) = 0;
	float specularFunction(float S);
	float diffuseFunction(float T);
};

