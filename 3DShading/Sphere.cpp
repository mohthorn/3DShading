#include "pch.h"
#include "Sphere.h"




float Sphere::hit(vec3 npe, vec3 pe)
{
	float b = fabs(dot(npe, p0 - pe));
	float c = dot(p0 - pe, p0 - pe) - r * r;
	float c2 = dot(p0 - pe, p0 - pe) - 1.01* r * r;
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

float Sphere::diffuse(glm::vec3 npe, glm::vec3 pe, glm::vec3 L, float theta)
{
	double T = 0;
	float th = 0;
	if ((th=hit(npe, pe))>0)
	{
		vec3 ph = pe + th*npe;
		vec3 n = normalize(ph - p0);
		vec3 nL = normalize(L - ph);
		T = dot(nL, n);
		T = (T + 1) / 2.0;
		return diffuseFunction(T);
	}
	return MISS;
}
float Sphere::specular(glm::vec3 npe, glm::vec3 pe, glm::vec3 L, float theta)
{
	double S = 0;
	float th = 0;
	if ((th = hit(npe, pe)) > 0)
	{
		vec3 ph = pe + th * npe;
		vec3 n = normalize(ph - p0);
		vec3 nL = normalize(L - ph);
		vec3 R = -1.0f * nL + 2.0f *(dot(nL, n)*n);
		S = dot(R, -npe);
		S = (S + 1) / 2.0;
		return specularFunction(S);
	}
	return MISS;
}
Sphere::Sphere()
{
}

Sphere::Sphere(float nr, glm::vec3 ncolor, glm::vec3 np0)
{
	r = nr;
	color = ncolor;
	p0 = np0;
}


Sphere::~Sphere()
{
}
