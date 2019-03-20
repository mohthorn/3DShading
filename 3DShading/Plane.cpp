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
	this->color_dark = 0.1f * color ;
}

float Plane::diffuse(glm::vec3 &npe, glm::vec3 &pe, float &th, Light& light)
{
	double T = 0;
	if (th  > 0)
	{
		vec3 ph = pe + th * npe;
		vec3 n = n0;
		vec3 nL = normalize(light.position - ph);

		if (light.type == SPOT && dot(nL, light.direction) < cos(light.theta))
		{
			return 0;
		}
		if (light.type == DIR)
			nL = normalize(light.direction);
		T = dot(nL, n);
		T = (T + 1) / 2.0;
		return diffuseFunction(T);
	}
	return MISS;
}

float Plane::specular(glm::vec3 &npe, glm::vec3 &pe, float &th, Light& light)
{
	double S = 0;
	if (th > 0)
	{
		vec3 ph = pe + th * npe;
		vec3 n = n0;
		vec3 nL = normalize(light.position - ph);
		if (light.type == SPOT && dot(nL, light.direction) < cos(light.theta))
		{
			return 0;
		}
		if (light.type == DIR)
			nL = normalize(light.direction);
		vec3 R = -1.0f * nL + 2.0f *(dot(nL, n)*n);
		S = dot(R, -npe);
		S = (S + 1) / 2.0;
		return specularFunction(S);
	}
	return MISS;
}

float Plane::shadowLength(glm::vec3 & npl, Light light, float & ret)
{
	return 0.0f;
}
