#ifndef _GLUTANDSTDINLUDE_
#define _GLUTANDSTDINLUDE_
#include <stdlib.h>
#include <GL/GLUT.H>
#endif
#include<vector>
#include<math.h>
#ifndef _3DVEC_
#define _3DVEC_
struct SF3dVector  //Float 3d-vect, normally used
{
	GLfloat x, y, z;
};
struct SF2dVector
{
	GLfloat x, y;
};


SF3dVector F3dVector(GLfloat x, GLfloat y, GLfloat z);
SF3dVector AddF3dVectors(SF3dVector * u, SF3dVector * v);
void AddF3dVectorToVector(SF3dVector * Dst, SF3dVector * V2);


GLfloat GetF3dVectorLength(SF3dVector * v);

SF3dVector CrossProduct(SF3dVector * u, SF3dVector * v);

SF3dVector Normalize3dVector(SF3dVector v);

SF3dVector operator+ (SF3dVector v, SF3dVector u);

SF3dVector operator- (SF3dVector v, SF3dVector u);

SF3dVector operator* (SF3dVector v, float r);
SF3dVector operator/ (SF3dVector v, float r);

float operator* (SF3dVector v, SF3dVector u);	//dot product
#endif