#include <windows.h>
#define GLUT_DISABLE_ATEXIT_HACK
#ifndef _GLUTANDSTDINLUDE_
#define _GLUTANDSTDINLUDE_
#include <stdlib.h>
#include <GL/GLUT.H>
#endif
#include <math.h>
#include <stdio.h>
#include <vector>
#include <string>
using namespace std;
class ObjLoader
{
public:
	double maxH;
	double y;
	float lowestpt;
	ObjLoader();
	int direction;
	float* ObjLoader::calculateN(float* coord1, float* coord2, float* coord3);
	int ObjLoader::Load(char *filename);
	void ObjLoader::Draw();
	void ObjLoader::update(float x, float y, float z);
	void ObjLoader::higher();
	void ObjLoader::lower();
	float* normals;
	float* Faces_Triangles;
	float* vertexBuffer;
	long Points;
	long Triangles;

};