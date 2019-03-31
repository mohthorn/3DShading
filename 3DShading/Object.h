#pragma once
#include<glm/glm.hpp>
#include "Light.h"
#include "ImageData.h"
#define FAILCODE -1.0f
#define MISS 0.0f
#define HIT 1.0f
#define BOUNDARY -2.0f
#define SPECTHRESHOLD 200.0f
#define PI 3.14
//using namespace glm;
class MyObject
{
public:
	//glm::vec3 n0;
	glm::vec3 p0;
	glm::vec3 color;
	glm::vec3 color_dark;
	glm::vec3 color_specular;
	ImageData *texture;
	int shape;
	virtual float hit(glm::vec3 npe, glm::vec3 pe) =0;	//returns th
	float diffuse(glm::vec3 &npe, glm::vec3 &pe, float &th, Light& light);
	float specular(glm::vec3 &npe, glm::vec3 &pe, float &th, Light& light) ;
	virtual float shadowLength(glm::vec3 & npl, Light light, float &ret, glm::vec3 ph) = 0;
	virtual float getNormal(glm::vec3 &ph, glm::vec3 &normal) = 0;
	virtual float textureMapping(glm::vec3& ph, glm::vec3 &p0, glm::vec3 &nt0, glm::vec3 &nt1, float s0, float s1, glm::vec3 &ret_color) =0 ;
	float specularFunction(float S);
	float diffuseFunction(float T);
	float shadowFunction(float T_s);

};

