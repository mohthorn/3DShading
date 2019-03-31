#include "pch.h"
#include "Camera.h"


Camera::Camera()
{
}



Camera::~Camera()
{
}

Projector::Projector(Camera cam)
{
	p = cam.p;
	v0 = cam.v0;
	vUp = cam.vUp;
	d = cam.d;
	sx = cam.sx;
	sy = cam.sy;
}
