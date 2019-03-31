#include "pch.h"
#include "Camera.h"


Camera::Camera()
{
}



Camera::~Camera()
{
}

Projector::Projector(Camera &cam, ImageData * texture)
{
	p = cam.p;
	v0 = cam.v0;
	vUp = cam.vUp;
	d = cam.d;
	sx = cam.sx;
	sy = cam.sy;
	this->texture = texture;
}

Projector::Projector(glm::vec3  p, glm::vec3  v0, glm::vec3  vUp, float d, float sx, float sy, ImageData * texture)
{
	this->p = p;
	this->v0 = v0;
	this->vUp = vUp;
	this->d = d;
	this->sx = sx;
	this->sy = sy;
	this->texture = texture;
}

int Projector::projectColor(glm::vec3 &n0, glm::vec3 &n1, glm::vec3 &n2, glm::vec3 &ph, glm::vec3 &drawColor)
{
	glm::vec3 center = p + d * n2;
	glm::vec3 projectPoint = p+ d* (ph - p) / dot(n2, (ph - p));
	float x = dot(n0 / sx, projectPoint - center);
	float y = dot(n1 / sy, projectPoint - center);
	float u = (x + 1) / 2;
	float v = (y + 1) / 2;
	if (u > 1 || u < 0 || v>1 || v < 0)
	{
		return 0;
	}
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

	drawColor = glm::vec3(clr.r * 255, clr.g * 255, clr.b * 255);

	return 1;
}
