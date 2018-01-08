
#include <windows.h>
#define GLUT_DISABLE_ATEXIT_HACK
#define PI 3.14
#define POOL_HEIGHT 0.3
#define NUM_X_OSCILLATORS		170//NUM OF X OSC
#define NUM_Z_OSCILLATORS		170
#define OSCILLATOR_DISTANCE		0.015//DISTANCE BETWEEN OSC
#define OSCILLATOR_WEIGHT       0.0001

#define MAXX					(NUM_X_OSCILLATORS*OSCILLATOR_DISTANCE)
#define MAXZ					(NUM_Z_OSCILLATORS*OSCILLATOR_DISTANCE)
#include <GL/glaux.h>
#ifndef _GLUTANDSTDINLUDE_
#define _GLUTANDSTDINLUDE_
#include <stdlib.h>
#include <GL/GLUT.H>
#endif
#include <math.h>
#include <stdio.h>
using namespace std;
#include"camera.h"
#include "loadTGA.h"
#include"skybox.h"
#include "Particle.h"
#include "loader.h"
#include "fire.h"
#include"sunlights.h"
#pragma comment(lib,"GLAUX.LIB")

#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>
#endif

float light2 = 1.0f;
Sun sun;
Camera camera;
/** 创建一个粒子类对象 */
CPool Pool;

//water outside the bowl is in the air:
CAirFountain AirFountain;

CParticle Snow;
CParticle Fire;
int sunpos[] = { 0,0,600 };
/** 用来设置粒子的属性值 */
float x, y, z, vx, vy, vz, ax, ay, az, psize, lifetime, decz;
int r, g, b;

float x_fire, y_fire, z_fire, vx_fire, vy_fire, vz_fire, ax_fire, ay_fire, az_fire, psize_fire, lifetime_fire, decz_fire;
int r_fire, g_fire, b_fire;



ObjLoader pee;
ObjLoader obj;
ObjLoader tem;

int model = 0;

/** 载入纹理 */
GLuint texName[3];


AUX_RGBImageRec *LoadBMP(char *Filename) // Loads A Bitmap Image 
{
	FILE *File = NULL;        // File Handle 
	if (!Filename)        // Make Sure A Filename Was Given 
	{
		return NULL;        // If Not Return NULL 
	}
	fopen_s(&File, Filename, "r");      // Check To See If The File Exists 
	if (File)           // Does The File Exist? 
	{
		fclose(File);         // Close The Handle 
		return auxDIBImageLoad(Filename);   // Load The Bitmap And Return A Pointer 
	}
	return NULL;          // If Load Failed Return NULL 
}


int LoadGLTextures()        // Load Bitmaps And Convert To Textures 
{
	int Status = FALSE;        // Status Indicator 
	AUX_RGBImageRec *TextureImage[2];    // Create Storage Space For The Texture 

	memset(TextureImage, 0, sizeof(void *) * 2); // Set The Pointer To NULL 
	char *pictures[] = {// 创建一个位图名称数组，对应n幅位图
		"data/wall.bmp",
		"data/test4.bmp",
		"data/test7.bmp"
		//...
	};

	for (int i = 0; i < 2; i++)
	{
		if (TextureImage[i] = LoadBMP(pictures[i]))// 加载位图i成功，修改状态标志变量Status为TRUE
		{
			Status = TRUE;


			glGenTextures(1, &texName[i]);    // Create One Textures 

			glBindTexture(GL_TEXTURE_2D, texName[i]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, 3,
				TextureImage[i]->sizeX, TextureImage[i]->sizeY,
				0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[i]->data);
		}

		if (TextureImage[i])        // If Texture Exists 
		{
			if (TextureImage[i]->data)      // If Texture Image Exists 
			{
				free(TextureImage[i]->data);    // Free The Texture Image Memory 
			}
			free(TextureImage[i]);       // Free The Image Structure 
		}
	}
	return Status;          // Return The Status 
}




/** 绘制地面 */
float angle = 0;
void DrawGround()
{
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient); //环境光
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse); //漫射光
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular); //镜面反射


	GLfloat mat_ambient[] = { 0.105882, 0.058824, 0.113725, 1.000000 };
	GLfloat mat_diffuse[] = { 0.427451, 0.470588, 0.541176, 1.000000 };
	GLfloat mat_specular[] = { 0.333333, 0.333333, 0.521569, 1.000000 };
	GLfloat mat_shininess[] = { 9.846150 }; //材质RGBA镜面指数，数值在0～128范围内


	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glTranslatef(0.0f, 0.0f, -6.0f);
	glRotatef(angle, 0.0f, 1.0f, 0.0f);

	/** 指定纹理 */
	glTexEnvf(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, texName[0]);
	glColor4ub(255, 255, 255, 255);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-50.0f, -1.0f, 50.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-50.0f, -1.0f, -50.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(50.0f, -1.0f, -50.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(50.0f, -1.0f, 50.0f);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	/** 地面转动的角度 */
    angle += 0.02f;
}

/** 初始化雪花粒子 */
bool InitSnow()
{
	for (int i = 0; i < Snow.GetNumOfParticle(); ++i)
	{
		///初始化颜色（白色）
		r = 255;
		g = 255;
		b = 255;
		Snow.SetColor(i, r, g, b);

		///初始化坐标
		x = 0.1f * (rand() % 50) - 2.5f;
		y = 2 + 0.1f * (rand() % 2);
		if ((int)x % 2 == 0)
			z = rand() % 6;
		else
			x = -rand() % 3;
		Snow.SetPosition(i, x, y, z);

		///初始化速度
		vx = 0.00001 * (rand() % 100);
		vy = 0.0000002 * (rand() % 28000);
		vz = 0;
		Snow.SetVelocity(i, vx, vy, vz);

		///初始化加速度
		ax = 0;
		ay = 0.000005f;
		az = 0;
		Snow.SetAcceleration(i, ax, ay, az);

		///初始化生命周期
		lifetime = 100;
		Snow.SetLifeTime(i, lifetime);

		///消失速度
		decz = 0.005 * (rand() % 50);
		Snow.SetDec(i, decz);

		///初始化大小
		Snow.SetSize(i, 0.03f);
	}
	return true;
}

/** 更新粒子 */
void UpdateSnow()
{
	/** 更新位置 */
	x += (vx * 5);
	y -= vy;

	/** 更新速度 */
	vy += ay;

	/** 更新生存时间 */
	lifetime -= decz;

	if (x > 3)
		x = -2;

	/** 如果粒子消失或生命结束 */
	if (y <= -1 || lifetime <= 0)
	{
		/** 初始化位置 */
		x = 0.1f * (rand() % 50) - 2.5f;
		y = 2 + 0.1f * (rand() % 2);
		if ((int)x % 2 == 0)
			z = rand() % 6;
		else
			z = -rand() % 3;

		/** 初始化速度 */
		vx = (float)(0.00001 * (rand() % 100));
		vy = (float)(0.0000002 * (rand() % 28000));
		vz = 0;

		/** 初始化加速度 */
		ax = 0;
		ay = 0.000005f;
		az = 0;
		lifetime = 100;
		decz = 0.005*(rand() % 50);
	}
}

/** 绘制粒子 */
void DrawParticle()
{
	glPushMatrix();
	/** 绑定纹理 */
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texName[1]);
	for (int i = 0; i<Snow.GetNumOfParticle(); ++i)
	{
		GLfloat mat_ambient[] = { 0.8,0.8,0.8,1.0 };
		//定义 紫色 的漫反射特性
		GLfloat mat_diffuse[] = { 0.8,0.0,0.8,1.0 };
		//定义 亮紫色 的镜面反射特性
		GLfloat mat_specular[] = { 1.0,0.0,1.0,1.0 };
		//定义镜面反射的光亮度
		GLfloat mat_shininess[] = { 50.0 };
		//将以上材质定义应用
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
		/** 获得粒子的所有属性 */
		Snow.GetAll(i, r, g, b, x, y, z, vx, vy, vz, ax, ay, az, psize, lifetime, decz);
		glColor4ub(r, g, b, 255);
		glNormal3f(0.0f, 0.0f, 1.0f);   /**< 定义法线方向 */
										/** 画出粒子 */
		if (model == 0) {

			glTranslatef(x, y, z - 6);
			glBegin(GL_QUADS);
			GLUquadricObj *quadObj = gluNewQuadric();//创建一个二次曲面物体
			gluQuadricTexture(quadObj, GL_TRUE);        //启用该二次曲面的纹理

			gluSphere(quadObj, 0.1, 30, 30); //画圆
			gluDeleteQuadric(quadObj);
			glEnd();
			
		}
		else{
			obj.update(x,y,z-6);
		}
		/** 更新粒子属性 */
		UpdateSnow();
		Snow.SetAll(i, r, g, b, x, y, z, vx, vy, vz, ax, ay, az, psize, lifetime, decz);
	}
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glutPostRedisplay();//发送渲染请求
}
void Drawpool() {
	glPushMatrix();
	glTranslatef(0.0, POOL_HEIGHT, 0.0);
	Pool.Render();
	glPopMatrix();
	glEnable(GL_BLEND);
	glDisable(GL_LIGHTING);
	glColor4f(0.8, 0.8, 0.8, 0.8);
	AirFountain.Render();
	glEnable(GL_LIGHTING);
	glDisable(GL_BLEND);
}



void DrawTemplate()
{
	glPushMatrix();
	glColor4ub(r, g, b, 255);
	glNormal3f(0.0f, 0.0f, light2);   /**< 定义法线方向 */
	tem.update(1.0f, -0.7f, 0.0f);
	glPopMatrix();
}

void DrawBall() // 定义地球的材质并绘制地球  
{
	glDisable(GL_BLEND);
        GLfloat earth_mat_ambient[]  = {0.0f, 0.0f, 0.5f, 1.0f};  
        GLfloat earth_mat_diffuse[]  = {0.0f, 0.0f, 0.5f, 1.0f};  
        GLfloat earth_mat_specular[] = {0.0f, 0.0f, 1.0f, 1.0f};  
        GLfloat earth_mat_emission[] = {0.0f, 0.0f, 0.0f, 1.0f};  
        GLfloat earth_mat_shininess  = 30.0f;  
  
        glMaterialfv(GL_FRONT, GL_AMBIENT,   earth_mat_ambient);  
        glMaterialfv(GL_FRONT, GL_DIFFUSE,   earth_mat_diffuse);  
        glMaterialfv(GL_FRONT, GL_SPECULAR,  earth_mat_specular);  
        glMaterialfv(GL_FRONT, GL_EMISSION,  earth_mat_emission);  
        glMaterialf (GL_FRONT, GL_SHININESS, earth_mat_shininess);  
		glTranslatef(0.0f, 0.0f, -6.0f);
		glRotatef(angle, 0.0f, 1.0f, 0.0f);
        glutSolidSphere(1.0, 40, 32);  
		glEnable(GL_BLEND);
        //如果使用glutSolidSphere函数来绘制球体，则该函数会自动的指定这些法线向量，  
        //不必再手工指出。如果是自己指定若干的顶点来绘制一个球体，则需要自己指定法线向量。  
}  


/////////////////////////////////////////////////////////////////
/** 初始化 */
void init(void)
{
	/** 用户自定义的初始化过程 */

	
	glDepthFunc(GL_LESS);

	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);//指定深度缓冲区的清除值(即将深度缓冲区里的值设置为1)
	glDepthFunc(GL_LEQUAL);//指定用于深度缓冲比较值(即新进像素深度值与原来的1比较，<=则通过，否则丢弃)
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glEnable(GL_TEXTURE_2D);             /**< 开启纹理映射 */
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);    /**< 设置混合因子获得半透明效果 */
	glEnable(GL_BLEND);                  /**< 启用混和 */

										 /**  载入纹理 */
	if (!LoadGLTextures())
	{
		printf("bmp load failed");
		exit(-1);
	}

	/** 创建500个粒子 */
	Pool.Initialize(NUM_X_OSCILLATORS, NUM_Z_OSCILLATORS, OSCILLATOR_DISTANCE, OSCILLATOR_WEIGHT, 0.05, 4.0, 4.0);
	//init the airfountain: (look at KeyDown() to see more possibilities of initialization)
	AirFountain.Initialize(3, 8, 35, 76, 90, 0.5, 0.11);


	//place it in the center of the pool:
	AirFountain.Position = F3dVector(NUM_X_OSCILLATORS*OSCILLATOR_DISTANCE / 2.0f,
		POOL_HEIGHT,
		NUM_Z_OSCILLATORS*OSCILLATOR_DISTANCE / 2.0f);
	Snow.Create(50);
	Fire.Create(500);

	/** 初始化粒子 */
	InitSnow();
	InitFire();
	loadSkybox();
}

	GLfloat light_position0[] = { 2.0, 100.0, 10.0 ,1.0 };//0=infinite else point

	float temp = 1.0;
/** 渲染 */
void display(void)
{
	/** 用户自定义的绘制过程 */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	
	
	GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 }; 
	/*glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient); //环境光
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse); //漫射光
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular); //镜面反射
	glLightfv(GL_LIGHT0, GL_POSITION, light_position0); //光照位置
	/** 绘制地面 */
	
	
	glLoadIdentity();
	gluLookAt(camera.getX(), camera.getY(), camera.getZ(), 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	//printf("%f \n", camera.getY());
	//glLoadIdentity();
	//DrawGround();
	skybox();
	Drawpool();
	/** 绘制粒子 */
	sun.DrawSun();
	glPushMatrix();
	glRotated(-25.0, 0.0, 1.0, 0.0);
	pee.update(-0.3f, 1.5, -1.0f);
	glPopMatrix();
	//DrawTemplate();
	DrawParticle();
	DrawFire();
	
	light_position0[0] += temp;
	if (light_position0[0] <= -15.0) { temp = 1.0; }
	else if (light_position0[0] >= 15.0) { temp = -1.0; }
	float dtime = 0.006;
	AirFountain.Update(dtime, &Pool);
	Pool.Update(dtime);
	glFlush(); /**<强制执行所有的OpenGL命令 */
	glutSwapBuffers();
}


void ChangeSize(int width, int height)
{
	glViewport(0, 0, width, height);    /**< 重新设置视口 */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 1700.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void processNormalKeys(unsigned char key, int x, int y)
{

	switch (key) {
	case 87:camera.moveUp(); break;//w 
	case 83:camera.moveDown(); break;//d
	case 65:camera.moveLeft(); break;//a
	case 68:camera.moveRight(); break;//d
	case 49:model = 1; break;//1
	case 50:model = 0; break;
	}
	glutPostRedisplay();
	
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGB);
	glutInitWindowSize(1000, 800);
	glutInitWindowPosition(100, 0);
	glutCreateWindow("雪花粒子");
	glutKeyboardFunc(processNormalKeys);
	glutReshapeFunc(ChangeSize);
	init();
	
	obj.Load("data/egg4.obj");
	pee.Load("data/goldpeee.obj");
	//tem.Load("data/goldpeee.obj");
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}