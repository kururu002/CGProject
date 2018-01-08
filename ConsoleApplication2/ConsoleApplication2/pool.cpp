#ifndef _POOLH_
#include"pool.h"
#endif
#define BIRDPOS 2
void CPool::Initialize(int xSize, int zSize, float OscillatorDistance, float OscillatorWeight, float Damping, float TextureStretchX, float TextureStretchZ)
{
	//assign member variables
	m_xSize = xSize;
	m_zSize = zSize;
	m_NumOscillators = m_xSize*m_zSize;
	m_OscillatorDistance = OscillatorDistance;
	m_OscillatorWeight = OscillatorWeight;
	m_Damping = Damping;

	//temporary vector for indies:
	std::vector <GLuint> IndexVect;  //we first put the indices into this vector, then copy them to the array below


	m_Oscillators = new SOscillator[m_NumOscillators];
	IndexVect.clear();  //to be sure it is empty
	for (int zc = 0; zc < m_zSize; zc++)
		for (int xc = 0; xc < m_xSize; xc++)
		
		{
			m_Oscillators[xc + zc*m_xSize].x = m_OscillatorDistance*float(xc);
			m_Oscillators[xc + zc*m_xSize].y = 0.0f;
			m_Oscillators[xc + zc*m_xSize].z = m_OscillatorDistance*float(zc);

			m_Oscillators[xc + zc*m_xSize].nx = 0.0f;
			m_Oscillators[xc + zc*m_xSize].ny = 1.0f;
			m_Oscillators[xc + zc*m_xSize].nz = 0.0f;

			m_Oscillators[xc + zc*m_xSize].u = (float)xc / (float)m_xSize * TextureStretchX;
			m_Oscillators[xc + zc*m_xSize].v = 1.0f - (float)zc / (float)m_zSize * TextureStretchZ;

			m_Oscillators[xc + zc*m_xSize].UpSpeed = 0;

			//create two triangles:
			if ((xc < m_xSize - 1) && (zc < m_zSize - 1))
			{
				IndexVect.push_back(xc + zc*m_xSize);
				IndexVect.push_back((xc + 1) + zc*m_xSize);
				IndexVect.push_back((xc + 1) + (zc + 1)*m_xSize);

				IndexVect.push_back(xc + zc*m_xSize);
				IndexVect.push_back((xc + 1) + (zc + 1)*m_xSize);
				IndexVect.push_back(xc + (zc + 1)*m_xSize);
			}

		}

	//copy the indices:
	m_Indices = new GLuint[IndexVect.size()];  //allocate the required memory
	for (int i = 0; i < IndexVect.size(); i++)
	{
		m_Indices[i] = IndexVect[i];
	}


	m_NumIndices = IndexVect.size();
	IndexVect.clear();  //no longer needed, takes only memory
}

void CPool::Reset()
{
	for (int xc = 0; xc < m_xSize; xc++)
		for (int zc = 0; zc < m_zSize; zc++)
		{
			m_Oscillators[xc + zc*m_xSize].y = 0.0f;
			m_Oscillators[xc + zc*m_xSize].UpSpeed = 0.0f;
			m_Oscillators[xc + zc*m_xSize].nx = 0.0f;
			m_Oscillators[xc + zc*m_xSize].ny = 1.0f;
			m_Oscillators[xc + zc*m_xSize].nz = 0.0f;
		}
}

void CPool::AffectOscillator(int xPos, int zPos, float deltaY)
{
	if ((xPos >= 0) && (xPos < m_xSize) && (zPos >= 0) && (zPos < m_zSize))
	{
		if (m_Oscillators[xPos + zPos*m_xSize].y > -0.15)  //THIS LINE IS REQUIRED FOR FOUNTAINS WITH MANY DROPS!!!
			m_Oscillators[xPos + zPos*m_xSize].y += deltaY;
	}
}


void CPool::Update(float deltaTime)
{
	//********
	// Here we do the physical calculations: 
	// The m_Oscillators are moved according to their neighbors.
	// The parameter bEndIsFree indicates, whether the m_Oscillators in the edges can move or not.
	// The new position may be assigned not before all calculations are done!

	// PLEASE NOTE: THESE ARE APPROXIMATIONS AND I KNOW THIS! (but is looks good, doesn't it?)

	//if we use two loops, it is a bit easier to understand what I do here.

	int xc, zc;

	for (xc = 0; xc < m_xSize; xc++)
	{
		for (zc = 0; zc < m_zSize; zc++)
		{
			int ArrayPos = xc + zc*m_xSize;

			m_Oscillators[ArrayPos].newY = m_Oscillators[ArrayPos].y;

			//check, if this oscillator is on an edge (=>end)
			if ((xc == 0) || (xc == m_xSize - 1) || (zc == 0) || (zc == m_zSize - 1))
				;//TBD: calculating m_Oscillators at the edge (if the end is free)
			else
			{
				//calculate the new speed:


				//Change the speed (=accelerate) according to the oscillator's 4 direct neighbors:
				float AvgDifference = m_Oscillators[ArrayPos - 1].y				//left neighbor
					+ m_Oscillators[ArrayPos + 1].y				//right neighbor
					+ m_Oscillators[ArrayPos - m_xSize].y  //upper neighbor
					+ m_Oscillators[ArrayPos + m_xSize].y  //lower neighbor
					- 4 * m_Oscillators[ArrayPos].y;				//subtract the pos of the current osc. 4 times	

				m_Oscillators[ArrayPos].UpSpeed += AvgDifference*(deltaTime / m_OscillatorWeight);

				m_Oscillators[ArrayPos].UpSpeed *= (1.0f - m_Damping);

				//calculate the new position, but do not yet store it in "y" (this would affect the calculation of the other osc.s)
				m_Oscillators[ArrayPos].newY += m_Oscillators[ArrayPos].UpSpeed*deltaTime;



			}
		}
	}

	//copy the new position to y:
	for (xc = 0; xc < m_xSize; xc++)
	{
		for (int zc = 0; zc < m_zSize; zc++)
		{
			m_Oscillators[xc + zc*m_xSize].y = m_Oscillators[xc + zc*m_xSize].newY;
		}
	}
	//calculate new normal vectors (according to the oscillator's neighbors):
	for (xc = 0; xc < m_xSize; xc++)
	{
		for (int zc = 0; zc < m_zSize; zc++)
		{
			///
			//Calculating the normal:
			//Take the direction vectors 1.) from the left to the right neighbor 
			// and 2.) from the upper to the lower neighbor.
			//The vector orthogonal to these 

			SF3dVector u, v, p1, p2;	//u and v are direction vectors. p1 / p2: temporary used (storing the points)

			if (xc > 0) p1 = F3dVector(m_Oscillators[xc - 1 + zc*m_xSize].x,
				m_Oscillators[xc - 1 + zc*m_xSize].y,
				m_Oscillators[xc - 1 + zc*m_xSize].z);
			else
				p1 = F3dVector(m_Oscillators[xc + zc*m_xSize].x,
					m_Oscillators[xc + zc*m_xSize].y,
					m_Oscillators[xc + zc*m_xSize].z);
			if (xc < m_xSize - 1)
				p2 = F3dVector(m_Oscillators[xc + 1 + zc*m_xSize].x,
					m_Oscillators[xc + 1 + zc*m_xSize].y,
					m_Oscillators[xc + 1 + zc*m_xSize].z);
			else
				p2 = F3dVector(m_Oscillators[xc + zc*m_xSize].x,
					m_Oscillators[xc + zc*m_xSize].y,
					m_Oscillators[xc + zc*m_xSize].z);
			u = p2 - p1; //vector from the left neighbor to the right neighbor
			if (zc > 0) p1 = F3dVector(m_Oscillators[xc + (zc - 1)*m_xSize].x,
				m_Oscillators[xc + (zc - 1)*m_xSize].y,
				m_Oscillators[xc + (zc - 1)*m_xSize].z);
			else
				p1 = F3dVector(m_Oscillators[xc + zc*m_xSize].x,
					m_Oscillators[xc + zc*m_xSize].y,
					m_Oscillators[xc + zc*m_xSize].z);
			if (zc < m_zSize - 1)
				p2 = F3dVector(m_Oscillators[xc + (zc + 1)*m_xSize].x,
					m_Oscillators[xc + (zc + 1)*m_xSize].y,
					m_Oscillators[xc + (zc + 1)*m_xSize].z);
			else
				p2 = F3dVector(m_Oscillators[xc + zc*m_xSize].x,
					m_Oscillators[xc + zc*m_xSize].y,
					m_Oscillators[xc + zc*m_xSize].z);
			v = p2 - p1; //vector from the upper neighbor to the lower neighbor
						 //calculate the normal:
			SF3dVector normal = Normalize3dVector(CrossProduct(&u, &v));

			//assign the normal:
			if (normal.y > 0.0)   //normals always look upward!
			{
				m_Oscillators[xc + zc*m_xSize].nx = normal.x;
				m_Oscillators[xc + zc*m_xSize].ny = normal.y;
				m_Oscillators[xc + zc*m_xSize].nz = normal.z;
			}
			else
			{
				m_Oscillators[xc + zc*m_xSize].nx = -normal.x;
				m_Oscillators[xc + zc*m_xSize].ny = -normal.y;
				m_Oscillators[xc + zc*m_xSize].nz = -normal.z;
			}
		}
	}

}


void CPool::Render()
{
	//There might be more vertex arrays. Thus, pass the pointers each time you use them:
	glVertexPointer(3,   //3 components per vertex (x,y,z)
		GL_FLOAT,
		sizeof(SOscillator),
		m_Oscillators);
	glTexCoordPointer(2,
		GL_FLOAT,
		sizeof(SOscillator),
		&m_Oscillators[0].u);


	glNormalPointer(GL_FLOAT,
		sizeof(SOscillator),
		&m_Oscillators[0].nx);  //Pointer to the first normal

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	//Draw the array:
	glColor3f(1.0, 1.0, 1.0);
	glDrawElements(GL_TRIANGLES, //mode
		m_NumIndices,  //count, ie. how many indices
		GL_UNSIGNED_INT, //type of the index array
		m_Indices);


}

float CPool::GetOscillatorDistance()
{
	return m_OscillatorDistance;
}
#define RandomFactor 2.0

GLfloat GetRandomFloat(GLfloat range)
{
	return (GLfloat)rand() / (GLfloat)RAND_MAX * range * RandomFactor;
}

void CDrop::SetConstantSpeed(SF3dVector NewSpeed)
{
	ConstantSpeed = NewSpeed;
}

void CDrop::SetAccFactor(GLfloat NewAccFactor)
{
	AccFactor = NewAccFactor;
}

void CDrop::SetTime(GLfloat NewTime)
{
	time = NewTime;
}

void CDrop::GetNewPosition(SF3dVector * PositionVertex, float dtime, CPool * pPool, CAirFountain * pAirFountain)
{
	SF3dVector Position;
	time += dtime*20.0f;

	if (time > 0.0f)
	{
		Position.x = ConstantSpeed.x * time;
		Position.y = ConstantSpeed.y * time - AccFactor * time * time;
		Position.z = ConstantSpeed.z * time;
		PositionVertex->x = Position.x;
		PositionVertex->y = Position.y;
		PositionVertex->z = Position.z;
		if (Position.y < -BIRDPOS)
		{
			/*the drop has fallen into the water. The problem is now, that we cannot
			set time to 0.0, because if there are more "DropsPerRay" than "TimeNeeded" (See InitFountain())
			several drops would be seen as one. Check it out.
			*/
			time = time - int(time);
			if (time > 0.0) time -= 1.0;

			//The drop creates a little wave in the pool:
			int OscillatorX = (int)((Position.x + pAirFountain->Position.x) / pPool->GetOscillatorDistance());
			int OscillatorZ = (int)((Position.z + pAirFountain->Position.z) / pPool->GetOscillatorDistance());
			pPool->AffectOscillator(OscillatorX,
				OscillatorZ,
				-0.1);  //change this to make the waves stronger/weaker



		}
	}
	else
	{
		PositionVertex->x = 0.0;
		PositionVertex->y = 0.0;
		PositionVertex->z = 0.0;
	}
}
void CAirFountain::Initialize(GLint Steps, GLint RaysPerStep, GLint DropsPerRay,
	GLfloat AngleOfDeepestStep,
	GLfloat AngleOfHighestStep,
	GLfloat RandomAngleAddition,
	GLfloat AccFactor)
{
	//This function needn't be and isn't speed optimized

	m_NumDropsComplete = Steps*RaysPerStep*DropsPerRay;

	FountainDrops = new CDrop[m_NumDropsComplete];
	FountainVertices = new SF3dVector[m_NumDropsComplete];
	SF3dVector NewSpeed;
	GLfloat DropAccFactor; //different from AccFactor because of the random change
	GLfloat TimeNeeded;
	GLfloat StepAngle; //Angle, which the ray gets out of the fountain with
	GLfloat RayAngle;	//Angle you see when you look down on the fountain
	GLint i, j, k;
	for (k = 0; k <Steps; k++)
	{
		for (j = 0; j < RaysPerStep; j++)
		{
			for (i = 0; i < DropsPerRay; i++)
			{
				DropAccFactor = AccFactor + GetRandomFloat(0.005);
				if (Steps > 1)
					StepAngle = AngleOfDeepestStep + (AngleOfHighestStep - AngleOfDeepestStep)
					* GLfloat(k) / (Steps - 1) + GetRandomFloat(RandomAngleAddition);
				else
					StepAngle = AngleOfDeepestStep + GetRandomFloat(RandomAngleAddition);

				//This is the speed caused by the step:
				NewSpeed.x = cos(StepAngle * PI / 180.0) * (0.2 + 0.04*k);
				NewSpeed.y = sin(StepAngle * PI / 180.0) * (0.2 + 0.04*k);
				//This is the speed caused by the ray:

				RayAngle = (GLfloat)j / (GLfloat)RaysPerStep * 360.0 +90.0;  //+12.0 causes a rotation (12?
																			  //for the next computations "NewSpeed.x" is the radius. Care! Dont swap the two
																			  //lines, because the second one changes NewSpeed.x!
				NewSpeed.z = NewSpeed.x * sin(RayAngle * PI / 180.0);
				NewSpeed.x = NewSpeed.x * cos(RayAngle * PI / 180.0);

				NewSpeed.x *= 3.0f;
				NewSpeed.y *= 3.0f;
				NewSpeed.z *= 3.0f;


				//Calculate how many steps are required, that a drop comes out and falls down again
				TimeNeeded = NewSpeed.y / DropAccFactor+(-NewSpeed.y+sqrt(NewSpeed.y*NewSpeed.y+2*DropAccFactor*BIRDPOS))/DropAccFactor;//h=v0t+1/2*at^2
				FountainDrops[i + j*DropsPerRay + k*DropsPerRay*RaysPerStep].SetConstantSpeed(NewSpeed);
				FountainDrops[i + j*DropsPerRay + k*DropsPerRay*RaysPerStep].SetAccFactor(DropAccFactor);
				FountainDrops[i + j*DropsPerRay + k*DropsPerRay*RaysPerStep].SetTime(TimeNeeded * i / DropsPerRay);
			}
		}
	}

}

void CAirFountain::Update(float dtime, CPool * pPool)
{
	for (int i = 0; i < m_NumDropsComplete; i++)
		FountainDrops[i].GetNewPosition(&FountainVertices[i], dtime, pPool, this);

}

void CAirFountain::Render()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	glVertexPointer(3,			//x,y,z-components
		GL_FLOAT,	//data type of SVertex
		0,			//the vertices are tightly packed
		FountainVertices);
	glPushMatrix();
	glTranslatef(Position.x, Position.y+2.0, Position.z);
	glDrawArrays(GL_POINTS,
		0,
		m_NumDropsComplete);

	glPopMatrix();
}

void CAirFountain::Delete()
{
	delete[] FountainDrops;
	delete[] FountainVertices;
}
