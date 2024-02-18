#include <gl/glut.h>
#include <stdio.h>
#include <windows.h>
#include <GL/freeglut.h>
#include <string.h>
#include <math.h>

const int current_width = 500, current_height = 500;
double spin = 0;
GLUquadricObj* m_pQuadric;

void init(void)
{
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	/* 화면 좌표 정보 설정 */
	glMatrixMode(GL_PROJECTION);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat lmodel_ambient[] = { 0.5, 0.5, 0.5, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	GLfloat ambient_Sun[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat diffuse_Sun[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat specular_Sun[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_Sun);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_Sun);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular_Sun);
	glMaterialf(GL_FRONT, GL_SHININESS, 64);

	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	m_pQuadric = gluNewQuadric();

}


void idle(void)
{
	glutPostRedisplay();
}

void draw_txt(void* font, const char* str, int x, int y, int z)
{
	glViewport(0, current_height / 2, current_width / 2, current_height / 2);
	glRasterPos3i(x, y, z);
	for (int i = 0; i < strlen(str); i++)
		glutBitmapCharacter(font, str[i]);
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
}

void draw_can(void)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, current_width / 2, current_height / 2);
	gluPerspective(45, 1, 0.1, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(10, 10, 10, 0, 0, 0, 0, 1, 0);
	draw_axis();
	glColor3f(0.5, 0, 0);
	gluCylinder(m_pQuadric, 1, 1, 5, 30, 30);
	glFlush();
}


void draw_disk(void)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(current_width / 2, current_height / 2, current_width / 2, current_height / 2);
	gluPerspective(45, 1, 0.1, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(10, 10, 10, 0, 0, 0, 0, 1, 0);
	draw_axis();
	glColor3f(0, 0.5, 0);
	gluDisk(m_pQuadric, 1, 3, 30, 30);
	glFlush();

}


void draw_teapot(void)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(current_width / 2, 0, current_width / 2, current_height / 2); 
	gluPerspective(45, 1, 0.1, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(10, 10, 10, 0, 0, 0, 0, 1, 0);
	draw_axis();
	glColor3f(0, 0, 0.5);
	glutWireTeapot(3);
	glFlush();
}

void draw(void) {

	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity(); 
	draw_txt(GLUT_BITMAP_TIMES_ROMAN_24,"12181732 KSM", 0, 0, 0);
	draw_disk(); 
	draw_can(); 
	draw_teapot();
	glutSwapBuffers();
}



int main(int argc, char** argv) {
	/* Window 초기화 */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize( current_width, current_height);
	glutInitWindowPosition(300, 300);
	glutCreateWindow("My First GL Program");
	init(); // -> 사용자 초기화 함수
	/* Popup menu 생성 및 추가 */
	glutAddMenuEntry("Draw mode OFF", 0);
	glutAddMenuEntry("Point", 1);
	glutAddMenuEntry("Line", 2);
	glutAddMenuEntry("Triangle", 3);
	/* 3개의 항목이 있는 메뉴 생성 */
	/* Callback 함수 정의 */
	glutIdleFunc(idle);
	glutDisplayFunc(draw);
	//glutMouseFunc(mouse);
	//glutMotionFunc(motion);

	/* Looping 시작 */
	glutMainLoop();

	return 0;
}