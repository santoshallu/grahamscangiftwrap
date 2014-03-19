
#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <GL/glut.h> 
#include <conio.h> 
#include <windows.h>
#include <stdio.h>
#include <sstream>
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
Point p0;
void convexHull();
int WindowWidth=1366;
int WindowHeight=768;

Point elements(vector<Point> &ps)
{
	Point p = ps[ps.size()-1];
	ps.pop_back();
	Point res = ps[ps.size()-1];
	ps.push_back(p);
	return res;
}


void printtext(int x, int y, string String)
{
	//(x,y) is from the bottom left of the window
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0,WindowWidth,WindowHeight,0,-1.0,1.0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glPushAttrib(GL_DEPTH_TEST);
	glDisable(GL_DEPTH_TEST);
	glRasterPos2i(x,y);
	for (int i=0; i<String.size(); i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, String[i]);
	}
	glPopAttrib();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}
void drawPoints()
{


   	
	char input[64];
	string coordinates="";

	string x;
	string y;

	glColor3fv(RED);
	for(int i =0;i <points.size();i++)
	{
		ostringstream osx ;
		osx << points[i].x ;
		x=osx.str() ;
		ostringstream osy ;
		osy << points[i].y ;
		y=osy.str() ;
		coordinates = "("+x+","+y+")";
		char * cstr = new char [coordinates.length()+1];
		strcpy (cstr, coordinates.c_str());
		sprintf(input, cstr);
		printtext(points[i].x,points[i].y,input);

		glBegin(GL_POINTS);

		glVertex3f(points[i].x, points[i].y,0);   
		glEnd();
	}
	glutSwapBuffers();

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
	glOrtho(0.0,WindowWidth,WindowHeight,0,-1.0,1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void mouse(int btn, int state, int x, int y)
{
	if(btn==GLUT_LEFT_BUTTON && state==GLUT_DOWN)   
	{
           PlaySound("coin.WAV", NULL, SND_ASYNC);
		points.push_back(Point(x,y));
		glutPostRedisplay();
		convexHullPoints.clear();
	}
	if(btn==GLUT_RIGHT_BUTTON && state==GLUT_DOWN)   
	{
		convexHull();
	    PlaySound("stageover.WAV", NULL, SND_ASYNC);
	}
}
void myDisplay(void) 
{ 
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0,1.0,0.0);
	glBegin(GL_POLYGON);    
	for(int i=0; i < convexHullPoints.size();i++)
	{	
		glVertex2i(convexHullPoints[i].x,convexHullPoints[i].y);
	}
	glEnd();
	glColor3f(1.0,0.0,0);
	glBegin(GL_POINTS); 
	for(int i=0; i < points.size();i++)
	{	

		glVertex2i(points[i].x,points[i].y);
	}

	glEnd();
	glutSwapBuffers();
} 


int dist(Point p1, Point p2)
{
	return (p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y);
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

int angleComp(const void *point1, const void *point2)
{
	Point *p1 = (Point *)point1;
	Point *p2 = (Point *)point2;
	int direction = CCW(p0, *p1, *p2);
	if (direction == 0)
	{
		if (dist(p0, *p2) >= dist(p0, *p1))
		{	   
			return -1;
		}
		else
		{
			return 1;
		}
	}
	else if(direction == 2)
	{
		return -1;
	}
	else
	{
		return 1;
	}
}


void convexHull()
{
	int n = points.size();
	if (n < 3)
	{
		return;
	}


	Point temp;
	for (int i = 1; i < n; i++)
	{
		if ((points[i].y< points[0].y) || (points[i].y == points[0].y  && points[i].x < points[0].x))
		{
			temp=points[0];
			points[0]=points[i];
			points[i]=temp;

		}
	}
	p0 = points[0];
	qsort(&points[0], n, sizeof(Point), angleComp);
	convexHullPoints.push_back(points[0]);
	convexHullPoints.push_back(points[1]);


	for (int i = 2; i < n; i++)
	{
		while (CCW(elements(convexHullPoints), convexHullPoints[convexHullPoints.size()-1], points[i]) != 2)
		{
			convexHullPoints.pop_back();
		}
		convexHullPoints.push_back(points[i]);
	}
	glutDisplayFunc(myDisplay);
	glutPostRedisplay();

}


int main(int argc,char * argv[])
{   
	
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(WindowWidth,WindowHeight);
	glutInitWindowPosition(0,0);
	glutCreateWindow("Graham Scan Algorithm");
	glutMouseFunc(mouse);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}