#pragma once
#include "Object.h"
#include <string.h>
#include<tuple> // for tuple 
#define INITIAL_BUFFER 200

class Plane :
	public MyObject
{
public:
	glm::vec3 n0;
	float hit(glm::vec3 npe, glm::vec3 pe, spSet &sp);
	Plane(glm::vec3 n0, glm::vec3 p0,glm::vec3 color);
	//float diffuse(glm::vec3 &npe, glm::vec3 &pe, float &th, Light& light);
	//float specular(glm::vec3 &npe, glm::vec3 &pe, float &th, Light& light);
	float shadowLength(glm::vec3 & npl, Light light, float &ret, glm::vec3 ph);
	float textureMapping(glm::vec3& ph, glm::vec3 &ret_color, spSet &sp);
	float getNormal(glm::vec3 &ph, glm::vec3 &normal, spSet &sp);
};

class ObjFromFile :
	public MyObject
{
public:
	glm::vec3 *vertices;
	glm::vec2 *texturePoints;
	glm::vec3 *normalPoints;
	tuple <int, int, int> *tuples;
	tuple <int, int, int> *texTuple;
	tuple <int, int, int> *normTuple;
	int vNum;
	int tNum;
	int texNum;
	int normNum;

	ObjFromFile(char * filename);
	float hit(glm::vec3 npe, glm::vec3 pe, spSet &sp);
	float shadowLength(glm::vec3 & npl, Light light, float &ret, glm::vec3 ph);
	float textureMapping(glm::vec3& ph, glm::vec3 &ret_color, spSet &sp);
	float getNormal(glm::vec3 &ph, glm::vec3 &normal, spSet &sp);
};