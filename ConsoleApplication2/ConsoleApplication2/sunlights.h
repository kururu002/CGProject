#include <math.h>
GLfloat light_ambient[] = {0.5,0.5,0.5,1.0};
GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
class Sun {
	float farr;
	float delta;
	float time;;
public:
	Sun() :farr(600), delta(0.02), time(0.0) {};
	float getfar() { return farr; }
	float getangle() { return (time/24*360-60)*PI/180; }
	float* getcolor() { 
		float color[3] = { 0.0,0.0,0.0 };
		if (time >= 19.0 || time <= 4.5)return color;
		else if (time > 4.5&&time <= 6.0) {
			color[0] = color[1] = (time - 4.5)*255.0 / 1.5;
		}
		else if (time > 6.0&&time <= 12.0) {
			color[0] = color[1] = 255.0;
			color[2] = (time - 6.0)*255.0 / 6.0;
		}
		else if (time > 12.0&&time < 18.0) {
			color[0] = 255.0;
			color[1] = 255.0 - (time - 12.0) * 128 / 6.0;
				color[2] = 255.0 - (time - 12.0)*255.0 / 6.0;
		}
		else if (time >= 18.0 < 19.0) {
			color[0] = 255.0 - time*255.0;
			color[1] = 128.0 - time*128.0;
			color[2] = 0;
		}
		for (int i = 0; i < 3; i++) {
			color[i] /= 255.0;
		}
		return color;
	}
	void update() {
		if (time >= 24.0)time = 0.0;
		time += delta;
	}
	void DrawSun() // �������Ĳ��ʲ����Ƶ���  
	{
		GLfloat* clr = getcolor();
		glColor3fv(clr);
		GLfloat ori[4] = { 0.0f,0.0f,0.0f,1.0f };
		GLfloat pos[4] = { 0.0f, farr*sin(getangle()),farr*cos(getangle()),0.0 };
		glPushMatrix();
		glDisable(GL_BLEND);
		glDisable(GL_LIGHTING);
		glMaterialfv(GL_FRONT, GL_EMISSION, clr);
		glTranslatef(pos[0],pos[1],pos[2]);
		glutSolidSphere(50.0, 40, 32);		
		glPopMatrix();
		//glEnable(GL_BLEND);
		glEnable(GL_LIGHTING);
		glMaterialfv(GL_FRONT, GL_EMISSION,ori );
		//���ʹ��glutSolidSphere�������������壬��ú������Զ���ָ����Щ����������  
		//�������ֹ�ָ����������Լ�ָ�����ɵĶ���������һ�����壬����Ҫ�Լ�ָ������������  
		update();
		glEnable(GL_LIGHT1);
		GLfloat light_ambient1[] = {0.0,0.0,0.0,1.0};
		glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient); //������
		glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse); //�����
		glLightfv(GL_LIGHT1, GL_SPECULAR, clr);
		glLightfv(GL_LIGHT1, GL_POSITION, pos); //����λ��*/
		
		glEnable(GL_DEPTH_TEST);
		glutPostRedisplay();
	}

};