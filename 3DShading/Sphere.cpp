#include "pch.h"
#include "Sphere.h"




inline float Sphere::hit(glm::vec3 npe, glm::vec3 pe, spSet &sp)
{
	float b = fabs(dot(npe, p0 - pe));
	float c = dot(p0 - pe, p0 - pe) - r * r;
	float c2 = dot(p0 - pe, p0 - pe) - 1.00f* r * r;
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



inline float Sphere::shadowLength(glm::vec3 & npl, Light light, float &ret, glm::vec3 ph)
{
	glm::vec3 pl = p0 - light.position;
	glm::vec3 n = glm::normalize(ph - p0);
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

inline float Sphere::getNormal(glm::vec3 &ph, glm::vec3 &normal, spSet &sp)
{
	using namespace glm;
	normal = glm::normalize(ph - p0);
	if(normalMap == NULL)
		return 0.0f;
	else
	{
		glm::vec3 nph = normal;
		float v = acos(nph.z) / PI;
		float cu = nph.y*1.0 / sqrt(1 - nph.z*nph.z);
		float u = acos(cu) / (2.0*PI);
		if (nph.x < 0)
		{
			u = 1 - u;
		}

		v = 1 - v;

		float width = normalMap->getWidth();
		float height = normalMap->getHeight();

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
		/*u_i = width - 1 - u_i;*/
		//float v = nph.z*0.5 + 0.5;
		//float u = nph.x*0.5 + 0.5;

		ColorRGBA clr = normalMap->getRGBA(u_i, v_i);

		vec3 newNormal = glm::normalize(vec3(2*clr.r-1, 2*clr.g-1, 2* clr.b-1));
		
		vec3 nt0 = normalize(northPole);
		vec3 nt2 = normalize(normal);
		vec3 nt1 = normalize(cross(nt2, nt0));
		nt0 = normalize(cross(nt1, nt2));

		normal = normalize(nt1*newNormal.x + nt0*newNormal.y+nt2* newNormal.z);
	}
}

inline float Sphere::textureMapping(glm::vec3& ph, glm::vec3 &ret_color, spSet &sp)
{
	using namespace glm;
	vec3 nt2 = cross(nt0, nt1);
	float x = dot(1.0f / r * nt0, ph - p0);
	float y = dot(1.0f / r * nt1, ph - p0);
	float z = dot(1.0f / r * nt2, ph - p0);
	
	vec3 nph = normalize(vec3(x, y, z));
	float v = acos(nph.z) / PI;
	float cu = nph.y*1.0 / sqrt(1 - nph.z*nph.z);
	float u = acos(cu) / (2.0*PI);
	if (nph.x < 0)
	{
		u = 1 - u;
	}

	v = 1 - v;

	//float v = nph.z*0.5 + 0.5;
	//float u = nph.x*0.5 + 0.5;


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
	if (v_i <0)
		v_i = 0;
	/*u_i = width - 1 - u_i;*/

	ColorRGBA clr = texture->getRGBA(u_i, v_i);

	ret_color = glm::vec3(clr.r * 255, clr.g * 255, clr.b * 255);
	return 0.0f;
}

float Sphere::solidMapping(glm::vec3 & ph, glm::vec3 & ret_color)
{
	using namespace glm;
	vec3 nt2 = normalize(cross(nt0, nt1));
	float x = dot(1.0f / r * nt0, ph - p0);
	float y = dot(1.0f / r * nt1, ph - p0);
	float z = dot(1.0f / r * nt2, ph - p0);

	vec3 nph = normalize(vec3(x, y, z));
	//float v = acos(nph.z) / PI;
	//float cu = nph.y*1.0 / sqrt(1 - nph.z*nph.z);
	//float u = acos(cu) / (2.0*PI);
	//if (nph.x < 0)
	//{
	//	u = 1 - u;
	//}

	//v = 1 - v;

	float v = nph.z*0.5 + 0.5;
	float u = nph.x*0.5 + 0.5;



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
	/*u_i = width - 1 - u_i;*/

	ColorRGBA clr = texture->getRGBA(u_i, v_i);

	ret_color = glm::vec3(clr.r * 255, clr.g * 255, clr.b * 255);
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
	color_specular = 2.0f * ncolor;
	for (int i = 0; i < 3; i++)
	{
		color_specular[i] = std::max(color_specular[i], SPECTHRESHOLD);
		if (color_specular[i] > 255)
			color_specular[i] = 255;
	}
	normalMap = NULL;
}


Sphere::~Sphere()
{
}

inline float IFSphere::hit(glm::vec3 npe, glm::vec3 pe, spSet &sp)
{
	return 0.0f;
}

inline float IFSphere::shadowLength(glm::vec3 & npl, Light light, float & ret, glm::vec3 ph)
{
	return 0.0f;
}

inline float IFSphere::getNormal(glm::vec3 & ph, glm::vec3 & normal, spSet &sp)
{
	normal = normalize(ph);
	return 0.0f;
}

inline float IFSphere::textureMapping(glm::vec3 & ph , glm::vec3 & ret_color, spSet &sp)
{
	using namespace glm;
	vec3 nph = ph;
	float v = acos(nph.z) / PI;
	float cu = nph.y*1.0 / sqrt(1 - nph.z*nph.z);
	float u = acos(cu) / (2.0*PI);
	if (nph.x < 0)
	{
		u = 1 - u;
	}

	v = 1 - v;

	//float v = nph.z*0.5 + 0.5;
	//float u = nph.x*0.5 + 0.5;

	float width = texture->getWidth();
	float height = texture->getHeight();


	int u_i = floor(u*width);
	int v_i = floor(v*height);
	if (u_i >= width)
		u_i = floor(width - 1);
	if (v_i >= height)
		v_i = floor(height - 1);

	if (u_i < 0)
		u_i = 0;
	if (v_i < 0)
		v_i = 0;
	/*u_i = width - 1 - u_i;*/

	ColorRGBA clr = texture->getRGBA(u_i, v_i);

	ret_color = glm::vec3(clr.r * 255, clr.g * 255, clr.b * 255);
	/*ret_color = ret_color*sqrt(2*v-1);*/
	return 0.0f;
}

IFSphere::IFSphere()
{
}

IFSphere::IFSphere(glm::vec3 color)
{
	this->color = color;
	this->color_dark = 0.1f * color;
}

IFSphere::~IFSphere()
{
}
