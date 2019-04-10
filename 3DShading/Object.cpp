#include "pch.h"
#include "Object.h"

float MyObject::diffuse(glm::vec3 & npe, glm::vec3 & pe, float & th, Light & light, spSet &sp)
{
	float T = 0;
	if (th > 0)
	{
		glm::vec3 ph = pe + th * npe;
		glm::vec3 n;
		getNormal(ph, n, sp);
		glm::vec3 nL = normalize(light.position - ph);
		if (light.type == SPOT && glm::dot(nL, light.direction) < cos(light.theta))
		{
			return 0;
		}
		if (light.type == DIR)
			nL = normalize(light.direction);
		T = glm::dot(nL, n);
		T = (T + 1) / 2.0f;
		return diffuseFunction(T);
	}
	return MISS;
}

float MyObject::specular(glm::vec3 & npe, glm::vec3 & pe, float & th, Light & light, spSet &sp)
{
	float S = 0;
	if (th > 0)
	{
		glm::vec3 ph = pe + th * npe;
		glm::vec3 n ;
		getNormal(ph, n, sp);
		glm::vec3 nL = normalize(light.position - ph);
		if (light.type == SPOT && glm::dot(nL, light.direction) < cos(light.theta))
		{
			return 0;
		}
		if (light.type == DIR)
			nL = normalize(light.direction);
		glm::vec3 R = -1.0f * nL + 2.0f *(dot(nL, n)*n);
		S = glm::dot(R, -npe);
		S = (S + 1) / 2.0f;
		return specularFunction(S);
	}
	return MISS;
}



float MyObject::fresnel(glm::vec3 ph, MyObject** obj, int objListLen, Ray inCome, FresnelSet &fs)
{
	using namespace glm;
	spSet ss;
	vec3 normal;
	this->getNormal(ph, normal, ss);
	vec3 E = normalize(-inCome.v);
	vec3 Re = -1.0f *E + 2.0f * dot(normal, E)*normal;
	float th = INFINITE;
	int hitObj = -1;
	for (int i = 1; i < objListLen; i++)
	{
		float thC;
		thC = obj[i]->hit(Re, ph, ss);
		if (thC > 0 && th > thC)
		{
			th = thC;
			hitObj = i;
		}
	}
	glm::vec3 rfPh = ph + Re * th;
	if (hitObj == -1)
	{
		hitObj = objListLen-1; //if doesn't hit anythin, hit environment sphere
		rfPh = Re;
	}
	obj[hitObj]->textureMapping(rfPh, fs.color, ss);
	/*fs.color = obj[hitObj]->color;*/
	return 0.0f;
}

float MyObject::specularFunction(float S)
{
	float S_ret=0;
	glm::vec2 p0 = glm::vec2(0, 0);
	glm::vec2 p1 = glm::vec2(0.8, 0.1);
	glm::vec2 p2 = glm::vec2(0.99, 1);
	if (S < p1[0])
		S_ret = (S - p0[0]) /(p1[0] - p0[0]) * (p1[1]-p0[1]) + p0[1];
	if (S < p2[0] && S >= p1[0])
		S_ret = (S - p1[0]) / (p2[0] - p1[0]) * (p2[1] - p1[1]) + p1[1];
	if (S >= p2[0])
		S_ret = (S - p2[0]) / (1 - p2[0]) * (1 - p2[1]) + p2[1];
	return S_ret;
}

float MyObject::diffuseFunction(float T)
{
	float T_ret = 0;
	glm::vec2 p0 = glm::vec2(0, 0);
	glm::vec2 p1 = glm::vec2(0.5, 0.5);
	glm::vec2 p2 = glm::vec2(0.99, 1);
	if (T < p1[0])
		T_ret = (T - p0[0]) / (p1[0] - p0[0]) * (p1[1] - p0[1]) + p0[1];
	if (T < p2[0] && T >= p1[0])
		T_ret = (T - p1[0]) / (p2[0] - p1[0]) * (p2[1] - p1[1]) + p1[1];
	if (T >= p2[0])
		T_ret = (T - p2[0]) / (1 - p2[0]) * (1 - p2[1]) + p2[1];
	return T_ret;
}

float MyObject::shadowFunction(float T_s)
{
	float T_ret = 0;
	glm::vec2 p0 = glm::vec2(0.0, 0.7);
	glm::vec2 p1 = glm::vec2(0.2, -0.2);
	glm::vec2 p2 = glm::vec2(0.25, 1);
	if (T_s < p1[0])
		T_ret = (T_s - p0[0]) / (p1[0] - p0[0]) * (p1[1] - p0[1]) + p0[1];
	if (T_s < p2[0] && T_s >= p1[0])
		T_ret = (T_s - p1[0]) / (p2[0] - p1[0]) * (p2[1] - p1[1]) + p1[1];
	if (T_s >= p2[0])
		T_ret = (T_s - p2[0]) / (1 - p2[0]) * (1 - p2[1]) + p2[1];
	return T_ret;
}

