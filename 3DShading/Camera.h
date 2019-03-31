#pragma once
#include<glm/glm.hpp>
#include "ImageData.h"
class Camera
{
public:
	glm::vec3 p;
	glm::vec3 v0;
	glm::vec3 vUp;
	float d;
	float sx;
	float sy;
	Camera();
	~Camera();
};

class Projector:
	public Camera
{
public:
	ImageData * texture;
	Projector(Camera cam);
};