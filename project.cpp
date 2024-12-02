#include <gl/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <GL/freeglut.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <cmath>
#include <fstream>
#include <iostream>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")

using namespace std;

const int window_width = 500, window_height = 500;
const int start_point = 300;
const int ufo_movement_limit = 5;
const double home_size = 1;
const double ufo_size = 1;
const double asteroid_size = 1;
const int asteroid_distance = 3;
int camera_mode = 1;
int asteroid_number = start_point / asteroid_distance, bullet_number = 99;
int home_location[3] = { 0,0,0 };
int y[100], z[100];
int *asteroid, *asteroid_arr_y, *asteroid_arr_z;
int** asteroid_point;
double go_straight = 0, up_down = 0, left_right = 0, bullet = 0, ufo_position[3], shoot_position[3];
double speed = 3;
bool pause = true, hit = false, shooted = false, background = false;
GLuint* texName3 = new GLuint();
GLUquadricObj* m_pQuadric;

void init(void);
void draw(void);
void draw_background(void);
void draw_home(void);
void draw_asteroid(void);
void control(void);
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void idle(void);
void resize(int width, int height);
void asteroid_arr(void);
void background_arr(void);
void ufo_up();
void ufo_down();
void ufo_left();
void ufo_right();
void shoot();
int getRandomNumber(int min, int max);
double point_distance(int point1[], double point2[], int dimension);
void collision_check(void);
void home_check(void);
void shooting_check(void);
void open_txt(void);
void sphereTextureMapping();

int main(int argc, char** argv) {
	open_txt();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(window_width, window_height);
	glutInitWindowPosition(800, 200);
	glutCreateWindow("12181732 KangSeungMin");
	
	asteroid_arr();
	
	background_arr();

	PlaySound(TEXT("maintheme.wav"), 0, SND_FILENAME | SND_ASYNC);
	
	init();
	
	glutIdleFunc(idle);
	glutDisplayFunc(draw);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutReshapeFunc(resize);

	m_pQuadric = gluNewQuadric();

	glutMainLoop();

	return 0;
}

void init(void)
{
	/* ȭ���� �⺻������ dark blue ���� */
	glClearColor(0.1f, 0.1f, 0.4f, 1.0f);
	/* ȭ�� ��ǥ ���� ���� */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);

	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST); 
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST); 
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE); 
	glEnable(GL_COLOR_MATERIAL);

	glMatrixMode(GL_PROJECTION);
}


void idle(void)
{
	speed += 0.01;

	if (pause == false){
		go_straight += speed * 0.01;
	}

	collision_check();

	if (shooted == true) {
		bullet += 1;
		shooting_check();
	}

	home_check();


	glutPostRedisplay();
}

void draw(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if (camera_mode == 1)
		gluLookAt(ufo_position[0] + 7, ufo_position[1], ufo_position[2] + 2, ufo_position[0]-10, ufo_position[1], ufo_position[2] + 1 , 0, 0, 1);
	else if (camera_mode == 2)
		gluLookAt(ufo_position[0] - ufo_size, ufo_position[1], ufo_position[2], ufo_position[0] - 10, ufo_position[1], ufo_position[2],  0, 0, 1);
	else
		printf("camera error occured!\n");

	GLfloat light_position[] = { ufo_position[0] - ufo_size, ufo_position[1], ufo_position[2], 1.0 };
	GLfloat light0_ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat light0_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat light0_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light0_ambient);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 1.0);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 15.0);
	GLfloat spot_direction[] = { -1.0, 0.0, 0.0, 0.0 }; 
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);

	draw_background();

	draw_home();

	draw_asteroid();

	control();

	if (shooted == true) {
		shoot();
	}

	glFlush();
	glutSwapBuffers();
}


void draw_background(void)
{
	glPushMatrix();
	glTranslatef(-150, 0, 0);
	for (int i = 0; i < 100; i++) {
		glPushMatrix();
		glTranslatef(0, y[i], z[i]); // random number generator�� �̹� y,z�迭�� ���� �ѹ��� ä���� ����
		glColor3f(1.0f, 1.0f, 1.0f);
		GLfloat emission0[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT, GL_EMISSION, emission0); // emission�� ���� ��� ���� �������� �Ѵ�
		glutSolidSphere(0.5, 30, 30); 
		glTranslatef(0, -y[i], -z[i]);
		emission0[1] = 0.0f; emission0[2] = 0.0f; emission0[0] = 0.0f;
		glMaterialfv(GL_FRONT, GL_EMISSION, emission0);
		glPopMatrix();
	}
	glPopMatrix();
}


void draw_home(void) {
	glColor3f(0.0f, 0.0f, 0.7f);
	GLfloat emission1[] = { 0.0f, 0.0f, 0.7f, 1.0f };
	glMaterialfv(GL_FRONT, GL_EMISSION, emission1);
	glutSolidSphere(home_size, 60, 60);
	 emission1[2] = 0.0f;
	glMaterialfv(GL_FRONT, GL_EMISSION, emission1);
}

void draw_asteroid(void) {
	for (int i = 0; i < asteroid_number; i++) {
		if (asteroid[i] == 1) {
			glPushMatrix();
			glTranslatef(asteroid_point[i][0], 0, 0);
			glPushMatrix();
			glTranslatef(0, asteroid_point[i][1], asteroid_point[i][2]);
			glColor3f(0.2f, 0.05f, 0.05f);
			GLfloat emission2[] = { 0.2f, 0.05f, 0.05f, 1.0f };
			glMaterialfv(GL_FRONT, GL_EMISSION, emission2);
			glutSolidSphere(asteroid_size, 30, 30);
			emission2[0] = 0; emission2[1] = 0;	emission2[2] = 0;
			glMaterialfv(GL_FRONT, GL_EMISSION, emission2);
			glPopMatrix();
			glPopMatrix();
		}
	}
}

void control(void) {	
	glPushMatrix();
	glTranslatef(start_point, 0, 0);
	glColor3f(0.0f, 0.1f, 0.4f);
	glTranslatef(-go_straight, left_right, up_down);
	GLfloat emission4[] = { 0.0f, 1.0f, 0.7f, 1.0f };
	glMaterialfv(GL_FRONT, GL_EMISSION, emission4);
	glutSolidSphere(ufo_size, 30, 30);
	emission4[1] = 0.0f; emission4[2] = 0.0f;
	glMaterialfv(GL_FRONT, GL_EMISSION, emission4);
	glColor3f(1.0f, 1.0f, 1.0f);
	gluPartialDisk(m_pQuadric, 1, 1.5, 30, 30, 0, 180);
	glPopMatrix();

	ufo_position[0] = start_point - go_straight;
	ufo_position[1] = left_right;
	ufo_position[2] = up_down;
}


void shoot() {
	shoot_position[0] = ufo_position[0] - bullet;
	shoot_position[1] = ufo_position[1];
	shoot_position[2] = ufo_position[2];
	GLfloat light_position1[] = { shoot_position[0], shoot_position[1], shoot_position[2] , 1};

	glPushMatrix();
	glTranslatef(ufo_position[0] - ufo_size, ufo_position[1], ufo_position[2]);
	glColor3f(1.0f, 1.0f, 1.0f);
	glTranslatef(-bullet, 0, 0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position1);
	GLfloat emission3[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_EMISSION, emission3);
	glutSolidSphere(0.1, 30, 30);
	emission3[0] = 0; emission3[1] = 0;
	glMaterialfv(GL_FRONT, GL_EMISSION, emission3);
	glPopMatrix();
}


void keyboard(unsigned char key, int x, int y) {

	switch (key) {
	case 13:
		if (bullet_number == 0) {
			printf("There is no bullet\n");
			break;
		}
		bullet = 0;
		bullet_number--;
		shooted = true;
		printf("Lost number of bullet is %d\n",bullet_number);
		break;
	case 'w':
		ufo_up(); break;
	case 'a':
		ufo_left(); break;
	case 's':
		ufo_down(); break;
	case'd':
		ufo_right(); break;
	case ' ' : 
		if (pause == true)
			pause = false;
		else
			pause = true;
		break;
	defalt : break;
	}
}

void mouse(int button, int state, int x, int y) {
	if (camera_mode == 1 && state == GLUT_DOWN)
		camera_mode = 2;
	else if(camera_mode ==2 && state == GLUT_DOWN)
		camera_mode = 1;
}

void resize(int width, int height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)width / (float)height, 1, 500);
	glMatrixMode(GL_MODELVIEW);
}

void asteroid_arr(void) {
	int n = 5; // ���༺�� x�� ������ �ִ� �Ÿ�
	srand((unsigned int)time(NULL));

	asteroid = new int[asteroid_number];
	asteroid_arr_y = new int[asteroid_number];
	asteroid_arr_z = new int[asteroid_number];
	asteroid_point = new int* [asteroid_number];

	for (int i = 0; i < asteroid_number; i++) {
		asteroid[i] = 1;
		asteroid_point[i] = new int[3];
		asteroid_point[i][0] = (i + 1) * asteroid_distance;
	}

	for (int i = 0; i < asteroid_number; i++) {
		asteroid_arr_y[i] = getRandomNumber(-n, n);
		asteroid_point[i][1] = asteroid_arr_y[i];
	}

	for (int i = 0; i < asteroid_number; i++) {
		asteroid_arr_z[i] = getRandomNumber(-n, n);
		asteroid_point[i][2] = asteroid_arr_z[i];
	}
}

void background_arr(void) {

	if (background == false) {
		for (int i = 0; i < 100; i++) {
			y[i] = getRandomNumber(-200, 200);
			z[i] = getRandomNumber(-200, 200);
		}
		background == true;
	}
}

int getRandomNumber(int min, int max) {
	return rand() % (max - min + 1) + min;
}

void ufo_up() {
	if (up_down <= ufo_movement_limit)
		up_down += 0.1;
		
	else
		printf("limit\n");
}
void ufo_down(){
	if (-ufo_movement_limit <= up_down)
		up_down -= 0.1;
		
	else
		printf("limit\n");
}
void ufo_right() {
	if (left_right <= ufo_movement_limit)
		left_right += 0.1;
	else
		printf("limit\n");
}
void ufo_left() {
	if (-ufo_movement_limit <= left_right)
		left_right -= 0.1;
	else
		printf("limit\n");
}


double point_distance(int point1[], double point2[], int dimension) {
	double distance = 0.0;

	for (int i = 0; i < dimension; i++) {
		distance += (point1[i] - point2[i]) * (point1[i] - point2[i]);
	}

	return sqrt(distance);
}

void collision_check(void) {
	hit = false;

	for (int i = 0; i < asteroid_number; i++) {
		if (asteroid[i]==1 && point_distance(asteroid_point[i], ufo_position, 3) <= asteroid_size + ufo_size) {
			hit = true;
			PlaySound(NULL, 0, 0);
			PlaySound(TEXT("explode.wav"), 0, SND_FILENAME | SND_ASYNC);
			Sleep(2500);
			printf("You Died\n");
			exit(0);
		}
	}
}

void shooting_check(void){

	if (shoot_position[0] <= 0) {
		shooted = false;
		return;
	}

	else
	for (int i = 0; i < asteroid_number; i++) {
	if (asteroid[i]==1 && point_distance(asteroid_point[i], shoot_position, 3) <= asteroid_size) {
			asteroid[i] = 0;
			shooted = false;
			return ;
			}
	}

}


void home_check(void) {
	if (point_distance(home_location, ufo_position, 3) <= home_size + ufo_size) {
		printf("Congratulation, mission success!!!!\n");
		exit(0);
	}

	if (ufo_position[0]<0 && point_distance(home_location, ufo_position, 3) > home_size + ufo_size) {
		printf(" Mission failed!\n");
		exit(0);
	}
}


void open_txt(void) {
	const char* filename = "HowToGame.txt"; 
	FILE* file;

	if (fopen_s(&file, filename, "r") == 0) { 
		char buffer[100]; 

		while (fgets(buffer, sizeof(buffer), file) != NULL) {
			printf("%s", buffer); 
		}
		fclose(file); 
	}
	else {
		printf("Failed to read file!\n");
	}
}
