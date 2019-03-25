#include "pch.h"
#include "Object.h"

float MyObject::diffuse(glm::vec3 & npe, glm::vec3 & pe, float & th, Light & light)
{
	float T = 0;
	if (th > 0)
	{
		vec3 ph = pe + th * npe;
		vec3 n;
		getNormal(ph, n);
		vec3 nL = normalize(light.position - ph);
		if (light.type == SPOT && dot(nL, light.direction) < cos(light.theta))
		{
			return 0;
		}
		if (light.type == DIR)
			nL = normalize(light.direction);
		T = dot(nL, n);
		T = (T + 1) / 2.0f;
		return diffuseFunction(T);
	}
	return MISS;
}

float MyObject::specular(glm::vec3 & npe, glm::vec3 & pe, float & th, Light & light)
{
	float S = 0;
	if (th > 0)
	{
		vec3 ph = pe + th * npe;
		vec3 n ;
		getNormal(ph, n);
		vec3 nL = normalize(light.position - ph);
		if (light.type == SPOT && dot(nL, light.direction) < cos(light.theta))
		{
			return 0;
		}
		if (light.type == DIR)
			nL = normalize(light.direction);
		vec3 R = -1.0f * nL + 2.0f *(dot(nL, n)*n);
		S = dot(R, -npe);
		S = (S + 1) / 2.0f;
		return specularFunction(S);
	}
	return MISS;
}

float MyObject::specularFunction(float S)
{
	float S_ret=0;
	vec2 p0 = vec2(0, 0);
	vec2 p1 = vec2(0.92, 0.1);
	vec2 p2 = vec2(0.95, 0.98);
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
	vec2 p0 = vec2(0, 0);
	vec2 p1 = vec2(0.8, 0.15);
	vec2 p2 = vec2(0.99, 1);
	if (T < p1[0])
		T_ret = (T - p0[0]) / (p1[0] - p0[0]) * (p1[1] - p0[1]) + p0[1];
	if (T < p2[0] && T >= p1[0])
		T_ret = (T - p1[0]) / (p2[0] - p1[0]) * (p2[1] - p1[1]) + p1[1];
	if (T >= p2[0])
		T_ret = (T - p2[0]) / (1 - p2[0]) * (1 - p2[1]) + p2[1];
	return T_ret;
	return 0.0f;
}
