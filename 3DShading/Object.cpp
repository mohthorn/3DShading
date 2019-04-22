#include "pch.h"
#include "Object.h"

float MyObject::diffuse(glm::vec3 & npe, glm::vec3 & pe, float & th, Light & light, spSet &sp)
{
	float T = 0;
	if (th > 0)
	{
		glm::vec3 ph = pe + th * npe;
		glm::vec3 n;
		getNormal(ph, n, sp);
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

float MyObject::specular(glm::vec3 & npe, glm::vec3 & pe, float & th, Light & light, spSet &sp)
{
	float S = 0;
	if (th > 0)
	{
		glm::vec3 ph = pe + th * npe;
		glm::vec3 n ;
		getNormal(ph, n, sp);
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



float MyObject::fresnel(glm::vec3 ph, MyObject** obj, int objListLen, Ray inCome, FresnelSet &fs, spSet ss, glm::vec3 distort)
{
	using namespace glm;
	fs.energy *= (1.0 - transparency)*0.95;

	vec3 normal;
	this->getNormal(ph, normal, ss);

	vec3 E = normalize(-inCome.v);
	vec3 Re = -1.0f *E + 2.0f * dot(normal, E)*normal;
	Re += distort;
	Re = normalize(Re);
	float rtTh[2] = { INFINITE,INFINITE };
	int rtHitObj[2] = { -1,-1 };

	//float yeta = 2.0 / 3.0;
	float C = dot(normal, E);
	float F = fresnelFunction(1 - C);
	F = 1;
	vec3 T = (-1.0f / yeta) * E + (C / yeta - sqrt((yeta*yeta - 1) / yeta * yeta + 1))*normal;
	T += distort;
	T = normalize(T);
	spSet stf[2];
	
	vec3 NR[2] = { Re, T };

	for (int i = 0; i < objListLen; i++)
	{	
		for (int j = 0; j < 2; j++)
		{
			float thC;
			thC = obj[i]->hit(NR[j], ph, stf[j]);
			if (thC > eps && rtTh[j] > thC)
			{
				rtTh[j] = thC;
				rtHitObj[j] = i;
			}
		}
	}
	
	vec3 rtPh[2] ;


	for (int j = 0; j < 2; j++)
	{
		rtPh[j] = ph + NR[j] * rtTh[j];
		if (rtHitObj[j] == -1)
		{
			rtHitObj[j] = objListLen - 1; //if doesn't hit anythin, hit environment sphere
			rtPh[j] = NR[j];
		}
	}

	FresnelSet rtFr[2];
	glm::vec3 selfColor;

	this->textureMapping(ph, selfColor, ss);
	float nf[2] = { F, 1 - F };

	if (fs.energy < ENERGYTHRESHOLD)
	{
		for (int j = 0; j < 2; j++)
		{
			obj[rtHitObj[j]]->textureMapping(rtPh[j], rtFr[j].color, stf[j]);
		}
		vec3 selfColor;
		this->textureMapping(ph, selfColor, ss);
		fs.color = (1 - transparency)* (F * rtFr[0].color + (1 - F) * rtFr[1].color) + transparency * selfColor;
		return 0;
	}

	for (int j = 0; j < 2; j++)
	{
		Ray splitted;
		splitted.v = NR[j];
		splitted.p = rtPh[j];
		rtFr[j].energy = nf[j] * fs.energy;
		/*obj[rtHitObj[j]]->textureMapping(rtPh[j], rtFr[j].color, stf[j]);*/
		obj[rtHitObj[j]]->fresnel(rtPh[j], obj, objListLen, splitted, rtFr[j], stf[j], distort);
	}

	fs.color =(1-transparency)* ( F * rtFr[0].color + (1 - F) * rtFr[1].color) + transparency* selfColor ;
	if (IRR)
	{
		int irrLen = floor(C* 1.0 * irrMap->getWidth());
		if (irrLen > irrMap->getWidth() - 1)
			irrLen = irrMap->getWidth() - 1;
		if (irrLen < 0)
			irrLen = 0;
		ColorRGBA ic = irrMap->getRGBA(irrLen, 0);
		vec3 irrColor = vec3(ic.r *255, ic.g*255,ic.b*255);
		fs.color = (1-0.1f* (1 - C))* fs.color + 0.1f* (1-C) * irrColor;
	}
	return 0.0f;
}

float MyObject::shadowCast(glm::vec3 ph, MyObject ** obj, int objListLen, Light LS, spSet ss)
{
	using namespace glm;
	float T_s = 0;
	int spot_flag = 0;
	float d = 20;


	vec3 n;
	this->getNormal(ph, n, ss);
	vec3 ph_d = ph - d * n;
	vec3 pl = ph_d - LS.position;
	vec3 npl = normalize(pl);


	if (LS.type == DIR)
	{
		npl = normalize(-LS.direction);
		LS.position = ph_d - 10000.0f * npl;
		pl = ph_d - LS.position;
	}
	if (LS.type == SPOT)
	{
		vec3 nL = normalize(LS.position - ph_d);
		if (dot(nL, LS.direction) < cos(LS.theta))
		{
			spot_flag = 1;
		}
	}
	float totalLength = 0;

	float costheta = 1;

	if (!spot_flag)
	{
		for (int oo = 0; oo < objListLen; oo++)
		{

			float slen = 0;
			float lh = obj[oo]->shadowLength(npl, LS, slen, ph_d);
			if (lh < glm::length(pl) && fabs(lh - glm::length(pl)) > 0.1 && lh > 0)
			{
				totalLength = totalLength + slen;
			}

			//if (oo == objDrawn)
			//{
			//	float r = glm::length(LL.position + lh * npl - ph_d);
			//	costheta = d * 1.0 / r;
			//	costheta = (costheta + 1) / 2.0;
			//}
		}

		T_s = d / (totalLength/* * costheta*/);
	}
	//if (objDrawn == 0)
	//	T_s = objList[objDrawn]->shadowFunction(T_s);
	if (T_s > 1)
		T_s = 1;
	if (T_s < 0)
		T_s = 0;
	return T_s;
}

float MyObject::specularFunction(float S)
{
	float S_ret=0;
	glm::vec2 p0 = glm::vec2(0, 0);
	glm::vec2 p1 = glm::vec2(0.8, 0.1);
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

float MyObject::fresnelFunction(float F)
{
	float F_ret = 0;
	glm::vec2 p0 = glm::vec2(0.0, 0.2);
	glm::vec2 p1 = glm::vec2(0.3, 0.1);
	glm::vec2 p2 = glm::vec2(0.8, 1);
	if (F < p1[0])
		F_ret = (F - p0[0]) / (p1[0] - p0[0]) * (p1[1] - p0[1]) + p0[1];
	if (F < p2[0] && F >= p1[0])
		F_ret = (F - p1[0]) / (p2[0] - p1[0]) * (p2[1] - p1[1]) + p1[1];
	if (F >= p2[0])
		F_ret = (F - p2[0]) / (1 - p2[0]) * (1 - p2[1]) + p2[1];
	return F_ret;
}

