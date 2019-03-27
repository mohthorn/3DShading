#include "pch.h"
#include "Object.h"

float MyObject::diffuse(glm::vec3 & npe, glm::vec3 & pe, float & th, Light & light)
{
	float T = 0;
	if (th > 0)
	{
		glm::vec3 ph = pe + th * npe;
		glm::vec3 n;
		getNormal(ph, n);
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

float MyObject::specular(glm::vec3 & npe, glm::vec3 & pe, float & th, Light & light)
{
	float S = 0;
	if (th > 0)
	{
		glm::vec3 ph = pe + th * npe;
		glm::vec3 n ;
		getNormal(ph, n);
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

float MyObject::textureMapping(ImageData & img, glm::vec3 & ph, glm::vec3 & p0, glm::vec3 & nt0, glm::vec3 & nt1, float s0, float s1, glm::vec3 & ret_color)
{
	float x = dot(1.0f / s0 * nt0, ph - p0);
	float y = dot(1.0f / s1 * nt1, ph - p0);
	float u = x - (int)x;
	if (x < 0)
		u = 1 + u;
	float v = y - (int)y;
	if (y < 0)
		v = 1 + v;
	float width = img.getWidth();
	float height = img.getHeight();

	int u_i = floor(u*width);
	int v_i = floor(v*height);
	if (u_i >= width)
		u_i = width - 1;
	if (v_i >= height)
		v_i = height - 1;
	u_i = width-1 - u_i;

	ColorRGBA clr= img.getRGBA(u_i, v_i);

	ret_color = glm::vec3(clr.r*255,clr.g*255,clr.b*255);
	return 0.0f;
}

float MyObject::specularFunction(float S)
{
	float S_ret=0;
	glm::vec2 p0 = glm::vec2(0, 0);
	glm::vec2 p1 = glm::vec2(0.98, 0.1);
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
