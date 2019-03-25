#include "pch.h"
#include "Sphere.h"




float Sphere::hit(vec3 npe, vec3 pe)
{
	float b = fabs(dot(npe, p0 - pe));
	float c = dot(p0 - pe, p0 - pe) - r * r;
	float c2 = dot(p0 - pe, p0 - pe) - 1.0f* r * r;
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



float Sphere::shadowLength(glm::vec3 & npl, Light light, float &ret, glm::vec3 ph)
{
	vec3 pl = p0 - light.position;
	vec3 n = normalize(ph - p0);
	float b = fabs(dot(npl, pl));
	float c = dot(pl, pl) - r * r;
	float result = b * b - c;
	if (c < 0)
		return FAILCODE;
	if (result < 0)
	{
		return 0;
	}

	float lh = b - sqrt(result);

	ret = 2 * sqrt(result);
	if (glm::length(ph - p0) < r)
		ret = glm::length(ph - light.position) - lh;
	return lh;
}

float Sphere::getNormal(glm::vec3 &ph, glm::vec3 &normal)
{
	normal = normalize(ph - p0);
	return 0.0f;
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
	color_specular = 1.5f * ncolor;
	for (int i = 0; i < 3; i++)
	{
		color_specular[i] = std::max(color_specular[i], SPECTHRESHOLD);
		if (color_specular[i] > 255)
			color_specular[i] = 255;
	}
}


Sphere::~Sphere()
{
}
