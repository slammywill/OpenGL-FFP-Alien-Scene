#include <GL/freeglut.h>
#include <cmath>

float rotRoundBody = 0;


void displayFloor(GLuint* texturePtr) {
	float white[] = {1, 1, 1, 1};

	glEnable(GL_TEXTURE_2D);
	glColor3f(1, 1, 1);
	glBindTexture(GL_TEXTURE_2D, *texturePtr);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);
	float texScale = 0.1;
	glPushMatrix();
		glBegin(GL_QUADS);
			for(int i = -200; i < 200; i++)
	{
		for(int j = -200;  j < 200; j++)
		{
			glTexCoord2f(i * texScale, j * texScale);
			glVertex3f(i , 0, j);
			glTexCoord2f(i * texScale, (j+1) * texScale);
			glVertex3f(i, 0, j+1);
			glTexCoord2f((i+1) * texScale, (j+1) * texScale);
			glVertex3f(i+1, 0, j+1);
			glTexCoord2f((i+1) * texScale, j * texScale);
			glVertex3f(i+1, 0, j);
		}
	}
		glEnd();
	glPopMatrix();
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glDisable(GL_TEXTURE_2D);
}


void ufo(float shadowMat[], GLuint* texturePtr, GLUquadricObj* q, float height) {
	float mainRad = 5;
	rotRoundBody+= 2;
	rotRoundBody > 360 ? 0 : rotRoundBody;
	float spotPos[] = { 0, 1, 0, 1 };
	float spotDir[] = { 0, -1, 0 }; // Spotlight direction pointing outward
	float c[3] = { 0., height, -20. };

	// -------------- Body ------------------ //
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, *texturePtr);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glColor3f(1, 1, 1);
	glPushMatrix();
		glTranslatef(c[0], c[1], c[2]);
		glScalef(5, 1, 5);
		glRotatef(rotRoundBody, 0, 1, 0);
		glRotatef(-90., 1.0, 0., 0.0);
		gluSphere(q, 1., 50., 50.);
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	glColor3f(0.1, 0.07, 0);
	glPushMatrix();
	glMultMatrixf(shadowMat);
	glTranslatef(c[0], c[1], c[2]);
	glScalef(5, 1, 5);
	glutSolidSphere(1., 50., 50.);
	glPopMatrix();
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);

	// -------------- Cockpit ------------------ //

	glColor3f(.2, .8, .8);
	glPushMatrix();
	glTranslatef(c[0], c[1] + .6, c[2]);
	glScalef(2, 1, 2);
	gluSphere(q, 1., 50., 50.);
	glPopMatrix();


	// ---------------- Spinning Things ---------------- //

	for (int i = 0; i <= 1; i++) {
		glColor4f(0.8, 0.8, 0.8, 1);
		glPushMatrix();
		glTranslatef(c[0], c[1], c[2]);
		glRotatef(rotRoundBody + i * 180, 0, 1, 0);
		glTranslatef(0, 0, mainRad - 0.15);
		glutSolidCylinder(0.1, 0.2, 20, 20);
		glPopMatrix();

		i == 0 ? glColor4f(0, 1, 0, 1.) : glColor4f(1, 0, 0, 1);
		glPushMatrix();
		glTranslatef(c[0], c[1], c[2]);
		glRotatef(rotRoundBody + i * 180, 0, 1, 0);
		glTranslatef(0, 0, mainRad - 0.1);
		glutSolidCylinder(0.08, 0.2, 20, 20);
		glLightfv(GL_LIGHT2, GL_POSITION, spotPos);
		glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spotDir);
		glPopMatrix();
	}

	glPushMatrix();
	glTranslatef(c[0], c[1], c[2]);
	glRotatef(rotRoundBody, 0, 1, 0);
	glTranslatef(0, 0, mainRad + 0.5);
	glLightfv(GL_LIGHT1, GL_POSITION, spotPos);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotDir);
	glPopMatrix();

	/*glEnable(GL_TEXTURE_2D);*/

}

void skydome(float radius, GLuint* texturePtr, GLUquadricObj* q) {
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);
	glBindTexture(GL_TEXTURE_2D, *texturePtr);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glScalef(1., 1., 1.0);

	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
	glRotatef(-90., 1.0, 0., 0.0);
	gluSphere(q, radius, 50., 50.);
	glPopMatrix();
	glEnable(GL_CULL_FACE);
		

}

