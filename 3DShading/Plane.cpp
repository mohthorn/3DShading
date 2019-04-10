#include "pch.h"
#include "Plane.h"

float Plane::hit(glm::vec3 npe, glm::vec3 pe, spSet &sp)
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
	normalMap = NULL;
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

float Plane::textureMapping(glm::vec3& ph , glm::vec3 &ret_color, spSet &sp)
{
	float x = dot(1.0f / s0 * nt0, ph - txP0);
	float y = dot(1.0f / s1 * nt1, ph - txP0);
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

float Plane::getNormal(glm::vec3 &ph, glm::vec3 &normal, spSet &sp)
{
	normal = n0;
	return 0.0f;
}

ObjFromFile::ObjFromFile(char * filename)
{
	// **************Opening and Reading File from HTMLParser
	HANDLE hFile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);
	// process errors
	if (hFile == INVALID_HANDLE_VALUE)
	{
		printf("CreateFile failed with %d\n", GetLastError());
		exit;
	}

	// get file size
	LARGE_INTEGER li;
	BOOL bRet = GetFileSizeEx(hFile, &li);
	// process errors
	if (bRet == 0)
	{
		printf("GetFileSizeEx error %d\n", GetLastError());
		exit;
	}

	// read file into a buffer
	int fileSize = (DWORD)li.QuadPart;			// assumes file size is below 2GB; otherwise, an __int64 is needed
	DWORD bytesRead;
	// allocate buffer
	char *fileBuf = new char[fileSize];
	// read into the buffer
	bRet = ReadFile(hFile, fileBuf, fileSize, &bytesRead, NULL);

	// process errors
	if (bRet == 0 || bytesRead != fileSize)
	{
		printf("ReadFile failed with %d\n", GetLastError());
		exit;
	}

	// done with the file
	CloseHandle(hFile);
	//*****************************
	vertices = new glm::vec3[INITIAL_BUFFER];
	texturePoints = new glm::vec2[INITIAL_BUFFER];
	normalPoints = new glm::vec3[INITIAL_BUFFER];
	tuples = new tuple<int, int, int>[INITIAL_BUFFER];
	texTuple = new tuple<int, int, int>[INITIAL_BUFFER];
	normTuple = new tuple<int, int, int>[INITIAL_BUFFER];
	vNum = 0;
	tNum = 0;
	texNum = 0;
	normNum = 0;
	fileBuf[fileSize] = 0;
	printf("Opened %s with size %d\n", filename, fileSize);
	char * pStart = fileBuf;
	char * pEnd = pStart;

	while ((pEnd = strchr(pStart, '\n')) != NULL)
	{
		*pEnd = 0;
		if (*pStart == 'v')
		{	
			if (*(pStart + 1) == ' ')
			{
				/*printf("%s\n", pStart);*/
				float x, y, z;
				sscanf(pStart + 1, "%f%f%f", &x, &y, &z);
				vertices[vNum++] = glm::vec3(x, y, z);
			}
			if (*(pStart + 1) == 't')
			{
				float x, y, z;
				sscanf(pStart + 2, "%f%f", &x, &y);
				texturePoints[texNum++] = glm::vec2(x, y);
			}
			if (*(pStart + 1) == 'n')
			{
				float x, y, z;
				sscanf(pStart + 2, "%f%f%f", &x, &y, &z);
				normalPoints[normNum++] = glm::vec3(x, y, z);
			}
		}
		if (*pStart == 'f')
		{
			/*printf("%s\n", pStart);*/
			int x[3][3];

			pStart++;
			for (int i = 0; i < 3; i++)
			{
				while (*pStart == ' ')
					pStart++;
				char* slash = strchr(pStart, '/');
				if(slash !=NULL)
					sscanf(pStart, "%d/%d/%d", &x[i][0], &x[i][1], &x[i][2]);
				else
					sscanf(pStart, "%d%d%d", &x[i][0], &x[i][1], &x[i][2]);
				pStart = strchr(pStart, ' ');
			}
			texTuple[tNum] = make_tuple(x[0][1], x[1][1], x[2][1]);
			normTuple[tNum] = make_tuple(x[0][2], x[1][2], x[2][2]);
			tuples[tNum++] = make_tuple(x[0][0], x[1][0], x[2][0]);
			
		}
		pStart = pEnd + 1;
		while (*pStart == '\n')
			pStart++;	
	}

	for (int i = 0; i < vNum; i++)
	{
		printf("%f, %f, %f\n", vertices[i].x, vertices[i].y,vertices[i].z);
	}

	for (int i = 0; i < tNum; i++)
	{
		printf("%d, %d, %d\n", get<0>(tuples[i]), get<1>(tuples[i]), get<2>(tuples[i]));
	}
	for (int i = 0; i < texNum; i++)
	{
		printf("%f, %f\n", texturePoints[i].x, texturePoints[i].y);
	}
	for (int i = 0; i < normNum; i++)
	{
		printf("%f, %f, %f\n", normalPoints[i].x, normalPoints[i].y, normalPoints[i].z);
	}
	color = glm::vec3(255, 255, 0);
	color_dark = 0.1f*color;
	color_specular= glm::vec3(255, 255, 255);
}

float ObjFromFile::hit(glm::vec3 npe, glm::vec3 pe, spSet &sp)
{
	using namespace glm;
	float th_min = 1e8;
	int triDrawn;
	float eps = 1e-4;
	bool isHit=FALSE;
	vec3 v0, v1, v2;
	for (int i = 0; i < tNum; i++)
	{
		v0 = vertices[get<0>(tuples[i%tNum])-1];
		v1 = vertices[get<1>(tuples[i%tNum])-1];
		v2 = vertices[get<2>(tuples[i%tNum])-1];

		vec3 v01 = v1 - v0;
		vec3 v12 = v2 - v1;

		vec3 nt0 = normalize(cross(v01, v12));
		
		Plane pt(nt0, v0, color);
		float th = pt.hit(npe, pe,sp);
		
		if (th > 0)
		{
			vec3 ph = pe + th * npe;

			vec3 A = cross((v1 - v0), (v2 - v0));
			vec3 A1 = cross((ph - v2), (ph - v0));
			vec3 A2 = cross((ph - v0), (ph - v1));

			int nonZero = 0;
			for (int j = 0; j < 3; j++)
			{
				if (fabs(A[j]) > eps)
					nonZero = j;
			}
			float u = A1[nonZero] / A[nonZero];
			float v = A2[nonZero]/ A[nonZero];
			if (u >= 0 && v >= 0 && (1 - u - v) >= 0 && u <= 1 && v <= 1 && (1 - u - v) <= 1)
				if (th < th_min)
				{
					isHit = TRUE;
					th_min = th;
					sp.hitNum = i;
					sp.u = u;
					sp.v = v;
					triDrawn = tNum;
				}
		}
	}
	if (isHit)
	{
		return th_min;
	}
		
	return MISS;
}

float ObjFromFile::shadowLength(glm::vec3 & npl, Light light, float & ret, glm::vec3 ph)
{
	return 0.0f;
}

float ObjFromFile::textureMapping(glm::vec3 & ph, glm::vec3 & ret_color, spSet &sp)
{
	//if (get<0>(texTuple[sp.hitNum%tNum]) == 3)
	//{
	//	int a = 0;
	//}
	//glm::vec2 v0 = texturePoints[get<0>(texTuple[sp.hitNum%tNum]) - 1];
	//glm::vec2 v1 = texturePoints[get<1>(texTuple[sp.hitNum%tNum]) - 1];
	//glm::vec2 v2 = texturePoints[get<2>(texTuple[sp.hitNum%tNum]) - 1];

	//glm::vec2 tp = (1-sp.u-sp.v)*v0+ v1* sp.u +  v2* sp.v;
	//int width = texture->getWidth();
	//int height = texture->getHeight();
	//int u_i = floor(width* tp.x);
	//int v_i = floor(height * tp.y);
	//if (u_i >= width)
	//	u_i = width - 1;
	//if (v_i >= height)
	//	v_i = height - 1;
	//if (u_i < 0)
	//	u_i = 0;
	//if (v_i < 0)
	//	v_i = 0;
	//ColorRGBA clr = texture->getRGBA((int)u_i, (int)v_i);
	//ret_color = glm::vec3(clr.r*255, clr.g*255, clr.b*255);
	return 0.0f;
}

float ObjFromFile::getNormal(glm::vec3 & ph, glm::vec3 & normal, spSet &sp)
{
	glm::vec3 n0 = vertices[get<1>(tuples[sp.hitNum%tNum])-1] - vertices[get<0>(tuples[sp.hitNum%tNum])-1];
	glm::vec3 n1 = vertices[get<2>(tuples[sp.hitNum%tNum])-1] - vertices[get<1>(tuples[sp.hitNum%tNum])-1];
	normal = cross(n0, n1);
	normal = normalize(normal);
	return 0.0f;
}
