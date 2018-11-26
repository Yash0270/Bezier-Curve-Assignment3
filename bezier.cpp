#include <GL/glut.h>
# include <iostream>
# include <stack>

using namespace std;

static int window;
static int menu_id;
static int value = 0;


struct point
{
	float x;
	float y;
};

point control_point[100];
point temp_array[100];
int number_cp = 0;
int moveControlPointState = 0;
int old_x = 0;
int old_y = 0;




// evaluate a point on a bezier-curve. t goes from 0 to 1.0
// @param t 	 : at which t ranging from 0 - 1 , we need to find the point 
// @return point : returning the calculated the point thorugh de castelau algorithm
point bezier(const float t)
{

	for (int i = 0; i < number_cp; i++) {
		temp_array[i] = control_point[i];
	}

	for (int k = 1; k < number_cp; k++) {
		for (int i = 0; i < number_cp - k; i++) {
			temp_array[i].x = (1 - t)* temp_array[i].x + t * temp_array[i + 1].x;
			temp_array[i].y = (1 - t)* temp_array[i].y + t * temp_array[i + 1].y;
		}
	}

	return temp_array[0];



}

// callback func of glutDisplayFunc
void Display()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT); // clear display window

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	const double w = glutGet(GLUT_WINDOW_WIDTH);
	const double h = glutGet(GLUT_WINDOW_HEIGHT);
	gluOrtho2D(0.0, w, 0.0, h);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	for (int i = 0; i < 1000; ++i)
	{
		if (number_cp < 1)
			break;

		point p;
		float t = static_cast<float>(i) / 999.0;
		p = bezier(t);


		glPointSize(1.0f);
		glBegin(GL_POINTS);

		int x = p.x;
		int y = p.y;

		glColor3f(0, 1, 0);
		glVertex2i(x, h - y);

		glEnd();
	}

	glPointSize(5.0f);
	glBegin(GL_POINTS);
	for (int i = 0; i < number_cp; i++)
	{
		int x = control_point[i].x;
		int y = control_point[i].y;

		glColor3f(1, 0, 0);
		glVertex2i(x, h - y);
	}
	glEnd();

	glFlush();
}


// For adding the control points
// @param x : representing the x  coordinate to add the control point 
// @param y : representing the y coordinate to add the control point 
void addControlPoint(int x, int y) {
	control_point[number_cp].x = x; control_point[number_cp].y = y;
	number_cp++;
}


// For deleting the control points 
// @param x : representing the x coordinate to delete the control point 
// @param y : representing the y coordinate to delete the control point 
void deleteControlPoint(int x, int y) {
	for (int i = 0; i < number_cp; i++) {
		if (abs(x - control_point[i].x) < 10 && abs(control_point[i].y - y) < 10) {
			for (int j = i; j < number_cp - 1; j++) {
				control_point[j].x = control_point[j + 1].x;
				control_point[j].y = control_point[j + 1].y;
			}
			number_cp--;
			break;
		}
	}
}


// Callback func of glutMouseFunc
// @param button : represeting  which buton pressed, the left , right or middle 
// @param state : pressed or not pressed
// @param x : representing the x  coordinate of mouse 
// @param y : representing the y coordinate of mouse 
void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (value == 3) {
			addControlPoint(x, y);
		}
		else if (value == 1) {
			deleteControlPoint(x, y);
		}
		else if (value == 2) {

			old_x = x;
			old_y = y;
			
		}
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		
		for (int i = 0; i < number_cp; i++) {
			if (abs(old_x - control_point[i].x) < 10 && abs(control_point[i].y - old_y) < 10) {
				control_point[i].x = x;
				control_point[i].y = y;
				printf("Kuch ho rha hai \n");
				break;
			}
		}
		

	}


	glutPostRedisplay();
}

// callback func for glutCreateMenu 
// @param num : represting the menu entry number
void menu(int num) {
	if (num == 0) {
		glutDestroyWindow(window);
		exit(0);
	}
	else {
		value = num;
	}
	glutPostRedisplay();
}

// For creating menu when pressed right click
void createMenu(void) {

	menu_id = glutCreateMenu(menu);
	glutAddMenuEntry("Add a point", 3);
	glutAddMenuEntry("Delete a point", 1);
	glutAddMenuEntry("Move a point", 2);
	glutAddMenuEntry("Quit", 0);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// For reshaping the window size 
// @param x : representing the width
// @param y : representing the height
void reshape(int x, int y) {

	/* define the viewport transformation */
	glViewport(0, 0, x, y);

}

// When pressed ESC key , window closes
// @param key : the key pressed 
// @param xx : representing the x  coordinate 
// @param yy : representing the y coordinate  
void processNormalKeys(unsigned char key, int xx, int yy) {
	if (key == 27)
		exit(0);
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(1200, 800);
	window = glutCreateWindow("Beizer Curve");

	glutReshapeFunc(reshape);
	glutKeyboardFunc(processNormalKeys);
	glutMouseFunc(mouse);
	glutDisplayFunc(Display);
	createMenu();

	glutMainLoop();
	return 0;
}