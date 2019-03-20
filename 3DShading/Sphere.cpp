#include "pch.h"
#include "Sphere.h"




float Sphere::hit(vec3 npe, vec3 pe)
{
	float b = fabs(dot(npe, p0 - pe));
	float c = dot(p0 - pe, p0 - pe) - r * r;
	float c2 = dot(p0 - pe, p0 - pe) - 1.1f* r * r;
	float result = b * b - c;
	if (c < 0)
		return FAILCODE;
	if (result < 0)
	{
		if (b*b - c2 > 0)
			return BOUNDARY;
		return MISS;
	}

	float th = b - sqrt(result);
	return th;
}

float Sphere::diffuse(glm::vec3 &npe, glm::vec3 &pe, float &th, Light& light)
{
	float T = 0;
	if (th>0)
	{
		vec3 ph = pe + th*npe;
		vec3 n = normalize(ph - p0);
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
float Sphere::specular(glm::vec3 &npe, glm::vec3 &pe, float &th, Light& light)
{
	float S = 0;
	if (th > 0)
	{
		vec3 ph = pe + th * npe;
		vec3 n = normalize(ph - p0);
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

float Sphere::shadowLength(glm::vec3 & npl, Light light, float &ret)
{
	vec3 pl = p0 - light.position;
	float b = fabs(dot(npl, pl));
	float c = dot(pl, pl) - r * r;
	float result = b * b - c;
	if (c < 0)
		return FAILCODE;
	if (result < 0)
	{
		return 0;
	}

	float th = b - sqrt(result);
	ret = 2 * sqrt(result);

	return th;
}


Sphere::Sphere()
{
}

Sphere::Sphere(float nr, glm::vec3 ncolor, glm::vec3 np0)
{
	r = nr;
	color = ncolor;
	p0 = np0;
	color_dark = 0.1f * ncolor;
}


Sphere::~Sphere()
{
}
