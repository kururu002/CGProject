GLuint textureId;  //texture ID

void loadSkybox()
{
	glGenTextures(1, &textureId); 				// Create a Texture object
	glBindTexture(GL_TEXTURE_2D, textureId);		//Use the texture
	loadTGA("skyboxsun.tga");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void skybox()
{
	glDisable(GL_LIGHTING);
	GLfloat mat_ambient[] = { 1.0,1.0,1.0,1.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glBegin(GL_QUADS);
	glNormal3f(0.0, 1.0, 0.0); //bottom floor
	glTexCoord2f(0.25, 0); glVertex3f(-600.0, -600.0, 600.0);
	glTexCoord2f(0.5, 0); glVertex3f(600.0, -600.0, 600.0);
	glTexCoord2f(0.5, 0.33); glVertex3f(600.0, -600.0, -600.0);
	glTexCoord2f(0.25, 0.33); glVertex3f(-600.0, -600.0, -600.0);

	glNormal3f(0.0, -1.0, 0.0); //top wall
	glTexCoord2f(0.25, 1); glVertex3f(-600.0, 600.0, 600.0);
	glTexCoord2f(0.5, 1); glVertex3f(600.0, 600.0, 600.0);
	glTexCoord2f(0.5, 0.66); glVertex3f(600.0, 600.0, -600.0);
	glTexCoord2f(0.25, 0.66); glVertex3f(-600.0, 600.0, -600.0);

	glNormal3f(0.0, 0.0, 1.0); //front wall
	glTexCoord2f(0.25, 0.5); glVertex3f(-600.0, -600.0, -600.0);
	glTexCoord2f(0.5, 0.5); glVertex3f(600.0, -600.0, -600.0);
	glTexCoord2f(0.5, 0.66); glVertex3f(600.0, 600.0, -600.0);
	glTexCoord2f(0.25, 0.66); glVertex3f(-600.0, 600.0, -600.0);

	glNormal3f(1.0, 0.0, 0.0); //left wall
	glTexCoord2f(0, 0.5); glVertex3f(-600.0, -600.0, 600.0);
	glTexCoord2f(0.25, 0.5); glVertex3f(-600.0, -600.0, -600.0);
	glTexCoord2f(0.25, 0.66); glVertex3f(-600.0, 600.0, -600.0);
	glTexCoord2f(0, 0.66); glVertex3f(-600.0, 600.0, 600.0);

	glNormal3f(-1.0, 0.0, 0.0); //right wall
	glTexCoord2f(0.75, 0.5); glVertex3f(600.0, -600.0, 600.0);
	glTexCoord2f(0.5, 0.5); glVertex3f(600.0, -600.0, -600.0);
	glTexCoord2f(0.5, 0.66); glVertex3f(600.0, 600.0, -600.0);
	glTexCoord2f(0.75, 0.66); glVertex3f(600.0, 600.0, 600.0);

	glNormal3f(0.0, 0.0, -1.0); //back wall
	glTexCoord2f(1, 0.5); glVertex3f(-600.0, -600.0, 600.0);
	glTexCoord2f(0.75, 0.5); glVertex3f(600.0, -600.0, 600.0);
	glTexCoord2f(0.75, 0.66); glVertex3f(600.0, 600.0, 600.0);
	glTexCoord2f(1, 0.66); glVertex3f(-600.0, 600.0, 600.0);

	glEnd();
	glPopMatrix();
	glEnable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
}
