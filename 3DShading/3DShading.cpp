// 3DShading.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "ImageData.h"
#define NONE -1
#define M 3

int Main_Window;
static int MouseX;
static int MouseY;
static int Button = NONE;

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define DOF 1
#define MOTIONBLUR 0
#define CAMERA_PAINTING 1
#define PAINTING_FRAMES 36
#define TRANFORMING_FRAMES 36

using namespace std;
using namespace glm;

UCHAR *pixels;
Camera cam;
MyObject * objList[10]; //allowing a fixed amount of objects
vec3 LightSource = vec3(-1000, -5000, 5000);
vec3 LD = vec3(0, -1, 0);
float Ltheta = 70.0*PI / 180.0;
Light originalLL(POINT, LightSource, LD, Ltheta);

//##############Projector################
ImageData projectTexture("viz.jpg");
Projector pr1(vec3(0,500,1000),vec3(0,0,-1),vec3(0,1,0),10,20,20,&projectTexture);

//###############Animation#############
int curFrame = 0;
ImageData PaintingImage("Blood-dripping.jpg");
ImageData PaintingImage2("symbol.png");
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
ImageData floorTexturemap("tileable-textures-cracked-brown-soil-seamless-tileable-texture-stock-photo-colourbox.jpg");

//ImageData wallTexturemap("seamless-cubic-stone-road-floor-texture-free-221.jpg");

ImageData skyTexturemap("0 (2).jpg");
float FocalDistance = 1000;
float ApertureSize = 5;
float timeStart = 0;
float timeEnd = 9;
vec3 endPos = vec3(0, 400, 0);
//#################################
std::random_device rd;  //Will be used to obtain a seed for the random number engine
std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
std::uniform_real_distribution<> dis(0, 1);
std::uniform_real_distribution<> dis_np(-1, 1);

static void init(void)
{
	glClearColor(1, 1, 1, 1); // Set background color.
	pixels = new unsigned char[SCREEN_WIDTH * SCREEN_HEIGHT * 3];
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
	cam.p = vec3(-1000, 300, -200);
	//cam.p = vec3(-1000, 0, 0);
	cam.v0 = center - cam.p;
	cam.vUp = vec3(0, 0, 1);
	cam.sx = 12.8;
	cam.sy = 7.2;
	cam.d = 7;			//90 vertical fov
	printf("Camera: %lf,%lf,%lf\n", cam.p[0], cam.p[1], cam.p[2]);

	vec3 sCenter = cam.p + normalize(cam.v0) * (float)cam.d;

	vec3 n2 = normalize(cam.v0);
	vec3 n0 = normalize(glm::cross(n2, cam.vUp));
	vec3 n1 = glm::cross(n0, n2);
	vec3 s0 = sCenter - n0 * (float)(cam.sx / 2.0) - n1 * (float)(cam.sy / 2.0);
	//###################################

	//#############Setting Projector################

	//vec3 pCenter = pr1.p + normalize(pr1.v0)*(float)pr1.d;

	//vec3 pn2 = normalize(pr1.v0);
	//vec3 pn0 = normalize(cross(pn2, pr1.vUp));
	//vec3 pn1 = cross(pn0, pn2);
	//vec3 ps0 = pCenter - pn0 * (float)(pr1.sx / 2.0) - pn1 * (float)(pr1.sy / 2.0);

	//###############################

	//#########################Setting Light###################
	vec3 ln2 = normalize(originalLL.xd);
	vec3 ln0 = normalize(glm::cross(ln2, originalLL.direction));
	vec3 ln1 = glm::cross(ln0, ln2);
	vec3 ls0 = originalLL.position - ln0 * (float)(originalLL.sx / 2.0) - ln1 * (float)(originalLL.sy / 2.0);
	//###################################

	
	double eps = 1e-4;
	
	for (int j = SCREEN_HEIGHT - 1; j >= 0; j--)
		for (int i = 0; i < SCREEN_WIDTH; i++)
		{	
			spSet shdingPointSet;
			double ri = dis(gen);
			double rj = dis(gen);
			int k = (j * SCREEN_WIDTH + i) * 3;
			vec3 colorSum = vec3(0, 0, 0);

			int il_start = (int)(M*dis(gen));
			int jl_start = (int)(M*dis(gen));

			vec3 distort = 0.02f * vec3(dis_np(gen), dis_np(gen), dis_np(gen));

			int timeCur = (int)(sqrt(dis(gen)) * (timeEnd - timeStart));
			float frameInt = dis(gen);
			for(int ii=0;ii<M;ii++)
				for (int jj = 0; jj < M; jj++)
				{
					vec3 drawColor = vec3(0, 0, 0);
					float ix = (i + (ii + ri) * 1.0/M) * 1.0 / SCREEN_WIDTH;
					float jy = (j + (jj + rj) * 1.0 /M) * 1.0 / SCREEN_HEIGHT;

					vec3 npe = s0 + n0 * ix * cam.sx + n1 * jy *cam.sy;
					npe = npe - cam.p;
					npe = normalize(npe);

					vec3 newCamPosition = cam.p;	//for dof

					if (CAMERA_PAINTING)
					{
						int paint_x = floor(ix*1.0  * (PaintingImage.getWidth()-1));
						int paint_y = floor(jy*1.0 * (PaintingImage.getHeight() - 1));
						ColorRGBA paintColor = PaintingImage.getRGBA(paint_x, paint_y);
						vec3 paint = vec3(paintColor.r, paintColor.g, paintColor.b);
						npe = s0 + n0 * ix * cam.sx + n1 * jy *cam.sy + float(std::min(curFrame,2*PAINTING_FRAMES) *3.0/PAINTING_FRAMES)*1.0f
							*(paint[0]*n0 + paint[1]*n1+paint[2]*n2 ) - cam.p;
						int paint_x2 = floor(ix*1.0  * (PaintingImage2.getWidth() - 1));
						int paint_y2 = floor(jy*1.0 * (PaintingImage2.getHeight() - 1));
						ColorRGBA paintColor2 = PaintingImage2.getRGBA(paint_x2, paint_y2);
						vec3 paint2 = vec3(paintColor2.r, paintColor2.g, paintColor2.b);
						npe = npe + 3.0f*float(std::max(curFrame - PAINTING_FRAMES, 0)*1.0/TRANFORMING_FRAMES) * (paint2[0] * n0 + paint2[1] * n1 + paint2[2] * n2);
						npe = normalize(npe);
					}
					if (MOTIONBLUR)
					{	
						sp1.p0 = center + ((timeCur+ii*M +jj)%(int)(timeEnd - timeStart) + frameInt - timeStart) / (timeEnd - timeStart)* (endPos - center);
					}
					if (DOF)
					{
						vec3 FocalPoint = cam.p + FocalDistance * npe;
						newCamPosition = newCamPosition + vec3(dis_np(gen)*ApertureSize, dis_np(gen)*ApertureSize, dis_np(gen)*ApertureSize);
						npe = normalize(FocalPoint - newCamPosition);
					}
					float th = INFINITE;
					int objDrawn = -1;

					//###############Hit##########################
					for (int oo = 0; oo < objNum; oo++)
					{
						float cur_th = objList[oo]->hit(npe, newCamPosition, shdingPointSet);
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
						vec3 ph = newCamPosition + th * npe;

						//####################Texturing & reflecting##################
						if (objDrawn == 1 || objDrawn == 2 || objDrawn == 3)
						{
							//Plane* pl = (Plane*)(objList[objDrawn]);

							objList[objDrawn]->textureMapping(ph, drawColor, shdingPointSet);
						}

						if (objDrawn == 0 ) // reflecting sphere
						{
							Sphere* spt = (Sphere*)(objList[objDrawn]);
							glm::vec3 nt2 = n2;
							spt->nt0 = normalize(vec3(1,1,0));
							spt->nt1 = normalize(vec3(1, -1, 0));
							spt->solidMapping(ph, drawColor);

							//FresnelSet fs;
							//Ray income;
							//income.v = npe;
							//income.p = cam.p;
							//spt->fresnel(ph, objList, objNum, income, fs, shdingPointSet,distort);
							//drawColor = fs.color;
						}
						//###################Area Light Check##########
						Light LL = originalLL;
						if (originalLL.type == AREA)
						{
							float il = ((ii + il_start)%M + ri) * 1.0 / M;
							float jl = ((jj+jl_start)%M + rj) * 1.0 / M;
							LL.position = ls0 + ln0 * il * originalLL.sx + ln1 * jl *originalLL.sy;
						}
						//##################Diffusing & Specular Highlight####################
						float T = objList[objDrawn]->diffuse(npe, newCamPosition, th, LL, shdingPointSet);
						/*float T = 1;*/
						float S = objList[objDrawn]->specular(npe, newCamPosition, th, LL,shdingPointSet);
						//drawColor = T * drawColor + (1 - T)*objList[objDrawn]->color_dark;

						//###############Cast Shadow################//
					
						float T_s = objList[objDrawn]->shadowCast(ph, objList, objNum, LL, shdingPointSet);
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
						objList[objDrawn]->textureMapping(ph, drawColor, shdingPointSet);
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
	char saveName[50];
	sprintf(saveName, "animations/%03d.png", curFrame);
	img.writeFile(saveName);
	curFrame++;
	printf("\n");
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

void handleTime(int a)
{
	glutPostRedisplay();
	glutTimerFunc(1000, handleTime, 10);
	Sleep(200);
	printf("wait 200\n");
}

int main(int argc, char *argv[])
{
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
	sp1.normalMap = NULL;
	sp1.northPole = vec3(0, 0, 1);
	//pl1.texture = &wallTexturemap;
	pl3.texture = &floorTexturemap;
	glm::vec3 nt2 = pl3.n0;
	pl3.nt0 = normalize(cross(vec3(0, 0, 1), nt2));
	pl3.nt0 = normalize(cross(vec3(0, 1, 0), nt2));
	pl3.nt1 = normalize(cross(pl3.nt0, nt2));
	pl3.s0 = 800;
	pl3.s1 = 600;
	pl3.txP0 = pl3.p0;

	ifsp.texture = &skyTexturemap;

	ImageData vizimage("viz.jpg");
	ImageData irr("irtex.jpg");
	sp1.irrMap = &irr;
	char file[100] = "cube.obj";
	ObjFromFile cube(file);
	cube.texture = &vizimage;
	cube.irrMap = &irr;
	/*objList[objNum++] = &cube;*/
	sp1.transparency = 0;
	cube.transparency = 0;

	objList[objNum++] = &ifsp;
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
	glutTimerFunc(1000, handleTime, 10);
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


