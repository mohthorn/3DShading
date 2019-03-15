// 3DShading.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

int Main_Window;


#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 900
UCHAR pixels[SCREEN_WIDTH*SCREEN_HEIGHT * 3] = { 0 };

static void init(void)
{
	glClearColor(1, 1, 1, 1); // Set background color.
}

static void windowResize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, (w / 2), 0, (h / 2), 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

static void windowDisplay(void)
{

	glClear(GL_COLOR_BUFFER_BIT);
	glRasterPos2i(0, 0);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	Camera cam;
	MyObject * objList[10]; //allowing a fixed amount of objects
	vec3 LightSource = vec3(-100, 100, -100);
	double eps = 1e-5;
	//###############setting objects##############//
	vec3 sphereColor = vec3(255, 0, 255);
	vec3 planeColor = vec3(0, 255, 0);
	vec3 center = vec3(0, 500, 0);
	Sphere sp1(200, sphereColor, center);
	Plane pl1(vec3(0, -1, 0), vec3(0, 500, 0), planeColor);
	int objNum = 0;


	//############setting camera##############//
	cam.p = vec3(0, 400, -400);
	cam.v0 = center - cam.p;
	cam.vUp = vec3(0, 0, 1);
	cam.sx = 10;
	cam.sy = 10;
	cam.d = 5;

	vec3 sCenter = cam.p + normalize(cam.v0) * (float)cam.d;

	vec3 n2 = normalize(cam.v0);
	vec3 n0 = normalize(glm::cross(n2, cam.vUp));
	vec3 n1 = glm::cross(n0, n2);
	vec3 s0 = sCenter - n0 * (float)(cam.sx / 2.0) - n1 * (float)(cam.sy / 2.0);



	printf("%lf ", sp1.r);
	printf("%lf,%lf,%lf\n", sp1.p0[0], sp1.p0[1], sp1.p0[2]);

	memset(pixels, 0, SCREEN_WIDTH*SCREEN_HEIGHT * 3);
	objList[objNum++] = &sp1;
	objList[objNum++] = &pl1;



	for (int j = SCREEN_HEIGHT - 1; j >= 0; j--)
		for (int i = 0; i < SCREEN_WIDTH; i++)
		{
			vec3 drawColor = vec3 (0,0,0);
			float ix = i * 1.0 / SCREEN_WIDTH;
			float jy = j * 1.0 / SCREEN_HEIGHT;
			int k = (j * SCREEN_WIDTH + i) * 3;
			//if (fabs(ix - 0.5) < 0.0001 && fabs(jy - 0.5) < 0.0001)
			//{
			//	int a = 0;
			//}
			vec3 npe = s0 + n0 * ix * cam.sx + n1 * jy *cam.sy;
			npe = npe - cam.p;
			npe = normalize(npe);

			float th = 1e9;
			int objDrawn = -1;
			for (int oo = 0; oo < objNum; oo++)
			{
				float cur_th = objList[oo]->hit(npe, cam.p);
				if (cur_th < th && (cur_th>0 || fabs(cur_th - BOUNDARY) < eps))
				{
					th = cur_th;
					objDrawn = oo;
				}
			}

			if (fabs(th - FAILCODE) < 1e-5)
			{
				printf("error, camera inside the shape\r\n");
				exit(0);
				//return FAILCODE;
			}

			if (objDrawn>=0)
			{
				
				float T = objList[objDrawn]->diffuse(npe, cam.p, LightSource, 0);
				float S = objList[objDrawn]->specular(npe, cam.p, LightSource,0);
				drawColor = T* objList[objDrawn]->color + (1-T)*vec3(0,0,0);
				drawColor = S * vec3(255, 255, 255) + (1 - S) *drawColor;
				if (fabs(th - BOUNDARY) < eps)
					drawColor = vec3(255, 0, 0);

			}

			pixels[k] = drawColor[0];
			pixels[k + 1] = drawColor[1];
			pixels[k + 2] = drawColor[2];
		}
	glDrawPixels(SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	glFlush();
}

int main(int argc, char *argv[])
{

	//camera1 = camera(glm::vec3(0, 0, 10), glm::vec3(0, 0.4, 0), glm::vec3(0, 1, 0), 45);

	glutInit(&argc, argv);      // intialize glut package
	glutInitWindowPosition(100, 100); // Where the window will display on-screen.
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE); // create single buffer RGB window
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	Main_Window = glutCreateWindow("glsl");
	glutReshapeFunc(windowResize);
	glutDisplayFunc(windowDisplay);
	//glutMouseFunc(handleButtons);
	//glutMotionFunc(handleMotion);
	//glutMouseWheelFunc(handlewheel);

	glewInit();
	if (glewIsSupported("GL_VERSION_4_3"))
	{
		printf("Ready for OpenGL 4.3\n");
	}
	else {
		printf("OpenGL 4.3 not supported\n");
		exit(1);
	}

	init();
	glutMainLoop();  // enter main loop waiting for events

	return 0;
}


