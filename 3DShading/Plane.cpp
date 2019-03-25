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
	color_specular = 1.5f * color;
	for (int i = 0; i < 3; i++)
	{
		color_specular[i] = std::max(color_specular[i], SPECTHRESHOLD);
		if (color_specular[i] > 255)
			color_specular[i] = 255;
	}
}



float Plane::shadowLength(glm::vec3 & npl, Light light, float & ret, glm::vec3 ph)
{
	if (dot(n0, p0 - light.position) > 0)
	{
		return FAILCODE;
	}
	float lh = dot(n0, p0 - light.position);
	float lh_p = (dot(n0, npl));
	lh = lh / lh_p;
	vec3 intersection = light.position + npl * lh;
	ret = glm::length(intersection - ph);
	return lh;
}

float Plane::getNormal(glm::vec3 &ph, glm::vec3 &normal)
{
	normal = n0;
	return 0.0f;
}
