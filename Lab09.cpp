#include <gl/glut.h>
#include <stdio.h>
#include <Windows.h>

double earth_revolution = 0;
double earth_rotation = 0;
double mars_revolution = 0;
double mars_rotation = 0;
double moon_revolution = 0;
double moon_rotation = 0;
int light = 0;
int a = 0;

void init(void) {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat lmodel_ambient[] = { 0.5, 0.5, 0.5, 1.0 };
	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	//glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	//glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	//glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 2.0); 
	//glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.2); 
	//glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.2);
	glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
	GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, 64);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE); //
	glEnable(GL_COLOR_MATERIAL);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_LINE_SMOOTH, GL_FASTEST);
	glEnable(GL_LINE_SMOOTH);

	printf("init function called\n");
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
	// 좌표축의 두께 // X축은 red
	// Y축은 green // Z축은 blue //두께다시환원
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
	moon_revolution = moon_revolution + 0.03;
	if (moon_revolution > 360) moon_revolution -= 360;
	moon_rotation = moon_rotation + 2;
	if (moon_rotation > 360)moon_rotation -= 360;
}


void keyboard(unsigned char key, int x, int y) {
	if ('0' <= key && key <= '4') {
		light = key - 48;
		printf("light's position is %d\n", light);
	}

	if (key == 'q')
		a = 1;

	if (key == 'r')
		a = 0;
}


void idle(void) {
	if (a == 1)
		return;
	earth();
	mars();
	moon();
	glutPostRedisplay();
}

void draw(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 };

	/*glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 40.0);
	GLfloat spot_direction[] = { 1.0, 0.0, 0.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 2.0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);*/

	
	gluLookAt(10, 10, 10, 0, 0, 0, 0, 1, 0);

	
	if (light == 0)
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glColor3f(1, 0, 0);
	glutSolidSphere(3, 60, 60);
	// 지구
	glPushMatrix();
	glRotatef(earth_revolution, 0, 1, 0);
	glTranslatef(5, 0, 0);
	// 지구 위치 조명
	if(light==1)
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glRotatef(earth_rotation, 0, 1, 0);
	glColor3f(0, 0, 1);
	glutSolidSphere(1, 40, 40);
	// 달
	glRotatef(moon_revolution, 0, 1, 0);
    glTranslatef(2, 0, 0);
	// 달 위치 조명
	if (light == 2)
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glRotatef(moon_rotation, 0, 1, 0);
	glColor3f(1, 1, 1);
	glutSolidSphere(0.25, 20, 20);
	glPopMatrix();
	// 화성
	glPushMatrix();
	glRotatef(mars_revolution, 0, 1, 0);
	glTranslatef(10, 0, 0);
	// 화성 위치 조명
	if (light == 3)
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glRotatef(mars_rotation, 0, 1, 0);
	glColor3f(0.5, 0, 0);
	glutSolidSphere(1, 40, 40);
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
	glutKeyboardFunc(keyboard);



	/* Looping 시작 */
	glutMainLoop();

	return 0;
}
