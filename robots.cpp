
//  ========================================================================
//  COSC363 Assignment Claire Barnaby
//	20/03/2017
//
//  FILE NAME: rooots.cpp
//
//  Program displays 2 robots and plays an animation
//  ========================================================================

#include <iostream>
#include <fstream>
#include <cmath> 
#include <GL/freeglut.h>
#include "loadTGA.h"
# define M_PI           3.14159265358979323846  /* pi */
using namespace std;


//--Globals ---------------------------------------------------------------
int cam_hgt = 4; //Camera height
float angle = 0.0;  //Rotation angle for viewing
float theta = 20.0;

float cam_x = 0.0;
float cam_y = 5.0;
float cam_z = 10.0;

float look_x = 0.0;
float look_y = 5.0;
float look_z = -1.0;
GLuint txId[5];
GLUquadricObj*	q;

float spot_rotate = 0.0;
float prop_angle = 0.0;
float droneY = 0;
float trapTime = 0.0;
float trap_angle = 0.0;
float vaseDropDistance = 20.0;
float cogDropDistance = 20.0;
float dropTime = 0.0;

float robotX = 0.0;
float robotZ = 0.0;
float wheelRotate = 0.0;
int botR = 0;
int upperBotR = 0;
int armsRot = 0;
float vaseX = -5.0;
float vaseZ = -5.0;
float vaseRot = 0;
float cogX = -6;
float cogZ = -7;
int cogR = 0;
float bigCogRotate = 0;

int armsRotYL = 0;
int armsRotYR = 0;

int step = 0;

int firstPerson = 0;

void loadTexture()
{
	glGenTextures(5, txId);
	glBindTexture(GL_TEXTURE_2D, txId[0]);
	loadTGA("robotEyeTex.tga");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, txId[1]);
	loadTGA("TexturesCom_MetalFloorsBare0049_1_seamless_S.tga");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, txId[2]);
	loadTGA("DD-Grunge-Stripes-Background-00900-Preview.tga");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, txId[3]);
	loadTGA("TexturesCom_MetalPainted0064_1_seamless_S.tga");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, txId[4]);
	loadTGA("skybox.tga");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

//--Draws a grid of lines on the floor plane -------------------------------
void drawLevel()
{

	glEnable(GL_TEXTURE_2D);
	glColor3f(1, 1, 1);
	glBindTexture(GL_TEXTURE_2D, txId[1]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	float texX = 0;
	float texY = 0;
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	for (int x = -15; x <= 15; x += 1)
	{
		texY = 0;
		for (int z = -15; z <= 15; z++)
		{
			if (!(z > 3 && z < 11 && x > 3 && x < 11)) {

				glTexCoord2f(texX, texY);
				glVertex3f(x, -0.1, z);

				glTexCoord2f(texX, texY + 0.25);
				glVertex3f(x, -0.1, z + 1);

				glTexCoord2f(texX + 0.25, texY + 0.25);
				glVertex3f(x + 1, -0.1, z + 1);

				glTexCoord2f(texX + 0.25, texY);
				glVertex3f(x + 1, -0.1, z);
			}
			texY += 0.25;
		}
		texX += 0.25;


	}

	glEnd();

	glDisable(GL_TEXTURE_2D);
	glColor3f(0.450, 0.439, 0.349);
	glPushMatrix(); //Leg
	glScalef(1, 15, 1);
	glTranslatef(-13, -0.7, -13);
	glutSolidCube(1.5);
	glPopMatrix();

	glPushMatrix(); //Leg
	glScalef(1, 15, 1);
	glTranslatef(14, -0.7, -13);
	glutSolidCube(1.5);
	glPopMatrix();

	glPushMatrix(); //Leg
	glScalef(1, 15, 1);
	glTranslatef(-13, -0.7, 14);
	glutSolidCube(1.5);
	glPopMatrix();

	glPushMatrix(); //Leg
	glScalef(1, 15, 1);
	glTranslatef(14, -0.7, 14);
	glutSolidCube(1.5);
	glPopMatrix();

}

void drawSkyBox() {
	glEnable(GL_TEXTURE_2D);
	glColor3f(1, 1, 1);
	glBindTexture(GL_TEXTURE_2D, txId[4]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glBegin(GL_QUADS);
	//floor
	glNormal3f(0, 1, 0);
	glTexCoord2f(0.49, 0.0);
	glVertex3f(-500, -500, -500);



	glTexCoord2f(0.49, 0.333);
	glVertex3f(-500, -500, 500);



	glTexCoord2f(0.31, 0.333);
	glVertex3f(500, -500, 500);



	glTexCoord2f(0.31, 0.0);
	glVertex3f(500, -500, -500);

	//roof
	glNormal3f(0, -1, 0);
	glTexCoord2f(0.5, 1);
	glVertex3f(-500, 500, -500);

	glTexCoord2f(0.5, 0.666);
	glVertex3f(-500, 500, 500);

	glTexCoord2f(0.3, 0.666);
	glVertex3f(500, 500, 500);

	glTexCoord2f(0.3, 1);
	glVertex3f(500, 500, -500);

	//left wall
	glNormal3f(1, 0, 0);
	glTexCoord2f(0.75, 0.34);
	glVertex3f(-500, -500, -500);

	glTexCoord2f(0.5, 0.34);
	glVertex3f(-500, -500, 500);

	glTexCoord2f(0.5, 0.666);
	glVertex3f(-500, 500, 500);

	glTexCoord2f(0.75, 0.666);
	glVertex3f(-500, 500, -500);


	//right wall
	glNormal3f(-1, 0, 0);

	glTexCoord2f(0.0, 0.34);
	glVertex3f(500, -500, -500);

	glTexCoord2f(0.25, 0.34);
	glVertex3f(500, -500, 500);


	glTexCoord2f(0.25, 0.666);
	glVertex3f(500, 500, 500);

	glTexCoord2f(0.0, 0.666);
	glVertex3f(500, 500, -500);

	//behind wall
	glNormal3f(0, 0, -1);

	glTexCoord2f(0.5, 0.34);
	glVertex3f(-500, -500, 500);

	glTexCoord2f(0.5, 0.666);
	glVertex3f(-500, 500, 500);


	glTexCoord2f(0.25, 0.666);
	glVertex3f(500, 500, 500);

	glTexCoord2f(0.25, 0.34);
	glVertex3f(500, -500, 500);

	//ahead wall
	glNormal3f(0, 0, 1);
	glTexCoord2f(0.75, 0.34);
	glVertex3f(-500, -500, -500);

	glTexCoord2f(0.75, 0.666);
	glVertex3f(-500, 500, -500);

	glTexCoord2f(1, 0.666);
	glVertex3f(500, 500, -500);
	glTexCoord2f(1, 0.34);
	glVertex3f(500, -500, -500);
	glEnd();



}

void drawTrapDoor(int startX, int startZ, int sizeX, int sizeZ) {

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, txId[3]);
	glColor3f(1, 1, 1);

	float texX = 0;
	float texY = 0;

	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);

	for (int x = startX; x <= sizeX; x += 1)
	{
		texY = 0;
		for (int z = startZ; z <= sizeZ; z++)
		{
			glTexCoord2f(texX, texY);
			glVertex3f(x, -0.1, z);
			glTexCoord2f(texX, texY + 0.25);
			glVertex3f(x, -0.1, z + 1);
			glTexCoord2f(texX + 0.25, texY + 0.25);
			glVertex3f(x + 1, -0.1, z + 1);
			glTexCoord2f(texX + 0.25, texY);
			glVertex3f(x + 1, -0.1, z);
			texY += 0.25;
		}
		texX += 0.25;
	}
	glEnd();

}

//--Draws a robot 1 constructed using GLUT objects ------------------
void drawRobot1(int shadow)
{
	glColor3f(0.2, 0.2, 0.2);

	if (shadow == 1) {
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
	}

	if (!shadow) {
		glColor3f(0.964, 0.866, 0.117); // yellow
	}
	else {
		glColor3f(0.2, 0.2, 0.2);
	}

	glPushMatrix();
	glTranslatef(0, -0.5, 0);
	glPushMatrix();
	glTranslatef(0, 1.2, 0);
	glRotatef(upperBotR, 0, 0, 1);
	glTranslatef(0, -1.2, 0);
	glPushMatrix(); //Head
	glScalef(0.6, 0.5, 0.8);
	glTranslatef(0, 7.5, 0);
	glutSolidCube(1.5);
	glPopMatrix();

	if (!shadow) {
		glEnable(GL_TEXTURE_2D);
		gluQuadricTexture(q, GL_TRUE);
		glColor3f(1, 1, 1);
	}

	glPushMatrix(); // left eye
	glScalef(0.8, 1, 0.8);
	glTranslatef(-0.5, 3.8, -0.5);
	glRotatef(-30, 1, 0, 0);
	glRotatef(-90, 0, 0, 1);
	gluSphere(q, 0.5, 36, 17);
	glPopMatrix();

	glPushMatrix(); // right eye
	glScalef(0.8, 1, -0.8);
	glTranslatef(-0.5, 3.8, -0.5);
	glRotatef(30, 1, 0, 0);
	glRotatef(-90, 0, 0, 1);
	gluSphere(q, 0.5, 36, 17);
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
	gluQuadricTexture(q, GL_FALSE);

	if (!shadow) {
		glColor3f(0.450, 0.439, 0.349);
	}
	else {
		glColor3f(0.2, 0.2, 0.2);
	}

	glPushMatrix(); //Neck
	glScalef(0.25, 1, 0.25);
	glTranslatef(0, 3.5, 0);
	glutSolidCube(1.5);
	glPopMatrix();

	if (!shadow) {
		glColor3f(0.964, 0.866, 0.117); // yellow
	}
	else {
		glColor3f(0.2, 0.2, 0.2);
	}

	glPushMatrix(); //Body
	glTranslatef(0, 2.25, 0);
	glutSolidCube(1.5);
	glPopMatrix();

	glPushMatrix(); // left arm bearing
	glTranslatef(-0.2, 2.8, -0.85);
	glutSolidSphere(0.2, 20, 20);
	glPopMatrix();

	glPushMatrix(); //left arm
	glTranslatef(-0.8, 1.9, -1);
	glTranslatef(0.7, 1, 0);
	glRotatef(armsRotYL, 0, 1, 0);
	glRotatef(-30 - armsRot, 0, 0, 1);
	glTranslatef(0, -1, 0);
	glRotatef(7, 1, 0, 0);
	glScalef(0.25, 2, 0.25);
	glutSolidCube(1);
	glPopMatrix();

	if (!shadow) {
		glColor3f(0.450, 0.439, 0.349);
	}
	else {
		glColor3f(0.2, 0.2, 0.2);
	}

	/*glPushMatrix(); //left spike
	glTranslatef(-1.25, 1.1, -1.1);
	glRotatef(-25, 0, 0, 1);
	glRotatef(90, 1, 0, 0);
	glutSolidCone(0.1, 0.5, 20, 20);
	glPopMatrix();*/

	if (!shadow) {
		glColor3f(0.964, 0.866, 0.117); // yellow
	}
	else {
		glColor3f(0.2, 0.2, 0.2);
	}

	glPushMatrix(); // right arm bearing
	glTranslatef(-0.2, 2.8, 0.85);
	glutSolidSphere(0.2, 20, 20);
	glPopMatrix();

	glPushMatrix(); //right arm
	glTranslatef(-0.8, 1.9, 1);
	glTranslatef(0.7, 1, 0);
	glRotatef(armsRotYR, 0, 1, 0);
	glRotatef(-30 - armsRot, 0, 0, 1);
	glTranslatef(0, -1, 0);
	glRotatef(-7, 1, 0, 0);
	glScalef(0.25, 2, 0.25);
	glutSolidCube(1);
	glPopMatrix();

	if (!shadow) {
		glColor3f(0.450, 0.439, 0.349);
	}
	else {
		glColor3f(0.2, 0.2, 0.2);
	}

	/*glPushMatrix(); //right spike
	glTranslatef(-1.25, 1.1, 1.1);
	glRotatef(-25, 0, 0, 1);
	glRotatef(90, 1, 0, 0);
	glutSolidCone(0.1, 0.5, 20, 20);
	glPopMatrix();*/

	glPushMatrix(); //axel
	glTranslatef(0, 1.2, -1.5);
	GLUquadric *q;
	q = gluNewQuadric();
	gluCylinder(q, 0.3, 0.3, 3, 20, 20);
	gluQuadricDrawStyle(q, GLU_FILL);
	glPopMatrix();

	glPopMatrix();

	glPushMatrix(); //wheel left
	glTranslatef(0, 1.2, -1.1);
	glRotatef(wheelRotate, 0, 0, 1);
	glutSolidTorus(0.25, 0.5, 20, 20);
	glPopMatrix();

	glPushMatrix(); //wheel right
	glTranslatef(0, 1.2, 1.1);
	glRotatef(wheelRotate, 0, 0, 1);
	glutSolidTorus(0.25, 0.5, 20, 20);
	glPopMatrix();

	glPopMatrix();

}


void drawSenetry() {

	// legs

	glColor3f(0.6, 0.5, 0.5);
	glPushMatrix();
	glRotatef(30, 1, 0, 0);
	glScalef(1, 8, 1);
	glutSolidCube(0.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.6, 0, 1.2);
	glRotatef(120, 0, 1, 0);
	glRotatef(30, 1, 0, 0);
	glScalef(1, 8, 1);
	glutSolidCube(0.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1, 0, 1.3);
	glRotatef(-120, 0, 1, 0);
	glRotatef(30, 1, 0, 0);
	glScalef(1, 8, 1);
	glutSolidCube(0.5);
	glPopMatrix();

	//top
	glPushMatrix();
	glScalef(0.7, 0.7, 0.7);
	glTranslatef(0, 1.8, 1.2);
	glutSolidDodecahedron();
	glPopMatrix();
}

void drawPropeller() {

	glColor3f(0.0, 1.0, 1.0);
	glutSolidSphere(0.5, 20, 20);

	for (int i = 0; i < 360; i += 64) {
		glPushMatrix();
		glRotatef(i + prop_angle, 0, 1, 0);
		glTranslatef(5, 0, 0);
		glScalef(5, 0.5, 0.1);
		glutSolidSphere(1, 20, 20);
		glPopMatrix();
	}

}

void drawMount() {

	glPushMatrix();
	glScalef(0.2, 1, 1);
	glutSolidCube(4);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 3, 0);
	glScalef(0.2, 0.5, 0.5);
	glutSolidCube(4);
	glPopMatrix();
}

void drawDrone() {

	glPushMatrix(); //main tube
	glTranslatef(0, -2, 0);
	glScalef(17, 2, 2);
	glutSolidCube(2);
	glPopMatrix();

	//joins
	glPushMatrix();
	glTranslatef(-16, 1, 0);
	glRotatef(90, 1, 0, 0);
	GLUquadric *q;
	q = gluNewQuadric();
	gluCylinder(q, 0.5, 0.5, 3, 20, 20);
	gluQuadricDrawStyle(q, GLU_FILL);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(16, 1, 0);
	glRotatef(90, 1, 0, 0);
	gluCylinder(q, 0.5, 0.5, 3, 20, 20);
	gluQuadricDrawStyle(q, GLU_FILL);
	glPopMatrix();

	//propelers
	glPushMatrix();
	glTranslatef(-16, 1, 0);
	drawPropeller();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(16, 1, 0);
	drawPropeller();
	glPopMatrix();

	//mount
	glPushMatrix();
	glTranslatef(2, 0, 0);
	drawMount();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2, 0, 0);
	drawMount();
	glPopMatrix();

	//gun
	glPushMatrix();
	glTranslatef(0, 5, -3);
	glRotatef(30, 1, 0, 0);
	gluCylinder(q, 1.5, 1.4, 7, 20, 20);
	gluQuadricDrawStyle(q, GLU_FILL);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 1.5, 3);
	glutSolidSphere(1.4, 20, 20);
	glPopMatrix();
}

void normal(float x1, float y1, float z1,
	float x2, float y2, float z2,
	float x3, float y3, float z3)
{
	float nx, ny, nz;
	nx = y1*(z2 - z3) + y2*(z3 - z1) + y3*(z1 - z2);
	ny = z1*(x2 - x3) + z2*(x3 - x1) + z3*(x1 - x2);
	nz = x1*(y2 - y3) + x2*(y3 - y1) + x3*(y1 - y2);

	glNormal3f(nx, ny, nz);
}

void drawJunk() {

	const int N = 14;
	float vx[N] = { 0.0, 2.0, 3.0, 3.9, 4.2, 5.0, 4.2, 3.1, 2.0, 1.0, 1.05, 2.0, 1.9, 1.0 };
	float vy[N] = { 0.0, 0.0, 1.0, 2.1, 3.0, 5.0, 7.0, 8.25, 9.25, 10.0, 10.3, 11.0, 11.25, 11.3 };
	float vz[N] = { 0 };

	float wx[N], wy[N], wz[N];
	float angStep = -10.0*3.1415926 / 180.0;

	for (int j = 0; j < 36; j++) {

		for (int i = 0; i < N; i++) {
			wx[i] = vx[i] * cos(angStep) + vz[i] * sin(angStep);
			wy[i] = vy[i];
			wz[i] = -1 * vx[i] * sin(angStep) + vz[i] * cos(angStep);
		}
		glBegin(GL_TRIANGLE_STRIP);
		for (int i = 0; i < N; i++) {
			if (i > 0) normal(wx[i - 1], wy[i - 1], wz[i - 1],
				vx[i - 1], vy[i - 1], vz[i - 1],
				vx[i], vy[i], vz[i]);
			glTexCoord2f(j / 36.0, i / (float)N);
			glVertex3f(vx[i], vy[i], vz[i]);
			if (i > 0) normal(wx[i - 1], wy[i - 1], wz[i - 1],
				vx[i], vy[i], vz[i],
				wx[i], wy[i], wz[i]);
			glTexCoord2f((j + 1) / 36.0, i / (float)N);
			glVertex3f(wx[i], wy[i], wz[i]);
		}
		glEnd();

		for (int i = 0; i < N; i++) {
			vx[i] = wx[i];
			vy[i] = wy[i];
			vz[i] = wz[i];
		}
	}
}

void drawCog() {

	glDisable(GL_TEXTURE_2D);
	const int N = 36;
	float vx[N] = { 1.0, 1.0, 3.0, 4.0, 5.6, 4.5, 5.0, 6.1, 7.0, 5.4, 5.4, 7.0, 6.1, 4.9, 3.5, 4.6, 2.6, 1.7, -0.2, -0.2, -2.8, -2.2, -3.8, -5.0,
		-6.2, -5.0, -5.5, -7.0, -6.6, -5.0, -4.2, -5.2, -3.5, -2.4, -1.0, -1.0 };
	float vy[N] = { 7.0, 5.4, 4.6, 5.8, 4.1, 3.0, 2.2, 2.6, 0.6, 0.2, -0.8, -0.8, -3.2, -2.6, -4.2, -5.2, -6.6, -5.2, -5.4, -7.0, -6.4,
		-5.0, -4.0, -4.8, -3.2, -2.2, -0.2, 0.0, 2.4, 2.0, 3.6, 4.8, 6.0, 5.0, 5.2, 7.0 };
	float vz[N] = { 0 };


	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i < N; i++) {
		if (i > 0) normal(vx[i - 1], vy[i - 1], vz[i - 1],
			vx[i], vy[i], vz[i],
			vx[i], vy[i], vz[i] + 2);
		glVertex3f(vx[i], vy[i], vz[i]);
		glVertex3f(vx[i], vy[i], vz[i] + 2);
	}
	glVertex3f(vx[0], vy[0], vz[0]);
	glVertex3f(vx[0], vy[0], vz[0] + 1);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	glNormal3f(0, 0, -1);
	for (int i = 0; i <= 34;) {
		glVertex3f(vx[i], vy[i], vz[i]);
		glVertex3f(0, 0, 0);
		i++;
		glVertex3f(vx[i], vy[i], vz[i]);
	}
	glVertex3f(vx[0], vy[0], vz[0]);
	glVertex3f(0, 0, 0);
	glVertex3f(vx[34], vy[34], vz[34]);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	glNormal3f(0, 0, 1);
	for (int i = 0; i <= 34;) {
		glVertex3f(vx[i], vy[i], vz[i] + 2);
		glVertex3f(0, 0, 0);
		i++;
		glVertex3f(vx[i], vy[i], vz[i] + 2);
	}

	glVertex3f(vx[0], vy[0], vz[0] + 2);
	glVertex3f(0, 0, 0);
	glVertex3f(vx[34], vy[34], vz[34] + 2);
	glEnd();
}

//--Display: ---------------------------------------------------------------
//--This is the main display module containing function calls for generating
//--the scene.
void display()
{
	float lx = 0.0;
	float ly = 20.0;
	float lz = 0.0;
	float lpos[4] = { lx, ly, lz, 1.0 };
	float white[4] = { 1.0, 1.0, 1.0, 1.0 };

	// Light source position = (lx, ly, lz)
	float shadowMat[16] = { ly,0,0,0, -lx,0,-lz,-1,
		0,0,ly,0, 0,0,0,ly };

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if (!firstPerson){
		gluLookAt(cam_x, cam_y, cam_z, cam_x + look_x, look_y, cam_z + look_z, 0, 1, 0);
	} else {
		gluLookAt(robotX, 3, robotZ, robotX + -sin(botR * M_PI / 180 + 1.5), -sin(upperBotR * M_PI / 180) + 3, robotZ + -cos(botR * M_PI / 180 + 1.5), 0, 1, 0);
	}
	glLightfv(GL_LIGHT0, GL_POSITION, lpos);

	//glRotatef(angle, 0.0, 1.0, 0.0);
	drawSkyBox();
	int j = 0;
	for (int i = -20; i < 40; i += 20) {
		glPushMatrix();
		glTranslatef(0, i, 0);
		glRotatef(90 * j, 0, 1, 0);
		glPushMatrix();
		glTranslatef(3, 0, 11);
		glRotatef(-trap_angle, 1, 0, 0);
		glTranslatef(-3, 0, -11);
		drawTrapDoor(4, 7, 10, 10);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(10, 0, 4);
		glRotatef(trap_angle, 1, 0, 0);
		glTranslatef(-10, 0, -4);
		drawTrapDoor(4, 4, 10, 6);
		glPopMatrix();
		drawLevel();
		glPopMatrix();
		j++;
	}
	glDisable(GL_TEXTURE_2D);

	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 30);

	glPushMatrix();
	glTranslatef(0, 2, 10);
	drawSenetry();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(13, 2, 1);
	drawSenetry();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-10, 2, -10);
	drawSenetry();
	glPopMatrix();

	float spot_pos[] = { 0.0f, 1.5f, 0.0f, 1.0f };
	float spot_dir[] = { -1.0f, -1.0f, 0.0f };

	float spot_pos2[] = { 13.0f, 5.0f, 1.0f, 1.0f };

	glPushMatrix();
	glTranslatef(13, 5, 1);
	glRotatef(spot_rotate, 0, 1, 0);
	glTranslatef(-13, -5, -1);
	glLightfv(GL_LIGHT2, GL_POSITION, spot_pos2);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot_dir);
	glPopMatrix();

	float spot_pos3[] = { -10.0f, 5.0f, -10.0f, 1.0f };

	glPushMatrix();
	glTranslatef(-10, 5, -10);
	glRotatef(spot_rotate, 0, 1, 0);
	glTranslatef(10, 5, 10);
	glLightfv(GL_LIGHT3, GL_POSITION, spot_pos3);
	glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, spot_dir);
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, txId[0]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 10);

	glColor3f(0.5, 0.0, 1.0);
	glPushMatrix();
	glScalef(0.5, 0.5, 0.5);
	glTranslatef(50, 10 + droneY, 20);
	glRotatef(34, 0, 1, 0);
	drawDrone();
	glPopMatrix();


	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 20);
	if (step == 3) {

		glPushMatrix();

		glTranslatef(robotX, 0, robotZ);
		glRotatef(botR, 0, 1, 0);

		glLightfv(GL_LIGHT1, GL_POSITION, spot_pos);
		glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_dir);
		drawRobot1(0);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, txId[2]);
		glColor3f(1.0, 1.0, 1.0);

		glPushMatrix();
		glRotatef(upperBotR, 0, 0, 1);
		glTranslatef(-1.8, vaseDropDistance, 0.0);
		glScalef(0.2, 0.2, 0.2);
		drawJunk();
		glPopMatrix();

		glPopMatrix();


		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		glColor3f(0.2, 0.2, 0.2);
		glPushMatrix(); //Draw Shadow robot Object
		glMultMatrixf(shadowMat);
		glTranslatef(robotX, 0, robotZ);
		glRotatef(botR, 0, 1, 0);
		drawRobot1(1);
		glPopMatrix();

		glEnable(GL_LIGHTING);

		if (vaseDropDistance > -0.1 && vaseDropDistance < 7 && robotX > -13.5) {
			glDisable(GL_LIGHTING);
			glDisable(GL_TEXTURE_2D);
			glColor3f(0.2, 0.2, 0.2);
			glPushMatrix(); //Draw Shadow junk
			glMultMatrixf(shadowMat);
			glRotatef(upperBotR, 0, 0, 1);
			glTranslatef(robotX, 0, robotZ);
			glRotatef(botR, 0, 1, 0);
			
			glTranslatef(-1.8, vaseDropDistance, 0.0);
			glScalef(0.2, 0.2, 0.2);
			drawJunk();
			glPopMatrix();
		}
		glEnable(GL_LIGHTING);

		glMaterialfv(GL_FRONT, GL_SPECULAR, white);
		glMaterialf(GL_FRONT, GL_SHININESS, 5);
		glColor3f(1.0, 0.0, 1.0);
		glPushMatrix();
		glTranslatef(-6, cogDropDistance, -7);
		glScalef(0.2, 0.2, 0.2);
		drawCog();
		glPopMatrix();

		if (cogDropDistance > -0.1 && cogDropDistance < 7) {
			glColor3f(0.2, 0.2, 0.2);
			glDisable(GL_LIGHTING);
			glDisable(GL_TEXTURE_2D);
			glPushMatrix();
			glMultMatrixf(shadowMat);
			glTranslatef(-6, cogDropDistance, -7);
			glScalef(0.2, 0.2, 0.2);
			drawCog();
			glPopMatrix();
		}
		glEnable(GL_LIGHTING);

	}
	else if (step == 5 || step == 6) {
		glPushMatrix();

		glTranslatef(robotX, 0, robotZ);
		glRotatef(botR, 0, 1, 0);

		glLightfv(GL_LIGHT1, GL_POSITION, spot_pos);
		glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_dir);
		glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2);
		drawRobot1(0);

		glMaterialfv(GL_FRONT, GL_SPECULAR, white);
		glMaterialf(GL_FRONT, GL_SHININESS, 5);
		glColor3f(1.0, 0.0, 1.0);
		glEnable(GL_TEXTURE_2D);

		glPushMatrix();
		glRotatef(upperBotR, 0, 0, 1);
		glTranslatef(-2, cogDropDistance, 0);
		glRotatef(90, 0, 1, 0);
		glScalef(0.2, 0.2, 0.2);
		drawCog();
		glPopMatrix();

		glPopMatrix();

		if (cogDropDistance > -0.1 && cogDropDistance < 7) {
			glColor3f(0.2, 0.2, 0.2);
			glDisable(GL_LIGHTING);
			glDisable(GL_TEXTURE_2D);
			glPushMatrix();
			glMultMatrixf(shadowMat);
			glTranslatef(robotX, 0, robotZ);
			glRotatef(botR, 0, 1, 0);
			glRotatef(upperBotR, 0, 0, 1);
			glTranslatef(-2, cogDropDistance, 0);
			glRotatef(90, 0, 1, 0);
			glScalef(0.2, 0.2, 0.2);
			drawCog();
			glPopMatrix();
		}
		glEnable(GL_LIGHTING);

		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		glColor3f(0.2, 0.2, 0.2);
		glPushMatrix(); //Draw Shadow robot Object
		glMultMatrixf(shadowMat);
		glTranslatef(robotX, 0, robotZ);
		glRotatef(botR, 0, 1, 0);
		drawRobot1(1);
		glPopMatrix();

		glEnable(GL_LIGHTING);

	}
	else {
		glPushMatrix();
		glTranslatef(robotX, 0, robotZ);
		glRotatef(botR, 0, 1, 0);
		glLightfv(GL_LIGHT1, GL_POSITION, spot_pos);
		glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_dir);
		glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2);
		drawRobot1(0);
		glPopMatrix();

		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		glColor3f(0.2, 0.2, 0.2);
		glPushMatrix(); //Draw Shadow robot Object
		glMultMatrixf(shadowMat);
		glTranslatef(robotX, 0, robotZ);
		glRotatef(botR, 0, 1, 0);
		drawRobot1(1);
		glPopMatrix();

		glEnable(GL_LIGHTING);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, txId[2]);
		glColor3f(1.0, 1.0, 1.0);
		glPushMatrix();
		glTranslatef(vaseX, vaseDropDistance, vaseZ);
		glScalef(0.2, 0.2, 0.2);
		drawJunk();
		glPopMatrix();

		if (vaseDropDistance > -0.1 && vaseDropDistance < 7 && vaseX > -13.5) {
			glDisable(GL_LIGHTING);
			glDisable(GL_TEXTURE_2D);
			glColor3f(0.2, 0.2, 0.2);
			glPushMatrix(); //Draw Shadow Object
			glMultMatrixf(shadowMat);
			glTranslatef(vaseX, vaseDropDistance, vaseZ);
			glScalef(0.2, 0.2, 0.2);
			drawJunk();
			glPopMatrix();
		}
		glEnable(GL_LIGHTING);


		glMaterialfv(GL_FRONT, GL_SPECULAR, white);
		glMaterialf(GL_FRONT, GL_SHININESS, 5);
		glColor3f(1.0, 0.0, 1.0);
		glPushMatrix();
		glTranslatef(cogX, cogDropDistance, cogZ);
		glScalef(0.2, 0.2, 0.2);
		glRotatef(cogR, 0, 1, 0);
		drawCog();
		glPopMatrix();

		if (cogDropDistance > -0.1 && cogDropDistance < 7) {
			glColor3f(0.2, 0.2, 0.2);
			glDisable(GL_LIGHTING);
			glDisable(GL_TEXTURE_2D);
			glPushMatrix();
			glMultMatrixf(shadowMat);
			glTranslatef(cogX, cogDropDistance, cogZ);
			glRotatef(cogR, 0, 1, 0);
			glScalef(0.2, 0.2, 0.2);
			drawCog();
			glPopMatrix();
		}
		glEnable(GL_LIGHTING);
	}
	glColor3f(0.341, 0.392, 0.937);
	glPushMatrix();
		glTranslatef(-13.1, 10, 13);
		glRotatef(bigCogRotate, 1, 0 ,0);
		glRotatef(90, 0, 1 ,0);
		drawCog();
	glPopMatrix();


	glutSwapBuffers();
}

//------- Initialize OpenGL parameters -----------------------------------
void initialize()
{
	float grey[4] = { 0.2, 0.2, 0.2, 1.0 };
	float white[4] = { 1.0, 1.0, 1.0, 1.0 };

	loadTexture();
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_SMOOTH);

	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);	//Background colour

	glEnable(GL_LIGHTING);					//Enable OpenGL states
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, grey);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, grey);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	q = gluNewQuadric();
	gluQuadricDrawStyle(q, GLU_FILL);
	gluQuadricNormals(q, GLU_SMOOTH);

	glEnable(GL_LIGHT1);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, white);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT1, GL_SPECULAR, white);

	glEnable(GL_LIGHT2);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 10);
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 1);
	glLightfv(GL_LIGHT2, GL_AMBIENT, grey);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT2, GL_SPECULAR, white);

	glEnable(GL_LIGHT3);
	glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 10);
	glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 1);
	glLightfv(GL_LIGHT3, GL_AMBIENT, grey);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT3, GL_SPECULAR, white);

	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_TEXTURE_2D);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1.5, 1.0, -1.5, 1.5, 1.5, 1000);   //Camera Frustum
}

//------------ Special key event callback ---------------------------------
// To enable the use of left and right arrow keys to rotate the scene
void special(int key, int x, int y)
{
	if (!firstPerson){
		if (key == GLUT_KEY_LEFT) {
			angle -= 0.09; //5 degrees in radians
			look_x = sin(angle);
			look_z = -cos(angle);
		}
		else if (key == GLUT_KEY_RIGHT) {
			angle += 0.09; //5 degrees in radians
			look_x = sin(angle);
			look_z = -cos(angle);
		}
		else if (key == GLUT_KEY_UP) {
			cam_x += look_x;
			cam_z += look_z;
		}
		else if (key == GLUT_KEY_DOWN) {
			cam_x -= look_x;
			cam_z -= look_z;
		}
		else if (key == GLUT_KEY_PAGE_DOWN) {
			cam_y--;
			look_y--;
		}
		else if (key == GLUT_KEY_PAGE_UP) {
			cam_y++;
			look_y++;
		}
	}

	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y){
	if (key == 'p'){
		firstPerson = !firstPerson;
	}
}

void moveDrone() {
	int static count = 0;
	droneY = 5 * sin(count * M_PI / 180);

	if (count >= 360){
		count = 0;
	}
	count ++;
}

void openTrap() {

		//opening
	if (trap_angle < 120) {
		trapTime += 0.1;
		float trapDistance = (9.81 * trapTime * trapTime) / 2; //distance to move by acceleration of gravity
		trap_angle = (trapDistance / (2 * 3.142 * 6)) * 360;
	}

}

void closeTrap() {
		//closing
	if (trap_angle > 0 && trapTime >= 0) {
		trapTime -= 0.1;
		float trapDistance = (9.81 * trapTime * trapTime) / 2; //distance to move by acceleration of gravity
		trap_angle = (trapDistance / (2 * 3.142 * 6)) * 360;
	}

}

void myTimer(int value) {

	prop_angle += 30;
	int static count = 0;
	moveDrone();
	spot_rotate += 5;
	
	bigCogRotate += 1;

	//~ if ((count > 0 && count <  18) || step == 0 || step == 7) {
		//~ openCloseTrap();
	//~ }
	//~ if (count > 100) {
		//~ count = 0;
	//~ }
	//~ count++;


	if (step == 0) {
		openTrap();

		if (vaseDropDistance > 0.0) {

			vaseDropDistance -= (9.81 * 1.2 * dropTime * dropTime) / 2; //distance to move by acceleration of gravity

		}
		if (cogDropDistance > 2) {
			dropTime += 0.01;
			cogDropDistance -= (9.81 * 2 * dropTime * dropTime) / 2; //distance to move by acceleration of gravity
		}

		if (vaseDropDistance <= 0.7) {
			step = 1;
			count += 1;
		}
	}
	
	if (step == 1) {
		closeTrap();
		
		if (botR > -45) {
			botR -= 1;
			wheelRotate++;
		}

		if (robotX > -3.5 && botR <= -45) {
			robotX -= 0.1;
			robotZ -= 0.1;
			wheelRotate++;
		}

		if (robotX <= -3.5 && upperBotR < 15) {
			upperBotR += 1;
		}

		if (upperBotR >= 15 && armsRot < 30) {
			armsRot += 1;
		}

		if (armsRot >= 30) {
			step = 2;
		}
	}
	if (step == 2) {
		openTrap();
		if (armsRot >= 30 && upperBotR > 0) {

			upperBotR -= 1;
			vaseDropDistance += 0.025;
		}

		if (upperBotR <= 0 && robotX < -2.0) {
			vaseX += 0.1;
			vaseZ += 0.1;
			robotX += 0.1;
			robotZ += 0.1;
			wheelRotate--;
		}
		if (robotX >= -2.0) {
			step = 3;
		}

	}
	if (step == 3) {
		closeTrap();
		if (robotX >= -2.0 && botR < 0) {
			botR++;
			wheelRotate--;
		}

		if (botR >= 0 && robotX > -13) {
			robotX -= 0.1;
			wheelRotate++;
		}

		if (robotX <= -13 && upperBotR > -20) {
			upperBotR -= 1;
		}

		if (upperBotR == -20) {
			step = 4;
			vaseX = robotX - 1.6;
			vaseZ = robotZ;
			vaseDropDistance += 0.1;
			dropTime = 0;
		}
	}
	if (step == 4) {
		openTrap();
		if (upperBotR < 5) {
			upperBotR += 1;
		}
		
		if (vaseDropDistance > -100) {
			vaseDropDistance = 5 * sin(1) * dropTime - 5.88 * dropTime * dropTime + 1; //speed 5ms angle about 60 + offset
			vaseX = -14 - 5 * cos(1) * dropTime;
			dropTime += 0.05;
		}

		if (upperBotR >= 5 && robotX < -6) {
			robotX += 0.1;
			wheelRotate--;
		}
		if (robotX >= -6.0 && botR > -90) {
			botR -= 1;
		}

		if (botR <= -90 && armsRotYR < 15) {
			armsRotYL -= 2;
			armsRotYR += 2;
		}
		if (armsRotYR >= 14 && robotZ > -5) {
			robotZ -= 0.1;
		}

		if (robotZ <= -5) {
			step = 5;
			cogX += 2;
		}
	}
	if (step == 5) {
		closeTrap();
		if (upperBotR > -10) {
			upperBotR--;
		}

		if (upperBotR <= -10 && robotZ > -8) {
			robotZ -= 0.2;
			cogZ -= 0.2;
		}

		if (robotZ <= -8 && botR > -180) {
			botR--;
		}

		if (botR <= -180 && robotX < 2.5) {
			robotX += 0.2;
			cogX += 0.2;
		}

		if (robotX >= 2.5) {
			step = 6;
		}
	}
	if (step == 6) {
		if (upperBotR < 0) {
			upperBotR++;
		}

		if (upperBotR >= 0) {
			cogX = robotX + 2;
			cogZ = robotZ;
			cogR = 90;
			step = 7;
			dropTime = 0;
		}
	}

	if (step == 7) {
		if (armsRotYR > 0) {
			armsRotYL += 2;
			armsRotYR -= 2;
		}

		static int wait = 0;

		if (wait > 50 && armsRotYR <= 5 && cogDropDistance > -18) {
			
			dropTime += 0.01;
			cogDropDistance -= (9.81 * dropTime * dropTime) / 2;
			openTrap();
		}
		wait++;
		if (armsRotYR <= 5 && botR > -270) {
			botR -= 1;
		}
		if (botR <= -270 && robotZ < 0) {
			robotZ += 0.1;
		}
		if (robotZ >= 0 && botR > -360) {
			botR -= 1;
			closeTrap();
		}
		if (botR <= -360 && robotX > 0) {
			robotX -= 0.1;
		}

		if (robotX <= 0) {
			robotX = 0.0;
			robotZ = 0.0;
			wheelRotate = 0.0;
			botR = 0;
			upperBotR = 0;
			armsRot = 0;
			vaseX = -5.0;
			vaseZ = -5.0;
			vaseRot = 0;
			cogX = -6;
			cogZ = -7;
			cogR = 0;
			armsRotYL = 0;
			armsRotYR = 0;
			step = 0;
			vaseDropDistance = 20.0;
			cogDropDistance = 20.0;
			dropTime = 0;
		}
	}

	glutPostRedisplay();
	glutTimerFunc(30, myTimer, 0);


}

//  ------- Main: Initialize glut window and register call backs -----------
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(10, 10);
	glutCreateWindow("Robots");
	initialize();

	glutDisplayFunc(display);
	glutSpecialFunc(special);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(30, myTimer, 0);
	glutMainLoop();
	return 0;
}

