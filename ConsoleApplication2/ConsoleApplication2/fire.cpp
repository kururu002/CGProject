#include "fire.h"
extern float x_fire, y_fire, z_fire, vx_fire, vy_fire, vz_fire, ax_fire, ay_fire, az_fire, psize_fire, lifetime_fire, decz_fire;
extern int r_fire, g_fire, b_fire;
extern GLuint texName[2];
extern CParticle Fire;

bool InitFire()
{
	for (int i = 0; i < Fire.GetNumOfParticle(); ++i)
	{
		///��ʼ����ɫ
		//r = rand()%255;
		//g = rand()%255;
		//b = rand()%255;
		r_fire = 50;
		g_fire = 50;
		b_fire = 100;

		Fire.SetColor(i, r_fire, g_fire, b_fire);

		///��ʼ������
		x_fire = 0.015f * (rand() % 9);
		y_fire = 0.005f * (rand() % 9) - 1;
		z_fire = 0.005f * (rand() % 6);
		Fire.SetPosition(i, x_fire, y_fire, z_fire);

		///��ʼ���ٶ�
		vx_fire = 0.0000007f * (rand() % 9000 - rand() % 9000);
		vy_fire = 0.00000242f * (rand() % 9000);
		vz_fire = 0.0000001f * (rand() % 9000);

		Fire.SetVelocity(i, vx_fire, vy_fire, vz_fire);

		///��ʼ�����ٶ�
		ax_fire = 0;
		ay_fire = -0.0002;
		az_fire = 0;

		Fire.SetAcceleration(i, ax_fire, ay_fire, az_fire);
		///��ʼ����������
		lifetime_fire = 100;
		Fire.SetLifeTime(i, lifetime_fire);

		///��ʧ�ٶ�
		decz_fire = 0.05*(rand() % 50);
		Fire.SetDec(i, decz_fire);

		///��ʼ����С
		Fire.SetSize(i, 0.02f);
	}
	return true;
}


void UpdateFire()
{
	/** ����λ�� */
	x_fire += vx_fire;
	y_fire += vy_fire;
	z_fire += vz_fire;

	/** �����ٶ� */
	vy_fire += ay_fire;

	/** ��������ʱ�� */
	lifetime_fire -= decz_fire;

	/** ���������ʧ���������� */
	if (y_fire <= -1 || lifetime_fire <= 0)
	{
		/** ��ʼ��λ�� */
		x_fire = 0.015f * (rand() % 9);
		y_fire = 0.005f * (rand() % 9) - 1;
		z_fire = 0.000001f * (rand() % 9000);

		/** ��ʼ���ٶ� */
		vx_fire = 0.0000007f * (rand() % 9000 - rand() % 9000);
		vy_fire = 0.00000242f * (rand() % 9000);
		vz_fire = 0.0000001f * (rand() % 90000);

		lifetime_fire = 100;
		decz_fire = 0.1 * (rand() % 50);
	}
}


void DrawFire()
{
	/** ������ */
	glDisable(GL_BLEND);
	glDisable(GL_LIGHTING);
	GLfloat emmision[] = { 1.0,0.5,0.0,0 };
	GLfloat initemit[] = { 0,0,0,1 };
	/*GLfloat oriam[4]; 
	glGetMaterialfv(GL_FRONT,GL_AMBIENT,oriam);
	GLfloat oridif[4];
	glGetMaterialfv(GL_FRONT,GL_DIFFUSE, oridif);
	GLfloat orispec[4];
	glGetMaterialfv(GL_FRONT, GL_SPECULAR, orispec);
	
	/*GLfloat ambi[] = { 0.0,0.0,0.0,1.0 };
	GLfloat dif[] = { 0.0,0.0,0.0,1.0 };
	GLfloat spec[] = { 0.0,0.0,0.0,1.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambi);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emmision);*/
	glColor3f(1.0,0.5,0.0);
	glBindTexture(GL_TEXTURE_2D, texName[2]);
	glPushMatrix();
	glTranslatef(0.0f, 1.0f, -4.0f);
	for (int i = 0; i<Fire.GetNumOfParticle(); ++i)
	{
		/** ������ӵ��������� */
		Fire.GetAll(i, r_fire, g_fire, b_fire, x_fire, y_fire, z_fire, vx_fire, vy_fire, vz_fire, ax_fire, ay_fire, az_fire, psize_fire, lifetime_fire, decz_fire);
		//glLoadIdentity();
		//glColor4ub(r_fire, g_fire, b_fire, 255);
		glNormal3f(0.0f, 0.0f, 1.0f);   /**< ���巨�߷��� */
										/** �������� */
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f((x_fire - psize_fire)*lifetime_fire/100, (y_fire - psize_fire)*lifetime_fire / 100, z_fire*lifetime_fire / 100);
		glTexCoord2f(1.0f, 0.0f); glVertex3f((x_fire - psize_fire)*lifetime_fire / 100, (y_fire + psize_fire)*lifetime_fire / 100, z_fire*lifetime_fire / 100);
		glTexCoord2f(1.0f, 1.0f); glVertex3f((x_fire + psize_fire)*lifetime_fire / 100, (y_fire + psize_fire)*lifetime_fire / 100, z_fire*lifetime_fire / 100);
		glTexCoord2f(0.0f, 1.0f); glVertex3f((x_fire + psize_fire)*lifetime_fire / 100, (y_fire - psize_fire)*lifetime_fire / 100, z_fire*lifetime_fire / 100);
		glEnd();
		/** ������������ */
		UpdateFire();
		Fire.SetAll(i, r_fire, g_fire, b_fire, x_fire, y_fire, z_fire, vx_fire, vy_fire, vz_fire, ax_fire, ay_fire, az_fire, psize_fire, lifetime_fire, decz_fire);
	}
	glPopMatrix();
	//glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, initemit);
	/*glMaterialfv(GL_FRONT, GL_AMBIENT, oriam);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, oridif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, orispec);*/
	glEnable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glutPostRedisplay();//������Ⱦ����
}