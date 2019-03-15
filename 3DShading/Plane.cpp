#include "pch.h"
#include "Plane.h"

float Plane::hit(glm::vec3 npe, glm::vec3 pe)
{
	if (dot(n0, p0 - pe) > 0)
	{
		return FAILCODE;
	}
	float th = dot(n0, p0 - pe);
	float th_p = (dot(n0, npe));

	return th/th_p;
}

Plane::Plane(glm::vec3 n0, glm::vec3 p0, glm::vec3 color)
{
	this->n0 = normalize(n0);
	this->p0 = p0;
	this->color = color;
}

float Plane::diffuse(glm::vec3 npe, glm::vec3 pe, glm::vec3 L, float theta)
{
	double T = 0;
	float th = 0;
	if ((th = hit(npe, pe)) > 0)
	{
		vec3 ph = pe + th * npe;
		vec3 n = n0;
		vec3 nL = normalize(L - ph);
		T = dot(nL, n);
		T = (T + 1) / 2.0;
		return diffuseFunction(T);
	}
	return MISS;
}

float Plane::specular(glm::vec3 npe, glm::vec3 pe, glm::vec3 L, float theta)
{
	double S = 0;
	float th = 0;
	if ((th = hit(npe, pe)) > 0)
	{
		vec3 ph = pe + th * npe;
		vec3 n = n0;
		vec3 nL = normalize(L - ph);
		vec3 R = -1.0f * nL + 2.0f *(dot(nL, n)*n);
		S = dot(R, -npe);
		S = (S + 1) / 2.0;
		return specularFunction(S);
	}
	return MISS;
}
