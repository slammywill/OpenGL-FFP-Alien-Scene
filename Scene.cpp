#include <cmath>
#include <GL/freeglut.h>
#include "Models.h"
#include <iostream>
#include "loadBMP.h"
using namespace std;

// Globals
float angle = 0, look_x = 00., look_z = -1., eye_x, eye_z;  //Camera parameters
GLuint txId[3];   //Texture ids
GLUquadricObj* q;
float white[4] = { 1.0, 1.0, 1.0, 1.0 };
float green[4] = { 0.0, 1.0, 0.0, 1.0 };
float red[4]   = { 1.0, 0.0, 0.0, 1.0 };
float height = 2;
bool animationActive = false;
bool goingUp = true;

void loadTextures() {
	glGenTextures(3, txId);
	glBindTexture(GL_TEXTURE_2D, txId[0]);  //Use this texture name for the following OpenGL texture
	loadBMP("dirt.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Linear Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, txId[1]);  //Use this texture name for the following OpenGL texture
	loadBMP("metal2.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Linear Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, txId[2]);  //Use this texture name for the following OpenGL texture
	loadBMP("sky.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Linear Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}

void timer(int value) {

	if (animationActive) {
		if (height < 20 && goingUp) {
			height += 1;
		}
		else if (height >= 4 && !goingUp) {
			height -= 1;
		}
		else {
			goingUp = !goingUp;
			animationActive = false;
		}
	}

	glutTimerFunc(1000 / 60, timer, value);
	glutPostRedisplay();
}



void initialize(void) {

	loadTextures();

	glClearColor(0, 1, 1, 1);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, green);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 0);

	glEnable(GL_LIGHT2);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, red);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 30.0);
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 0);

	GLfloat mat_ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat mat_shininess[] = { 100.0f };
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);  //Default
	glEnable(GL_COLOR_MATERIAL);

	q = gluNewQuadric();
	gluQuadricDrawStyle(q, GLU_FILL);
	gluQuadricNormals(q, GLU_SMOOTH);
	glEnable(GL_TEXTURE_2D);
	gluQuadricTexture(q, GL_TRUE);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60., 1.0, 2.0, 1000.0);
}


void special(int key, int x, int y)
{
	switch (key) { 
	case GLUT_KEY_LEFT:
		angle -= 5 * 3.141592 / 180;
		break;
	case GLUT_KEY_RIGHT:
		angle += 5 * 3.141592 / 180;
		break;
	case GLUT_KEY_DOWN:
		eye_x -= 0.1 * sin(angle);
		eye_z += 0.1 * cos(angle);
		break;
	case GLUT_KEY_UP:
		eye_x += 0.1 * sin(angle);
		eye_z -= 0.1 * cos(angle);
		break;
	}
	look_x = eye_x + 100 * sin(angle);
	look_z = eye_z - 100 * cos(angle);
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 32:
		animationActive = true;
		break;
	}
}


void display(void) {
	float light[] = { 0.0f, 100.0f, 0.0f, 0.0f };  //Light's position (directly above the origin
	float shadowMat[16] = { light[1], 0, 0, 0, -light[0], 0, -light[2],-1,
		0, 0, light[1], 0, 0, 0, 0, light[1] };


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(eye_x, 2, eye_z, look_x, 2, look_z, 0, 1, 0);
	glLightfv(GL_LIGHT0, GL_POSITION, light);

	displayFloor(&txId[0]);
	ufo(shadowMat, &txId[1], q, height);
	skydome(50, &txId[2], q);
	alien(shadowMat);
    hill(1.1, 0.5, 50., q);
	egg();

	glutSwapBuffers();       //Double buffered animation
}



int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(5, 5);
	glutCreateWindow("Scene");
	initialize();

	glutDisplayFunc(display);
	glutTimerFunc(1000 / 60, timer, 0);
	glutSpecialFunc(special);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}
