#include "loader.h"
#include <fstream>
#include <iostream>
using namespace std;


#define objH 10
#define objX -1
#define objZ 0
#define perVertex 3
#define perTiangle 9
#define GRAV 9.8
#define STEP 2
#define minH 0.01
#define SPEED 0.01
/** 婥恇燴 */


ObjLoader::ObjLoader()

{
	this->maxH = objH;
	this->direction = -1;
	this->y = objH;
	this->lowestpt = 0.0;
	this->Triangles = 0;
	this->Points = 0;
}

float* ObjLoader::calculateN(float *coord1, float *coord2, float *coord3)
{
	float va[3], vb[3], vr[3], val;
	va[0] = coord1[0] - coord2[0];
	va[1] = coord1[1] - coord2[1];
	va[2] = coord1[2] - coord2[2];

	vb[0] = coord1[0] - coord3[0];
	vb[1] = coord1[1] - coord3[1];
	vb[2] = coord1[2] - coord3[2];

	vr[0] = va[1] * vb[2] - vb[1] * va[2];
	vr[1] = vb[0] * va[2] - va[0] * vb[2];
	vr[2] = va[0] * vb[1] - vb[0] * va[1];

	val = sqrt(vr[0] * vr[0] + vr[1] * vr[1] + vr[2] * vr[2]);

	float norm[3];
	norm[0] = vr[0] / val;
	norm[1] = vr[1] / val;
	norm[2] = vr[2] / val;


	return norm;
}


int ObjLoader::Load(char* filename)
{
	bool flag = true;
	string line;
	ifstream objFile(filename);
	if (objFile.is_open())
	{
		objFile.seekg(0, ios::end);
		long fileSize = objFile.tellg();
		objFile.seekg(0, ios::beg);

		vertexBuffer = (float*)malloc(fileSize);
		Faces_Triangles = (float*)malloc(fileSize * sizeof(float));
		normals = (float*)malloc(fileSize * sizeof(float));

		int triangle_index = 0;
		int normal_index = 0;

		while (!objFile.eof())
		{
			getline(objFile, line);

			if (line.c_str()[0] == 'v')
			{
				line[0] = ' ';
				sscanf_s(line.c_str(), "%f %f %f ",
					&vertexBuffer[Points],
					&vertexBuffer[Points + 1],
					&vertexBuffer[Points + 2]);
				if (flag)lowestpt = vertexBuffer[Points + 1];
				flag = false;
				if (vertexBuffer[Points + 1] < lowestpt)lowestpt = vertexBuffer[Points + 1];
				Points += perVertex;
			}
			if (line.c_str()[0] == 'f')
			{
				line[0] = ' ';

				int vertexNumber[4] = { 0, 0, 0 };
				sscanf_s(line.c_str(), "%i%i%i",
					&vertexNumber[0],
					&vertexNumber[1],
					&vertexNumber[2]);

				vertexNumber[0] -= 1;
				vertexNumber[1] -= 1;
				vertexNumber[2] -= 1;
				int tCounter = 0;
				for (int i = 0; i < perVertex; i++)
				{
					Faces_Triangles[triangle_index + tCounter] = vertexBuffer[3 * vertexNumber[i]];
					Faces_Triangles[triangle_index + tCounter + 1] = vertexBuffer[3 * vertexNumber[i] + 1];
					Faces_Triangles[triangle_index + tCounter + 2] = vertexBuffer[3 * vertexNumber[i] + 2];
					tCounter += perVertex;
				}

				float coord1[3] = { Faces_Triangles[triangle_index], Faces_Triangles[triangle_index + 1],Faces_Triangles[triangle_index + 2] };
				float coord2[3] = { Faces_Triangles[triangle_index + 3],Faces_Triangles[triangle_index + 4],Faces_Triangles[triangle_index + 5] };
				float coord3[3] = { Faces_Triangles[triangle_index + 6],Faces_Triangles[triangle_index + 7],Faces_Triangles[triangle_index + 8] };
				float *norm = this->calculateN(coord1, coord2, coord3);

				tCounter = 0;
				for (int i = 0; i < perVertex; i++)
				{
					normals[normal_index + tCounter] = norm[0];
					normals[normal_index + tCounter + 1] = norm[1];
					normals[normal_index + tCounter + 2] = norm[2];
					tCounter += perVertex;
				}

				triangle_index += perTiangle;
				normal_index += perTiangle;
				Triangles += perTiangle;
			}
		}
		objFile.close();
	}
	else
	{
		cout << "Cannot file";
	}
	return 0;
}



void ObjLoader::Draw()
{
	
	glShadeModel(GL_SMOOTH);
	//glEnable(GL_COLOR_MATERIAL);
	GLfloat mat_ambient[] = { 0.24725	,0.1995	,0.0745, 1.000000 };
	GLfloat mat_diffuse[] = { 0.75164,	0.60648,	0.22648, 1.000000 };
	GLfloat mat_specular[] = { 0.628281	,0.555802	,0.366065, 1.000000 };
	GLfloat mat_shininess[] = { 51.2 }; //材质RGBA镜面指数，数值在0～128范围内

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	//glEnable(GL_LIGHTING); //启动光照
	//glEnable(GL_LIGHT0); //使第一盏灯有效
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, Faces_Triangles);
	glNormalPointer(GL_FLOAT, 0, normals);
	glDrawArrays(GL_TRIANGLES, 0, Triangles);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glShadeModel(GL_FLAT);
}



void ObjLoader::update(float x,float y,float z) {

	glPushMatrix();
	//glLoadIdentity();
	glTranslatef(x, y, z);
	Draw();
	glPopMatrix();
}