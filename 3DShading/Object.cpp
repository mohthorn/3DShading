#include "pch.h"
#include "Object.h"

float MyObject::specularFunction(float S)
{
	float S_ret=0;
	vec2 p0 = vec2(0, 0);
	vec2 p1 = vec2(0.8, 0.1);
	vec2 p2 = vec2(0.91, 0.95);
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
	vec2 p1 = vec2(0.6, 0.3);
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
