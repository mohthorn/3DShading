// 3DShading.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#define NONE -1
#define M 1

int Main_Window;
static int MouseX;
static int MouseY;
static int Button = NONE;

#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 900
#define PI 3.14
UCHAR pixels[SCREEN_WIDTH*SCREEN_HEIGHT * 3] = { 0 };

Camera cam;
MyObject * objList[10]; //allowing a fixed amount of objects
vec3 LightSource = vec3(100, 0, 1000);
double eps = 1e-5;
//###############setting objects##############//
vec3 sphereColor = vec3(255, 0, 255);
vec3 planeColor = vec3(0, 255, 0);
vec3 center = vec3(0, 500, 0);
Sphere sp1(200, sphereColor, center);
Plane pl1(vec3(0, -1, 0), vec3(-1000, 1000, -1000), planeColor);
Plane pl2(vec3(1, 0, 0), vec3(-1000, 1000, -1000), vec3(125, 125, 0));
Plane pl3(vec3(0, 0, 1), vec3(-1000, 1000, -1000), vec3(0, 125, 125));
int objNum = 0;
vec3 LD = vec3(0, -1, 0);
float Ltheta = 30.0*PI/180.0;
Light LL(POINT, LightSource, LD, Ltheta);

std::random_device rd;  //Will be used to obtain a seed for the random number engine
std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
std::uniform_real_distribution<> dis(0, 1);


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


	//############setting camera##############//
	cam.p = vec3(MouseX - SCREEN_WIDTH / 2.0, 0, MouseY+ SCREEN_HEIGHT/2.0);
	cam.v0 = center - cam.p;
	cam.vUp = vec3(0, 0, 1);
	cam.sx = 10;
	cam.sy = 10;
	cam.d = 5;
	printf("%lf,%lf,%lf", cam.p[0], cam.p[1], cam.p[2]);

	vec3 sCenter = cam.p + normalize(cam.v0) * (float)cam.d;

	vec3 n2 = normalize(cam.v0);
	vec3 n0 = normalize(glm::cross(n2, cam.vUp));
	vec3 n1 = glm::cross(n0, n2);
	vec3 s0 = sCenter - n0 * (float)(cam.sx / 2.0) - n1 * (float)(cam.sy / 2.0);
	

	printf("%lf ", sp1.r);
	printf("%lf,%lf,%lf\n", sp1.p0[0], sp1.p0[1], sp1.p0[2]);

	for (int j = SCREEN_HEIGHT - 1; j >= 0; j--)
		for (int i = 0; i < SCREEN_WIDTH; i++)
		{	
			double ri = dis(gen);
			double rj = dis(gen);
			int k = (j * SCREEN_WIDTH + i) * 3;
			vec3 colorSum = vec3(0, 0, 0);
			for(int ii=0;ii<M;ii++)
				for (int jj = 0; jj < M; jj++)
				{
					vec3 drawColor = vec3(0, 0, 0);
					float ix = (i + (ii + ri) * 1.0/M) * 1.0 / SCREEN_WIDTH;
					float jy = (j + (jj + rj) * 1.0 /M) * 1.0 / SCREEN_HEIGHT;

					vec3 npe = s0 + n0 * ix * cam.sx + n1 * jy *cam.sy;
					npe = npe - cam.p;
					npe = normalize(npe);

					float th = 1e9;
					int objDrawn = -1;

					//get the closest hit object
					for (int oo = 0; oo < objNum; oo++)
					{
						float cur_th = objList[oo]->hit(npe, cam.p);
						if (cur_th < th && (cur_th > 0 || fabs(cur_th - BOUNDARY) < eps))
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

					if (objDrawn >= 0)
					{
						float T = objList[objDrawn]->diffuse(npe, cam.p, th, LL);
						float S = objList[objDrawn]->specular(npe, cam.p, th, LL);
						drawColor = T * objList[objDrawn]->color + (1 - T)*objList[objDrawn]->color_dark;
						//###############Cast Shadow################//
						vec3 pl = cam.p + th * npe - LL.position;
						vec3 npl = normalize(pl);
						float totalLength = 0;
						float goDown = 20;
						for (int oo = 0; oo < objNum; oo++)
						{
							if (oo != objDrawn)
							{
								float slen;
								float lh = objList[oo]->shadowLength(npl, LL, slen);
								if (lh < glm::length(pl) && lh >0)
								{
									totalLength = totalLength + slen;
								}
							}
						}

						float T_s = goDown / (totalLength+goDown);
						drawColor = T_s * drawColor + (1 - T)*objList[objDrawn]->color_dark;
						//##########################################//
						if(!(totalLength > eps))
							drawColor = S * vec3(255, 255, 255) + (1 - S) *drawColor;
						if (fabs(th - BOUNDARY) < eps)
							drawColor = vec3(255, 0, 0);
					}

					colorSum += drawColor;
				}
			pixels[k] =colorSum[0] / (float)(M*M);
			pixels[k + 1] =colorSum[1] / (float)(M*M);
			pixels[k + 2] =colorSum[2] / (float)(M*M);
		}
	glDrawPixels(SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	glFlush();
}

void handleButtons(int button, int state, int x, int y)
{
	if (state == GLUT_UP)
	{
		button = NONE;
	}
	else
	{
		MouseX = x;
		MouseY = -y;

		Button = button;
	}

}

/*control mouse scroll wheel*/
void handlewheel(int button, int direction, int x, int y)
{
	//camera1.Mouse_Sroll(direction);
	glutPostRedisplay();
}

/*(mouse motion call back) when mouse moves with a button down, update approriate camera parameters*/
void handleMotion(int x, int y)
{
	y = -y;

	int dy = y - MouseY;
	int dx = x - MouseX;

	switch (Button) {
	case GLUT_LEFT_BUTTON:
		if (glutGetModifiers() == GLUT_ACTIVE_ALT)
		{
			//camera1.Mouse_Motion(dx, dy);
			glutPostRedisplay();
			break;
		}
	case GLUT_MIDDLE_BUTTON:
		break;
	}

	MouseX = x;
	MouseY = y;
}

int main(int argc, char *argv[])
{

	//camera1 = camera(glm::vec3(0, 0, 10), glm::vec3(0, 0.4, 0), glm::vec3(0, 1, 0), 45);
	objList[objNum++] = &sp1;
	objList[objNum++] = &pl1;
	objList[objNum++] = &pl2;
	objList[objNum++] = &pl3;
	glutInit(&argc, argv);      // intialize glut package
	glutInitWindowPosition(100, 100); // Where the window will display on-screen.
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE); // create single buffer RGB window
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	Main_Window = glutCreateWindow("glsl");
	glutReshapeFunc(windowResize);
	glutDisplayFunc(windowDisplay);
	glutMouseFunc(handleButtons);
	glutMotionFunc(handleMotion);
	glutMouseWheelFunc(handlewheel);

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


