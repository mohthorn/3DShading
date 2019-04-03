// 3DShading.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "ImageData.h"
#define NONE -1
#define M 4

int Main_Window;
static int MouseX;
static int MouseY;
static int Button = NONE;

#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 900


using namespace glm;

UCHAR *pixels;
Camera cam;
MyObject * objList[10]; //allowing a fixed amount of objects
vec3 LightSource = vec3(0, 500, 1000);
vec3 LD = vec3(0, -1, 0);
float Ltheta = 70.0*PI / 180.0;
Light originalLL(POINT, LightSource, LD, Ltheta);

//##############Projector################
ImageData projectTexture("viz.jpg");
Projector pr1(vec3(0,500,1000),vec3(0,0,-1),vec3(0,1,0),10,20,20,&projectTexture);

//######################################

//###############setting objects##############//
vec3 sphereColor = vec3(255, 255, 255);
vec3 planeColor = vec3(255, 255, 255);
vec3 center = vec3(0, 500, 0);
Sphere sp1(200, sphereColor, center);
Plane pl1(vec3(0, -1, 0), vec3(-1000, 1000, -1000), planeColor);
Plane pl2(vec3(1, 0, 0), vec3(-1000, 1000, -1000), vec3(255, 255, 255));
Plane pl3(vec3(0, 0, 1), vec3(-1000, 1000, -300), vec3(255, 255, 255));
IFSphere ifsp(vec3(255,255,255));
int objNum = 0;


//############Texture Maps##########
ImageData eyeTexturemap("eye.jpg");


ImageData wallTexturemap("seamless-cubic-stone-road-floor-texture-free-221.jpg");

ImageData skyTexturemap("HDR-Day2-_1_.jpg");
//#################################
std::random_device rd;  //Will be used to obtain a seed for the random number engine
std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
std::uniform_real_distribution<> dis(0, 1);


static void init(void)
{
	glClearColor(1, 1, 1, 1); // Set background color.
	pixels = new unsigned char[SCREEN_WIDTH * SCREEN_HEIGHT * 3];
}

void setPixels()
{
	// Example to read image, modify and display it
	// Initialize a image instance by reading an image file
	// Check the class file to find more construction methods
	ImageData img("viz.jpg");
	// Change the global variable to make display window fit the image size
	int width = img.getWidth();
	int height = img.getHeight();
	// Traversal pixels in the image
	for (int j = 0; j < height; j += 10)
	{
		for (int i = 0; i < width; i += 10)
		{
			// Get image color at position i, j
			ColorRGBA cur_color = img.getRGBA(i, j);
			// Change the pixel color at i, j
			// For example, make the color half of the original value
			img.setRGBA(i, j, cur_color * 0.5);
		}
	}
	// Read color values into pixmap array
	//img.getPixels(pixels);
	img.writeFile("newViza.jpg");
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
	/*cam.p = vec3(341, 0, 347);*/
	cam.p = vec3(2000, 2000, 2000);
	cam.v0 = center - cam.p;
	cam.vUp = vec3(0, 0, 1);
	cam.sx = 10;
	cam.sy = 10;
	cam.d = 5;
	printf("Camera: %lf,%lf,%lf\n", cam.p[0], cam.p[1], cam.p[2]);

	vec3 sCenter = cam.p + normalize(cam.v0) * (float)cam.d;

	vec3 n2 = normalize(cam.v0);
	vec3 n0 = normalize(glm::cross(n2, cam.vUp));
	vec3 n1 = glm::cross(n0, n2);
	vec3 s0 = sCenter - n0 * (float)(cam.sx / 2.0) - n1 * (float)(cam.sy / 2.0);
	//###################################

	//#############Setting Projector################

	vec3 pCenter = pr1.p + normalize(pr1.v0)*(float)pr1.d;

	vec3 pn2 = normalize(pr1.v0);
	vec3 pn0 = normalize(cross(pn2, pr1.vUp));
	vec3 pn1 = cross(pn0, pn2);
	vec3 ps0 = pCenter - pn0 * (float)(pr1.sx / 2.0) - pn1 * (float)(pr1.sy / 2.0);

	//###############################


	//#########################Setting Light###################
	vec3 ln2 = normalize(originalLL.xd);
	vec3 ln0 = normalize(glm::cross(ln2, originalLL.direction));
	vec3 ln1 = glm::cross(ln0, ln2);
	vec3 ls0 = originalLL.position - ln0 * (float)(originalLL.sx / 2.0) - ln1 * (float)(originalLL.sy / 2.0);
	//###################################

	//printf("Sphere: %lf,%lf,%lf\n", sp1.p0[0], sp1.p0[1], sp1.p0[2]);
	
	double eps = 1e-4;

	for (int j = SCREEN_HEIGHT - 1; j >= 0; j--)
		for (int i = 0; i < SCREEN_WIDTH; i++)
		{	
			double ri = dis(gen);
			double rj = dis(gen);
			int k = (j * SCREEN_WIDTH + i) * 3;
			vec3 colorSum = vec3(0, 0, 0);

			int il_start = (int)(M*dis(gen));
			int jl_start = (int)(M*dis(gen));

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

					//###############Hit##########################
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

					//########################
					if (objDrawn >= 0)
					{
						drawColor = objList[objDrawn]->color;
						vec3 ph = cam.p + th * npe;

						//####################Texturing##################
						if (objDrawn == 1 || objDrawn == 2 || objDrawn == 3)
						{
							Plane* pl = (Plane*)(objList[objDrawn]);
							glm::vec3 nt2 = pl->n0;
							vec3 nt0 = normalize(cross(vec3(0, 0, 1), nt2));
							if(objDrawn == 1)
								nt0 = normalize(cross(vec3(0, 1, 0), nt2));
							vec3 nt1 = normalize(cross(nt0, nt2));
							objList[objDrawn]->textureMapping(ph, pl->p0, nt0, nt1, 800, 600, drawColor);
						}

						if (objDrawn == 0)
						{
							Sphere* pl = (Sphere*)(objList[objDrawn]);
							glm::vec3 nt2 = n2;
							vec3 nt0 = normalize(vec3(1,1,0));
							vec3 nt1 = normalize(vec3(1, -1, 0));
							pl->solidMapping(ph, pl->p0, nt0, nt1, 500, 500, drawColor);
						}
						//###################################


						Light LL = originalLL;
						if (originalLL.type == AREA)
						{
							float il = ((ii + il_start)%M + ri) * 1.0 / M;
							float jl = ((jj+jl_start)%M + rj) * 1.0 / M;
							LL.position = ls0 + ln0 * il * originalLL.sx + ln1 * jl *originalLL.sy;
						}

						//##################Diffusing & Specular Highlight####################
						//float T = objList[objDrawn]->diffuse(npe, cam.p, th, LL);
						/*float T = 1;*/
						float S = objList[objDrawn]->specular(npe, cam.p, th, LL);
						//drawColor = T * drawColor + (1 - T)*objList[objDrawn]->color_dark;

						//###############Cast Shadow################//
						float T_s=0;
						int spot_flag = 0;
						float d = 20;

						
						vec3 n;
						objList[objDrawn]->getNormal(ph, n);
						vec3 ph_d = ph - d * n;
						vec3 pl = ph_d - LL.position;
						vec3 npl = normalize(pl);
						

						if (LL.type == DIR)
						{
							npl = normalize(-LL.direction);
							LL.position = ph_d - 10000.0f * npl;
							pl = ph_d - LL.position;
						}
						if (LL.type == SPOT)
						{	
							vec3 nL = normalize(LL.position - ph_d);
							if ( dot(nL, LL.direction) < cos(LL.theta))
							{
								spot_flag = 1;
							}
						}
						float totalLength = 0;
						
						float costheta = 1;

						if (!spot_flag)
						{
							for (int oo = 0; oo < objNum; oo++)
							{

								float slen = 0;
								float lh = objList[oo]->shadowLength(npl, LL, slen, ph_d);
								if (lh < glm::length(pl) && fabs(lh - glm::length(pl)) > 0.1 && lh > 0)
								{
									totalLength = totalLength + slen;
								}

								if (oo == objDrawn)
								{
									float r = glm::length(LL.position + lh * npl - ph_d);
									costheta = d*1.0 / r;
									costheta = (costheta + 1) / 2.0;
								}
							}

							T_s = d / (totalLength/* * costheta*/);
						}
						//if (objDrawn == 0)
						//	T_s = objList[objDrawn]->shadowFunction(T_s);
						if (T_s > 1)
							T_s = 1;
						if (T_s < 0)
							T_s = 0;

						//#################Projecting########################

						//glm::vec3 pColor;

						//int projected = pr1.projectColor(pn0, pn1, pn2, ph, pColor);

						//if (projected && T_s >0.2)
						//	drawColor = 0.8f * pColor + 0.2f * drawColor;
						//##############################################

						drawColor = T_s * drawColor + (1 - T_s)*objList[objDrawn]->color_dark;
						//##########################################//
						
						S *= T_s;
						drawColor = S * objList[objDrawn]->color_specular + (1 - S) *drawColor;
						if (fabs(th - BOUNDARY) < eps)
							drawColor = vec3(0, 0, 0);
					}
					else
					{
					//######################Environment Map##########
						objDrawn = objNum - 1;
						drawColor = objList[objDrawn]->color;
						vec3 ph = npe;
						float theta=acos(ph.z) / PI;
						//theta = theta + 0.5;
						float T = theta;
						//if (theta < 0.5)
						//	T *= 1.1;
						//else
						//	T = 1;
						objList[objDrawn]->textureMapping(ph, cam.p, n0, n1, 0, 0, drawColor);
						drawColor = T * objList[objDrawn]->color_dark + (1 - T) *drawColor;
					//#########################################
					}

					colorSum += drawColor;
				}
			pixels[k] =colorSum[0] / (float)(M*M);
			pixels[k + 1] =colorSum[1] / (float)(M*M);
			pixels[k + 2] =colorSum[2] / (float)(M*M);
		}
	glDrawPixels(SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	
	ImageData img(SCREEN_WIDTH, SCREEN_HEIGHT, pixels);

	img.writeFile("result.jpg");
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
	//image test
	setPixels();

	//camera1 = camera(glm::vec3(0, 0, 10), glm::vec3(0, 0.4, 0), glm::vec3(0, 1, 0), 45);
	objList[objNum++] = &sp1;
	/*objList[objNum++] = &pl1;*/
	//objList[objNum++] = &pl2;
	objList[objNum++] = &pl3;

	originalLL.sx = 100;
	originalLL.sy = 100;
	originalLL.xd = vec3(0, 0, -1);
	sp1.texture = &eyeTexturemap;
	ImageData spN("12759-normal.jpg");
	sp1.normalMap = &spN;
	sp1.northPole = vec3(0, 0, 1);
	//pl1.texture = &wallTexturemap;
	pl3.texture = &wallTexturemap;
	ifsp.texture = &skyTexturemap;

	char file[100] = "cube.obj";
	ObjFromFile cube(file);
	objList[objNum++] = &cube;

	objList[objNum++] = &ifsp;
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


