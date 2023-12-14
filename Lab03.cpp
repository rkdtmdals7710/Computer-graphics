#include <gl/glut.h>
#include <stdio.h>

double spin = 0;
int spin_state = FALSE;

void init(void);

void draw(void);

void draw_point(void);

void draw_line(void);

void draw_triangle(void);

void mouse(int button, int state, int x, int y);

void motion(int x, int y);

void keyboard(unsigned char key, int x, int y);

void special_keyboard(unsigned char key, int x_trans, int y_trans);

void sub_menu_function(int option);

void main_menu_function(int option);

void idle(void);

void draw_blending_triangle(){
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor3f(0.0f, 1.0, 1.0, 0.75);
	glBegin(GL_TRIANGLES);
	glVertex2i(450, 450);
	glVertex2i(450, 50);
	glVertex2i(150, 250);
	glEnd();

	glColor3f(1.0, 1.0, 0.0, 0.75);
	glBegin(GL_TRIANGLES);
	glVertex2i(50, 50);
	glVertex2i(50, 450);
	glVertex2i(350, 250);
	glEnd();

}

int main(int argc, char** argv){
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
	glutAddMenuEntry("Sub1", 1);
	glutAddMenuEntry("Sub2", 2);
	glutAddMenuEntry("Sub3", 3);
	/* 3개의 항목이 있는 메뉴 생성 */
	glutCreateMenu(main_menu_function);
	glutAddMenuEntry("Quit", 999);
	glutAddMenuEntry("Go!", 11);
	glutAddSubMenu("Sub Menu", submenu1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	/* Callback 함수 정의 */
	glutIdleFunc(idle);
	glutDisplayFunc(draw);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);
	glutKeyboardFunc(special_keyboard);
	

	/* Looping 시작 */ glutMainLoop();

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

}

void draw(void){
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity();

	
	glTranslatef(250, 250, 0);
	glRotatef(spin, 0, 0, 1);
	glTranslatef(-250, -250, 0);


	//draw_point();
	//draw_line();
	draw_blending_triangle();

	glFlush();
	glutSwapBuffers();
}

void draw_point(void) {
	/* 점의 색상 */
	glColor3f(0.0f, 1.0f, 0.0f); 
	/* 점의 크기 (초기값은 1.0)*/ 
	glPointSize(8.0f);
	glBegin(GL_POINTS);
	glVertex2i(400, 400);
	glVertex2i(200, 200);
	glVertex2i(450, 300);
	glEnd();
}

void mouse(int button, int state, int x, int y) {
	/* 인자들을 해석해서 원하는 기능을 구현 */
	printf("Mouse button is clicked! (%d, %d, %d, %d)\n", button, state, x, y);
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
	glutPostRedisplay();
}


void special_keyboard(unsigned char key, int x_trans, int y_trans) {
	printf("You pressed %c\n", key);

	if(key=='r')
		spin -= 10;
		
	else if(key=='l')
		spin += 10;
	
	
	glutPostRedisplay();
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

void draw_line(void)
{
	glLineWidth(4.0f); 
	glEnable(GL_LINES); 
	glLineStipple(1, 0x00FF);

	/* 선의 색상을 노란색으로 */ 
	glColor3f(1.0f, 1.0f, 0.0f); 
	glBegin(GL_LINES);
	glVertex2i(100, 100); 
	glVertex2i(400, 100);
	glVertex2i(100, 400); 
	glVertex2i(400, 400);
	glEnd();
}
//
//void draw_triangle(void) {
//	// 면의 색상
//	glColor3f(0.0f, 1.0f, 0.0f);
//	glBegin(GL_TRIANGLES);
//	glVertex2i(200, 200); 
//	glVertex2i(300, 200);
//	glVertex2i(250, 350);
//	
//
//	glEnd();
//}

void idle(void)
{
	if (spin_state == TRUE){
		spin = spin + 0.1;
		if (spin > 360) spin -= 360;
		glutPostRedisplay();
	}
}
