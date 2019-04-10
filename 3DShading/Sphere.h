#pragma once
#include<glm/glm.hpp>
#include "Object.h"
//using namespace glm;
class Sphere :
	public MyObject
{
public:
	float r;
	glm::vec3 northPole;
	float hit(glm::vec3 npe, glm::vec3 pe, spSet &sp);
	//float diffuse(glm::vec3 &npe, glm::vec3 &pe, float &th, Light& light);
	//float specular(glm::vec3 &npe, glm::vec3 &pe, float &th, Light& light);
	float shadowLength(glm::vec3 & npl, Light light, float &ret, glm::vec3 ph) ;
	float getNormal(glm::vec3 &ph, glm::vec3 &normal, spSet &sp);
	float textureMapping(glm::vec3& ph, glm::vec3 &ret_color, spSet &sp);
	float solidMapping(glm::vec3& ph, glm::vec3 &ret_color);
	Sphere();
	Sphere(float r, glm::vec3 color, glm::vec3 p0);
	~Sphere();
};

class IFSphere :
	public MyObject
{
public:
	float hit(glm::vec3 npe, glm::vec3 pe, spSet &sp);
	//float diffuse(glm::vec3 &npe, glm::vec3 &pe, float &th, Light& light);
	//float specular(glm::vec3 &npe, glm::vec3 &pe, float &th, Light& light);
	float shadowLength(glm::vec3 & npl, Light light, float &ret, glm::vec3 ph);
	float getNormal(glm::vec3 &ph, glm::vec3 &normal, spSet &sp);
	float textureMapping(glm::vec3& ph, glm::vec3 &ret_color, spSet &sp);
	IFSphere();
	IFSphere(glm::vec3 color);
	~IFSphere();
};

