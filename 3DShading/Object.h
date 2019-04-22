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
#define ENERGYTHRESHOLD 0.01
#define IRR 0
//using namespace glm;

class spSet
{
public:
	glm::vec3 ph;
	float u;
	float v;
	int hitNum;
};

class Ray
{
public:
	glm::vec3 v;
	glm::vec3 p;
};

class FresnelSet
{
public:
	Ray rf;	//reflected ray
	Ray rt;	//refracted ray
	glm::vec3 color;
	float energy;
	FresnelSet() { energy = 1; }
};

class MyObject
{
public:
	//glm::vec3 n0;
	glm::vec3 p0;
	glm::vec3 color;
	glm::vec3 color_dark;
	glm::vec3 color_specular;
	ImageData *texture;
	ImageData *normalMap;
	ImageData *irrMap;
	glm::vec3 txP0;
	glm::vec3 nt0;
	glm::vec3 nt1;
	float s0;
	float s1;
	float eps = 1e-2;
	float transparency = 1;
	float yeta = 2.0 / 3.0;
	int shape;
	virtual float hit(glm::vec3 npe, glm::vec3 pe, spSet &sp) =0;	//returns th
	float diffuse(glm::vec3 &npe, glm::vec3 &pe, float &th, Light& light, spSet &sp);
	float specular(glm::vec3 &npe, glm::vec3 &pe, float &th, Light& light, spSet &sp) ;
	virtual float shadowLength(glm::vec3 & npl, Light light, float &ret, glm::vec3 ph) = 0;
	virtual float getNormal(glm::vec3 &ph, glm::vec3 &normal, spSet &sp) = 0;
	virtual float textureMapping(glm::vec3& ph, glm::vec3 &ret_color, spSet &sp) =0 ;
	float fresnel(glm::vec3 ph, MyObject** obj, int objListLen, Ray inCome, FresnelSet &fs, spSet ss, glm::vec3 distort);
	float shadowCast(glm::vec3 ph, MyObject** obj, int objListLen,Light LS, spSet ss);
	float specularFunction(float S);
	float diffuseFunction(float T);
	float shadowFunction(float T_s);
	float fresnelFunction(float F);
};



