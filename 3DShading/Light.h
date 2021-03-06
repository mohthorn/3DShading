#pragma once
#include<glm/glm.hpp>
#define DIR 0
#define POINT 1
#define SPOT 2
#define AREA 3

class Light
{
public:
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 xd;
	float sx;
	float sy;
	float theta;
	char type;
	Light(char type, glm::vec3 position, glm::vec3 direction, float theta);
};

