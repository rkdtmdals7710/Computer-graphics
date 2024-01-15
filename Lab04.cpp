#include <gl/glut.h>
#include <stdio.h>
#include <windows.h>
#include <string.h>

double spin = 0;
int spin_state = FALSE;
int draw_mode = 0;
bool blending_on = FALSE;
bool antialiasing_on = FALSE;
GLint point_x[100] = { 0, }, point_y[100] = { 0, };	
GLint line_x[200] = { 0, }, line_y[200] = {0,};		
GLint triangle_x[300] = { 0, }, triangle_y[300] = {0,};		
int point_index = 0;		
int line_index = 0;		
int triangle_index = 0;		

void init(void);
void draw(void);
void draw_point(int x, int y);
void draw_line(int x1, int y1, int x2, int y2);
void draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3);
void draw_string(void* font, const char* str, int x, int y);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void keyboard(unsigned char key, int x, int y);
void special_keyboard(unsigned char key, int x_trans, int y_trans);
void sub_menu_function(int option);
void main_menu_function(int option);
void idle(void);

int main(int argc, char** argv) {
	int submenu1;
	/* Window 초기화 */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(300, 300);
	glutCreateWindow("My First GL Program");
	init(); // -> 사용자 초기화 함수
	/* Popup menu 생성 및 추가 */
	submenu1 = glutCreateMenu(sub_menu_function);
	glutAddMenuEntry("Draw mode OFF", 0);
	glutAddMenuEntry("Point", 1);
	glutAddMenuEntry("Line", 2);
	glutAddMenuEntry("Triangle", 3);
	/* 3개의 항목이 있는 메뉴 생성 */
	glutCreateMenu(main_menu_function);
	glutAddMenuEntry("Quit", 999);
	glutAddMenuEntry("Clear", 11);
	glutAddSubMenu("Draw mode", submenu1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	/* Callback 함수 정의 */
	glutIdleFunc(idle);
	glutDisplayFunc(draw);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);

	/* Looping 시작 */ 
	glutMainLoop();

	return 0;
}

void init(void)
{
	/* 화면의 기본색으로 dark blue 설정 */
	glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
	/* 화면 좌표 정보 설정 */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); // 행렬을 단위행렬로 초기화
	gluOrtho2D(0.0f, 500.0f, 0.0f, 500.0f);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA);// Alpha-Blending 방식 설정
	
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);// Antialiasing 품질 결정
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
}

void draw(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor3f(1.0f, 1.0f, 1.0f);		
	draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "Hello World!", 20, 20);

	if (antialiasing_on == 1) {
		glEnable(GL_POINT_SMOOTH); 
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_POLYGON_SMOOTH); 
	}

	else {
		glDisable(GL_POINT_SMOOTH);
		glDisable(GL_LINE_SMOOTH);
		glDisable(GL_POLYGON_SMOOTH);
	}

	if (blending_on == 1)
		glEnable(GL_BLEND);

	else
		glDisable(GL_BLEND);

	if (draw_mode == 1) {
		for (int i = 0; i < point_index; i++) {
			draw_point(point_x[i], 500 - point_y[i]);
		}
	}

	else if (draw_mode == 2) {
		for (int i = 0; i < line_index; i+=2) {
			draw_line(line_x[i] , 500 - line_y[i], line_x[i+1],500 - line_y[i+1]);
		}
	}
	else if (draw_mode == 3) {
		for (int i = 0; i < triangle_index; i += 3) {
			draw_triangle(triangle_x[i], 500 - triangle_y[i], triangle_x[i + 1], 500 - triangle_y[i + 1], triangle_x[i+2], 500 - triangle_y[i+2]);
		}
	}

	glFlush();
	glutSwapBuffers();
}

void draw_point(int x, int y) {
	glColor4f(1.0f, 0.0f, 0.0f, 0.5f);
	/* 점의 크기 (초기값은 1.0)*/
	glPointSize(20.0f);
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
}


void draw_line(int x1, int y1, int x2, int y2)
{
	glLineWidth(4.0f);

	glLineStipple(1, 0x00FF);

	/* 선의 색상을 노란색으로 */
	glColor4f(1.0f, 1.0f, 0.0f, 0.5f);
	glBegin(GL_LINES);
	glVertex2i(x1, y1);
	glVertex2i(x2, y2);
	glEnd();
}

void draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3) {
	// 면의 색상
	glColor4f(0.0f, 1.0f, 0.0f, 0.5f);
	glBegin(GL_TRIANGLES);
	glVertex2i(x1, y1);
	glVertex2i(x2, y2);
	glVertex2i(x3, y3);


	glEnd();
}

void draw_string(void* font, const char* str, int x, int y) {
	glRasterPos2i(x, y);		// 문자열의 최초 출력 위치 설정
	for (unsigned i = 0; i < strlen(str); i++)
		glutBitmapCharacter(font, str[i]);
}

void mouse(int button, int state, int x, int y) {
	/* 인자들을 해석해서 원하는 기능을 구현 */
	printf("Mouse button is clicked! (%d, %d, %d, %d)\n", button, state, x, y);

	
	if (draw_mode == 1 && state == GLUT_DOWN) {
		point_x[point_index] = x;
		point_y[point_index] = y;
		point_index++;
	}

	if (draw_mode == 2 && state == GLUT_DOWN) {
		line_x[line_index] = x;
		line_y[line_index] = y;
		line_index++;
	}

	if (draw_mode == 3 && state == GLUT_DOWN) {
		triangle_x[triangle_index] = x;
		triangle_y[triangle_index] = y;
		triangle_index++;
	}
}

void motion(int x, int y)
{
	/* 인자들을 해석해서 원하는 기능을 구현 */
	printf("Mouse is moving! (%d, %d)\n", x, y);
}

void keyboard(unsigned char key, int x, int y) {
	/* 인자들을 해석해서 원하는 기능을 구현 */
	printf("You pressed %c\n", key);
	if (key == 's') {
		if (spin_state == TRUE)
			spin_state = FALSE;
		else
			spin_state = TRUE;
	}

	if (key == 'a') {
		if (antialiasing_on == 0) {
			antialiasing_on = 1;
			printf("antialinasing on \n");
		}

		else {
			antialiasing_on = 0;
			printf("antialinasing off \n");
		}
	}

	if (key == 'b') {
		if (blending_on == 0) {
			blending_on = 1;
			printf("alpha blending on \n");
		}

		else {
			blending_on = 0;
			printf("alpha blending off \n");
		}
	}

	glutPostRedisplay();
}


void special_keyboard(unsigned char key, int x_trans, int y_trans) {
	printf("You pressed %c\n", key);

	if (key == 'r')
		spin -= 10;

	else if (key == 'l')
		spin += 10;


	glutPostRedisplay();
}



void sub_menu_function(int option) {
	printf("Submenu %d has been selected\n", option);

	if (option == 0){
		draw_mode = 0;
	}

	else if (option == 1)
		draw_mode = 1;

	else if (option == 2)
		draw_mode = 2;

	else if (option == 3)
		draw_mode = 3;
}

void main_menu_function(int option) {
	printf("Main menu %d has been selected\n", option);
	if (option == 999) {
		exit(0);
	}
}


void idle(void)
{
	if (spin_state == TRUE) {
		spin = spin + 0.01;
		if (spin > 360) spin -= 360;
		glutPostRedisplay();
	}
}
