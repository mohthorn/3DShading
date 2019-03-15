#pragma once
#include<glm/glm.hpp>
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

