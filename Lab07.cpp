#include <gl/glut.h>
#include <stdio.h>
#include <Windows.h>

double earth_revolution = 0;
double earth_rotation = 0;
double mars_revolution = 0;
double mars_rotation = 0;
double moon_revolution = 0;
double moon_rotation = 0;

void init(void) {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_LINE_SMOOTH, GL_FASTEST);
	glEnable(GL_LINE_SMOOTH);

	printf("init function called\n");
}

void resize(int width, int height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)width / (float)height, 1, 500);
	glMatrixMode(GL_MODELVIEW);
	printf("resize is called\n");
}


void earth(void) {
	earth_revolution = earth_revolution + 0.1;
	if (earth_revolution > 360) earth_revolution -= 360;
	earth_rotation = earth_rotation + 1;
	if (earth_rotation > 360)earth_rotation -= 360;
}

void mars(void) {
	mars_revolution = mars_revolution + 0.05;
	if (mars_revolution > 360) mars_revolution -= 360;
	mars_rotation = mars_rotation + 2;
	if (mars_rotation > 360)mars_rotation -= 360;
}

void moon(void) {
	moon_revolution = moon_revolution + 0.05;
	if (moon_revolution > 360) moon_revolution -= 360;
	moon_rotation = moon_rotation + 2;
	if (moon_rotation > 360)moon_rotation -= 360;
}


void idle(void){
	earth();
	mars();
	moon();
	glutPostRedisplay();
}

void draw(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(20, 20, 20, 0, 0, 0, 0, 1, 0);
	// 태양.
	glColor3f(1, 0, 0);
	glutWireSphere(3,60,60); 
	// 지구
	glPushMatrix();
	glRotatef(earth_revolution, 0, 1, 0); 
	glTranslatef(5, 0, 0 ); 
	glRotatef(earth_rotation, 0, 1, 0); 
	glColor3f(0, 0, 1);
	glutWireSphere(1,40,40);
	// 달
	glRotatef(moon_revolution, 0, 1, 0); 
	glTranslatef(2, 0, 0);
	glRotatef(moon_rotation, 0, 1, 0);
	glColor3f(1, 1, 1);
	glutWireSphere(0.25,20,20);
	glPopMatrix();
	// 화성
	glPushMatrix();
	glRotatef(mars_revolution, 0, 1, 0);
	glTranslatef(10, 0, 0);
	glRotatef(mars_rotation, 0, 1, 0);
	glColor3f(0.5, 0, 0);
	glutWireSphere(1, 40, 40);
	glPopMatrix();

	glFlush();
	glutSwapBuffers();
	
}

int main(int argc, char** argv) {
	/* Window 초기화 */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(300, 300);
	glutCreateWindow("My First GL Program");
	init(); // -> 사용자 초기화 함수
	glutIdleFunc(idle);
	glutDisplayFunc(draw);
	glutReshapeFunc(resize);

	/* Looping 시작 */
	glutMainLoop();

	return 0;
}
