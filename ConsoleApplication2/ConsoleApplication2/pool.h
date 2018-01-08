#define GLUT_DISABLE_ATEXIT_HACK
#define PI 3.14
#ifndef _GLUTANDSTDINLUDE_
#define _GLUTANDSTDINLUDE_
#include <stdlib.h>
#include <GL/GLUT.H>
#endif
#ifndef _3DVEC_
#include"3dvector.h"
#endif
#ifndef _POOLH_

struct SOscillator
{
	GLfloat x, y, z;     //position
	GLfloat nx, ny, nz;  //normal vector
	GLfloat u, v;	   //texture coordinates

	GLfloat UpSpeed;
	GLfloat newY;
};

class CPool
{
protected:
	//vertex data for the waves:
	SOscillator * m_Oscillators;
	GLuint * m_Indices;


	int m_NumOscillators;  //x*z
	int m_xSize, m_zSize;

	int m_NumIndices;

	float m_OscillatorDistance;
	float m_OscillatorWeight;
	float m_Damping;


public:
	float GetOscillatorDistance();
	void Initialize(int xSize, int zSize, float OscillatorDistance, float OscillatorWeight, float Damping, float TextureStretchX, float TextureStretchZ);
	void Reset();
	void AffectOscillator(int xPos, int zPos, float deltaY);
	void Update(float deltaTime);
	void Render();
};

class CAirFountain;

class CDrop
{
private:
	GLfloat time;  //How many steps the drop was "outside", when it falls into the water, time is set back to 0
	SF3dVector ConstantSpeed;  //See the fountain doc for explanation of the physics
	GLfloat AccFactor;
public:
	void SetConstantSpeed(SF3dVector NewSpeed);
	void SetAccFactor(GLfloat NewAccFactor);
	void SetTime(GLfloat NewTime);
	void GetNewPosition(SF3dVector * PositionVertex, float dtime, CPool * pPool, CAirFountain * pAirFountain);
};
class CAirFountain
{
protected:
	SF3dVector * FountainVertices;
	CDrop * FountainDrops;
public:
	SF3dVector Position;
	void Render();
	void Update(float dtime, CPool * pPool);
	GLint m_NumDropsComplete;
	void Initialize(GLint Steps, GLint RaysPerStep, GLint DropsPerRay,
		GLfloat AngleOfDeepestStep,
		GLfloat AngleOfHighestStep,
		GLfloat RandomAngleAddition,
		GLfloat AccFactor);
	void Delete();
};
#define _POOLH_
#endif