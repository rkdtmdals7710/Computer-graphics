#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/glut.h>
#include <GL/freeglut.h>
#include <Glaux.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "bmpfuncs.h"

int theta = 0, phi = 0;
int distance = 10;
double cam[3] = { 10,10,10 };
double center[3] = { 0,0,0 };
double up[3] = { 0,1,0 };
const double pi = 3.14;
GLuint texName[6];

void init(void);
void draw(void);
void draw_axis(void);
void special_keyboard(int key, int x_trans, int y_trans);
void main_menu_function(int option);
void idle(void);
void mouse_wheel(int wheel, int dir, int x, int y);
void resize(int width, int height);
void change_angle(int angle, int x);
void change_distance(int x);

int main(int argc, char** argv) {
	/* Window �ʱ�ȭ */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(300, 300);
	glutCreateWindow("My First GL Program");
	init(); // -> ����� �ʱ�ȭ �Լ�
	/* Popup menu ���� �� �߰� */
	glutAddMenuEntry("Draw mode OFF", 0);
	glutAddMenuEntry("Point", 1);
	glutAddMenuEntry("Line", 2);
	glutAddMenuEntry("Triangle", 3);
	/* 3���� �׸��� �ִ� �޴� ���� */
	glutCreateMenu(main_menu_function);
	glutAddMenuEntry("Quit", 999);
	glutAddMenuEntry("Clear", 11);
	//glutAddSubMenu("Draw mode", submenu1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	/* Callback �Լ� ���� */
	glutIdleFunc(idle);
	glutDisplayFunc(draw);
	//glutMouseFunc(mouse);
	//glutMotionFunc(motion);
	glutSpecialFunc(special_keyboard);
	glutMouseWheelFunc(mouse_wheel);
	glutReshapeFunc(resize);

	/* Looping ���� */
	glutMainLoop();

	return 0;
}

void init(void)
{
	/* ȭ���� �⺻������ dark blue ���� */
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);

	glGenTextures(6, texName);
	AUX_RGBImageRec* pRGBImage;

	glBindTexture(GL_TEXTURE_2D, texName[0]);
	pRGBImage = auxDIBImageLoadA("TexImage0.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[1]);
	pRGBImage = auxDIBImageLoadA("TexImage1.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[2]);
	pRGBImage = auxDIBImageLoadA("TexImage2.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[3]);
	pRGBImage = auxDIBImageLoadA("TexImage3.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[4]);
	pRGBImage = auxDIBImageLoadA("TexImage4.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[5]);
	pRGBImage = auxDIBImageLoadA("TexImage5.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glEnable(GL_TEXTURE_2D);

	/* ȭ�� ��ǥ ���� ���� */
	glMatrixMode(GL_PROJECTION);
}

void draw(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(5,5,5,0,0,0,0,1,0);
	
	draw_axis();

	glRotatef(phi, 0, 1, 0);
	glBindTexture(GL_TEXTURE_2D, texName[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);  glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(1, 0);  glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1, 1);  glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(0, 1);  glVertex3f(-1.0f, 1.0f, 1.0f);

	glTexCoord2f(0, 0);  glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(1, 0);  glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1, 1);  glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(0, 1);  glVertex3f(1.0f, 1.0f, -1.0f);

	glTexCoord2f(0, 0);  glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1, 0);  glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(1, 1);  glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(0, 1);  glVertex3f(-1.0f, -1.0f, 1.0f);

	glTexCoord2f(0, 0);  glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(1, 0);  glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(1, 1);  glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0, 1);  glVertex3f(-1.0f, 1.0f, -1.0f);

	glTexCoord2f(0, 0);  glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(1, 0);  glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(1, 1);  glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(0, 1);  glVertex3f(1.0f, -1.0f, 1.0f);

	glTexCoord2f(0, 0);  glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(1, 0);  glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(1, 1);  glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(0, 1);  glVertex3f(1.0f, -1.0f, -1.0f);
	glEnd();

	glFlush();
	glutSwapBuffers();
}


void draw_axis(void)
{
	glLineWidth(3); glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(4, 0, 0);
	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 4, 0);
	glColor3f(0, 0, 1); glVertex3f(0, 0, 0); glVertex3f(0, 0, 4);
	glEnd();
	glLineWidth(1);
	// ��ǥ���� �β� // X���� red
	// Y���� green // Z���� blue //�β��ٽ�ȯ��
}


void special_keyboard(int key, int x, int y) {

	if (key == GLUT_KEY_LEFT) {
		change_angle(1, 5);
		printf("phi : %d, theta %: %d\n", phi, theta);
	}

	if (key == GLUT_KEY_RIGHT) {
		change_angle(1, -5);
		printf("phi : %d, theta %: %d\n", phi, theta);
	}

	if (key == GLUT_KEY_UP) {
		change_angle(2, 5);
		printf("phi : %d, theta %: %d\n", phi, theta);
	}

	if (key == GLUT_KEY_DOWN) {
		change_angle(2, -5);
		printf("phi : %d, theta %: %d\n", phi, theta);
	}

	glutPostRedisplay();
}

void mouse_wheel(int wheel, int dir, int x, int y) {

	if (dir > 0) {
		change_distance(-5);
		printf("distance : %d, zoom in\n", distance);
	}

	else {
		change_distance(5);
		printf("distance : %d, zoom out\n", distance);
	}
}

void sub_menu_function(int option) {
	printf("Submenu %d has been selected\n", option);
}

void main_menu_function(int option) {
	printf("Main menu %d has been selected\n", option);
	if (option == 999) {
		exit(0);
	}
}

void idle(void)
{
	glutPostRedisplay();
}

void resize(int width, int height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)width / (float)height, 1, 500);
	glMatrixMode(GL_MODELVIEW);
	printf("resize is called\n");
}

void change_angle(int angle, int x) {
	if (angle == 1) {
		phi += x;
		if (phi < 0)
			phi = 360 + phi;
		else if (phi > 360)
			phi -= 360;
	}

	if (angle == 2) {
		theta += x;
		if (theta <= 0) {
			theta = 360 + theta;
			up[1] = -up[1];
		}
		else if (theta == 180)
			up[1] = -up[1];
		else if (theta > 360) {
			theta = theta - 360;
			up[1] = -up[1];
		}
	}

	cam[0] = distance * sin(theta * (pi / 180)) * sin(phi * (pi / 180));
	cam[1] = distance * cos(theta * (pi / 180));
	cam[2] = distance * sin(theta * (pi / 180)) * cos(phi * (pi / 180));
}


void change_distance(int x) {
	distance += x;

	cam[0] = distance * sin(theta * (pi / 180)) * sin(phi * (pi / 180));
	cam[1] = distance * cos(theta * (pi / 180));
	cam[2] = distance * sin(theta * (pi / 180)) * cos(phi * (pi / 180));
}