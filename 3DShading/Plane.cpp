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
	glm::vec3 intersection = light.position + npl * lh;
	ret = glm::length(intersection - ph);
	return lh;
}

float Plane::textureMapping(glm::vec3& ph, glm::vec3 &p0, glm::vec3 &nt0, glm::vec3 &nt1, float s0, float s1, glm::vec3 &ret_color)
{
	float x = dot(1.0f / s0 * nt0, ph - p0);
	float y = dot(1.0f / s1 * nt1, ph - p0);
	float u = x - (int)x;
	if (x < 0)
		u = 1 + u;
	float v = y - (int)y;
	if (y < 0)
		v = 1 + v;
	float width = texture->getWidth();
	float height = texture->getHeight();

	int u_i = floor(u*width);
	int v_i = floor(v*height);
	if (u_i >= width)
		u_i = width - 1;
	if (v_i >= height)
		v_i = height - 1;
	if (u_i < 0)
		u_i = 0;
	if (v_i < 0)
		v_i = 0;
	u_i = width - 1 - u_i;

	ColorRGBA clr = texture->getRGBA(u_i, v_i);

	ret_color = glm::vec3(clr.r * 255, clr.g * 255, clr.b * 255);
	return 0.0f;
}

float Plane::getNormal(glm::vec3 &ph, glm::vec3 &normal)
{
	normal = n0;
	return 0.0f;
}
