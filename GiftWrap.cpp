
#include <iostream>
#include <vector>
#include <GL/glut.h> 


#include <conio.h> 
#include <windows.h>

using namespace std;

struct Point
{
	int x;
	int y;
	Point(){}
	Point(int x,int y)
	{
		this->x=x;
		this->y=y;
	} 

};

vector<Point> points;
vector<Point> convexHullPoints;
GLfloat RED[3] = {1,0,0};
int noOfPoints=0;
void convexHull(vector<Point> point, int n);

void drawPoints()
{
	glColor3fv(RED);
	glBegin(GL_POINTS);
	for(int i =0;i <points.size();i++)
	{
		glVertex3f(points[i].x, points[i].y,0);   
	
	}
	glEnd();
	glFlush();
}   

void display(void)
{   
	glClearColor(1.0,1.0,1.0,0.0);
	glClear (GL_COLOR_BUFFER_BIT);
	glLoadIdentity(); 
	glPointSize(8.0);
	drawPoints();

}
void reshape(int w, int h)
{   
	glViewport(0,0,(GLsizei)w,(GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0,1368,768,0,-1.0,1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void spindisplay(void)
{       
	glutPostRedisplay();
}


void mouse(int btn, int state, int x, int y)
{
	if(btn==GLUT_LEFT_BUTTON && state==GLUT_DOWN)   
	{

		points.push_back(Point(x,y));
		noOfPoints++;
		glutPostRedisplay();
		convexHullPoints.clear();
	}
	if(btn==GLUT_RIGHT_BUTTON && state==GLUT_DOWN)   
	{
		convexHull(points, noOfPoints);
	}
}
void myDisplay(void) 
{ 
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f (1.0,1.0,0);
	glBegin(GL_POLYGON); 
    
	for(int i=0; i < convexHullPoints.size();i++)
	{	
		
		glVertex2i(convexHullPoints[i].x,convexHullPoints[i].y);
	}

	glEnd();
	glColor3f (1.0,0.0,0);
	glBegin(GL_POINTS); 
	for(int i=0; i < points.size();i++)
	{	
		glVertex2i(points[i].x,points[i].y);
	}

	glEnd();
	glFlush();
} 


int CCW(Point p, Point q, Point r)
{
	int val = (q.y - p.y) * (r.x - q.x) -
		(q.x - p.x) * (r.y - q.y);

	if (val == 0) 
	{
		return 0;  
	}
	if (val > 0)
	{
		return 1;
	}
	else
	{
		return 2;
	}
}

void convexHull(vector<Point> point, int n)
{
	if (n < 3)
	{
		return;
	}
	int l = 0;
	for (int i = 1; i < n; i++)
	{
		if (points[i].x < points[l].x)
		{
			l = i;
		}
	}
	int p = l, q;
	do
	{
		q = (p+1)%n;
		for (int i = 0; i < n; i++)
		{
			if (CCW(points[p], points[i], points[q]) == 2)
			{
				q = i;
			}
		}
		convexHullPoints.push_back(Point(points[q].x,points[q].y));
		p = q; 

	} while (p != l);
	glutDisplayFunc(myDisplay);

}


int main(int argc,char * argv[])
{   
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowSize(1366,768);
	glutInitWindowPosition(0,0);
	glutCreateWindow("GiftWrap Algorithm");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutIdleFunc(spindisplay);
	glutMainLoop();
	
	return 0;
}