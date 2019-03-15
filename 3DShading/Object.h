#pragma once
#include<glm/glm.hpp>
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
	int shape;
	virtual float hit(glm::vec3 npe, glm::vec3 pe) =0;	//returns th
	virtual float diffuse(glm::vec3 npe, glm::vec3 pe, glm::vec3 L, float theta) = 0;
	virtual float specular(glm::vec3 npe, glm::vec3 pe, glm::vec3 L, float theta) = 0;
	float specularFunction(float S);
	float diffuseFunction(float T);
};

