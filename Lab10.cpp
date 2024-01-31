#include <gl/glut.h>
#include <stdio.h>
#include <Windows.h>

double earth_revolution = 0;
double earth_rotation = 0;
double mars_revolution = 0;
double mars_rotation = 0;
double moon_revolution = 0;
double moon_rotation = 0;
int light = -1;
int a = 0;

void init(void) {
	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
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

	printf("init function called\n");
}


void draw_string(void* font, const char* str, int x, int y, int z)
{
	glRasterPos3i(x, y, z);
	for (int i = 0; i < strlen(str); i++)
		glutBitmapCharacter(font, str[i]);
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
		printf("light's position is %c\n", key);
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
	//glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	gluLookAt(10, 10, 10, 0, 0, 0, 0, 1, 0);

	

	if (light == 0) {
		light_position[0] = 10;
		light_position[1] = 10;
		light_position[2] = 10;
	}
	

	if (light == 4) {
		light_position[0] = -10;
		light_position[1] = -5;
		light_position[2] = 20;
	}

	glColor3f(0, 0, 0);
	draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "12181732 Kang Seungmin", -10, -15, 0);

	//태양
	if (light == 1) {
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
		glColor3f(1, 0, 0);
		GLfloat emission0[] = { 1.0f, 0.0f, 0.0f, 1.0f };
		glMaterialfv(GL_FRONT, GL_EMISSION, emission0);
		glutSolidSphere(3, 30, 30);
		emission0[0] = 0.0f;
		glMaterialfv(GL_FRONT, GL_EMISSION, emission0);
	}

	else {
		glColor3f(1, 0, 0);
		glutSolidSphere(3, 30, 30);
	}

	// 지구
	glPushMatrix();
	glRotatef(earth_revolution, 0, 1, 0);
	glTranslatef(5, 0, 0);

	if (light == 2) {
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
		glRotatef(earth_rotation, 0, 1, 0);
		glColor3f(0, 0, 1);
		GLfloat emission1[] = { 0.0f, 1.0f, 0.7f, 1.0f };
		glMaterialfv(GL_FRONT, GL_EMISSION, emission1);
		glutSolidSphere(1, 30, 30);
		emission1[1] = 0.0f;
		emission1[2] = 0.0f;
		glMaterialfv(GL_FRONT, GL_EMISSION, emission1);
	}

	else {
		glRotatef(earth_rotation, 0, 1, 0);
		glColor3f(0, 0, 1);
		glutSolidSphere(1, 30, 30);
	}


	// 달
	glRotatef(moon_revolution, 0, 1, 0);
	glTranslatef(2, 0, 0);

	if (light == 3) {
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
		glRotatef(moon_rotation, 0, 1, 0);
		glColor3f(1, 1, 1);
		GLfloat emission2[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT, GL_EMISSION, emission2);
		glutSolidSphere(0.25, 20, 20);
		emission2[0] = 0;
		emission2[1] = 0;
		emission2[2] = 0;
		glMaterialfv(GL_FRONT, GL_EMISSION, emission2);
	}
	else {
		glRotatef(moon_rotation, 0, 1, 0);
		glColor3f(1, 1, 1);
		glutSolidSphere(0.25, 20, 20);
	}
	glPopMatrix();

	// 화성
	glPushMatrix();
	glRotatef(mars_revolution, 0, 1, 0);
	glTranslatef(10, 0, 0);
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
