//Derik Vega
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glu.h>
#include <math.h>
#include "shape.h"
#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <math.h>
#endif
#ifdef __linux__
#include <GL/glut.h>
#include <math.h>
#endif

#define shapelimit 10000

GLsizei left_mouse_button_x = 0, left_mouse_button_y = 0;
GLsizei right_mouse_button_x = 0, right_mouse_button_y = 0;
struct shape tempshape; //shape currently being drawn
int drawing, shapeinc; //if drawing,  counter for shapes array
struct shape shapes[shapelimit]; //drawn shapes

void keyboard (unsigned char, int, int);

//DRAW SHAPES
void drawsquare(struct shape in){  //DRAW SQUARE
    glColor3f (in.r, in.g, in.b); //set color
	if(in.filled==1){ //set filled, unfilled
		glBegin (GL_POLYGON);
	}
	else{
		glBegin (GL_LINE_LOOP);
	}
	glVertex3f(in.x[0], in.y[0], 0); //Drawing square
	glVertex3f(in.x[1], in.y[0], 0);
	glVertex3f(in.x[1], in.y[1], 0);
	glVertex3f(in.x[0], in.y[1], 0);
	glEnd ();
	glFlush ();
}
void drawcircle(struct shape in){  //DRAW CIRCLE
    float theta, circle_iterations = 24.0;
	int x0=in.x[0], x1=in.x[1];
	int y0=in.y[0], y1=in.y[1];
	float rx, ry, cx, cy; //radius and center
	cx = x0+(x1-x0)/2; //find center
	cy = y0+(y1-y0)/2;
	rx = abs(x1-x0)/2; //find radius
	ry = abs(y1-y0)/2;
    glColor3f (in.r, in.g, in.b); //set color
	if(in.filled==1){ //set filled, unfilled
		glBegin (GL_POLYGON);
	}
	else{
		glBegin (GL_LINE_LOOP);
	}
	for (theta = 0; theta < 2 * M_PI; theta += M_PI / circle_iterations) //Drawing circle
	{
		glVertex3f (cx + cos(theta) * rx, cy + sin(theta) * ry, 0.0f);
	}
	glEnd ();
	glFlush ();
}
void drawline(struct shape in){  //DRAW LINE
	//int x0=in.x[0], x1=in.x[1];
	//int y0=in.y[0], y1=in.y[1];
	glColor3f (in.r, in.g, in.b); //set color
	glBegin (GL_LINES);
	glVertex3f(in.x[0], in.y[0], 0); //Drawing line
	glVertex3f(in.x[1], in.y[1], 0);
	glEnd ();
	glFlush ();
}
void drawbezier(struct shape in){  //DRAW BEZIER
	int x0=in.x[0], x1=in.x[1], x2=in.x[2], x3=in.x[3];
	int y0=in.y[0], y1=in.y[1], y2=in.y[2], y3=in.y[3];
	float t, x, y;
	glColor3f (in.r, in.g, in.b); //set color
	glBegin (GL_LINE_STRIP);  //DRAWING A BEZIER
	for(t=0; t<=1; t+=.02){ //how precise to be, currently 50 points
        x=((1-t)*(1-t)*(1-t)*x0)+(3*(1-t)*(1-t)*t*x1)+(3*(1-t)*t*t*x2)+(t*t*t*x3);
        y=((1-t)*(1-t)*(1-t)*y0)+(3*(1-t)*(1-t)*t*y1)+(3*(1-t)*t*t*y2)+(t*t*t*y3);
        glVertex3f(x,y,0);
    }
	glEnd ();
	glFlush ();
}
//END DRAW SHAPES
void testdraw(){ //Used to test various drawings. Debugging mainly
    float t, x, y;
	glColor3f (tempshape.r, tempshape.g, tempshape.b);
	glBegin (GL_POLYGON);
	//glBegin (GL_LINES);
	//glBegin (GL_C)
	glVertex3f(tempshape.x[0], tempshape.y[0], 0);
	glVertex3f(tempshape.x[1], tempshape.y[0], 0);
	glVertex3f(tempshape.x[1], tempshape.y[1], 0);
	glVertex3f(tempshape.x[0], tempshape.y[1], 0);
	/*glBegin (GL_LINE_STRIP);  //DRAWING A BEZIER
	for(t=0; t<=1; t+=.02){ //how precise to be, currently 50 points
        x=((1-t)*(1-t)*(1-t)*40)+(3*(1-t)*(1-t)*t*80)+(3*(1-t)*t*t*150)+(t*t*t*260);
        y=((1-t)*(1-t)*(1-t)*100)+(3*(1-t)*(1-t)*t*20)+(3*(1-t)*t*t*180)+(t*t*t*100);
        glVertex3f(x,y,0);
    }*/
	glEnd ();
	glFlush ();
}

void display (void){
    int n;
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glClear (GL_COLOR_BUFFER_BIT);
	//printf("shape:%i  drawing:%i\n",tempshape.shape,drawing);
	//printf("tempshape x0:%i y0:%i x1:%i y1:%i x2:%i y2:%i x3:%i y3:%i, drawing: %i\n",
	// tempshape.x[0],tempshape.y[0],tempshape.x[1],tempshape.y[1],tempshape.x[2],tempshape.y[2],tempshape.x[3],tempshape.y[3],drawing);
	//glLoadIdentity ();
	//testdraw();
	//drawsquare(tempshape);
	for(n=0; n<shapeinc; n++){ //Draw all stored shapes
        if(shapes[n].shape==1){
            drawsquare(shapes[n]);
        }else if(shapes[n].shape==2){
            drawcircle(shapes[n]);
        }else if(shapes[n].shape==3){
            drawline(shapes[n]);
        }else if(shapes[n].shape==4){
            drawbezier(shapes[n]);
        }
	} //End draw stored
	if(tempshape.shape==1){ //Draw what user is drawing
        drawsquare(tempshape);
	}else if(tempshape.shape==2){
	    drawcircle(tempshape);
	}else if(tempshape.shape==3){
	    drawline(tempshape);
	}else if(tempshape.shape==4){
	    drawbezier(tempshape);
	} //End draw tempshape
	//draw_square_filled ();
	//draw_circle_filled ();
	//draw_circle_unfilled ();
	glFlush ();
	glutSwapBuffers();
}

void reshape (int w, int h){
	if(h==0){
		h = 1;
	}
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	glOrtho (0.0, 640, 480, 0, -1.0, 1.0);
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
}

void mouse (int mouse_button, int state, int x, int y){
	if ((mouse_button == GLUT_RIGHT_BUTTON) && (state == GLUT_DOWN)){
		if(drawing>=1){
			drawing++; //add up to get to next steps
			//printf("Click to place middle point: %i\n",drawing-2);
		}
		else{ //first click, begin drawing
			//left_mouse_button_x = x;
			//left_mouse_button_y = y;
			tempshape.x[0]=x;
			tempshape.y[0]=y;
			tempshape.x[1]=x;
			tempshape.y[1]=y;
			tempshape.x[2]=x;
			tempshape.y[2]=y;
			tempshape.x[3]=x;
			tempshape.y[3]=y;
			drawing = 1;
			if(tempshape.shape==4){
				printf("Click to place points\n");
			}
		}
		glutPostRedisplay ();
	}
	//printf("drawing:%i\n",drawing);
	/*if ((mouse_button == GLUT_RIGHT_BUTTON) && (state == GLUT_DOWN)){
		right_mouse_button_x = x;
		right_mouse_button_y = y;
		glutPostRedisplay ();
	}*/
}
void motionmouse(int x, int y){
	//printf("motion mouse: %i, %i\n",x,y);
	//printf("shape:%i  drawing:%i\n",tempshape.shape,drawing);
	if(tempshape.shape!=4){ //Follow mouse to resize shape in real-time
		tempshape.x[1]=x;
		tempshape.y[1]=y; //Need to fix this
	}
	if((tempshape.shape==4)&&(drawing==1)){ //handle bezier  step 1
		tempshape.x[1]=x;
		tempshape.y[1]=y;
		tempshape.x[2]=tempshape.x[1];
		tempshape.y[2]=tempshape.y[1];
		tempshape.x[3]=tempshape.x[1];
		tempshape.y[3]=tempshape.y[1];
		//drawing++; //drawing = 2
	}
	else if((tempshape.shape==4)&&(drawing==2)){ //bezier drawing  first click, step 3
		tempshape.x[2]=x;
		tempshape.y[2]=y;
		tempshape.x[1]=x;
		tempshape.y[1]=y;
		//drawing++; //drawing = 3;
	}
	else if((tempshape.shape==4)&&(drawing==3)){ //bezier drawing  second click, finish
		tempshape.x[2]=x;
		tempshape.y[2]=y;
		//drawing++; //drawing = 4;
	}
	//Sleep(10);
}
void passivemouse(int x, int y){
    int n;
	//printf("tempshape x0:%i y0:%i x1:%i y1:%i x2:%i y2:%i x3:%i y3:%i\n",
	//	tempshape.x[0],tempshape.y[0],tempshape.x[1],tempshape.y[1],tempshape.x[2],tempshape.y[2],tempshape.x[3],tempshape.y[3]);
	if(drawing>0){
		if(tempshape.shape!=4){ //Follow mouse to resize shape in real-time
			tempshape.x[1]=x;
			tempshape.y[1]=y; //Need to fix this
		}
		if((tempshape.shape==4)&&(drawing==1)){ //handle bezier  step 1   COPY of motion mouse so it remains updating at all times
			tempshape.x[1]=x;
			tempshape.y[1]=y;
			tempshape.x[2]=tempshape.x[1];
			tempshape.y[2]=tempshape.y[1];
			tempshape.x[3]=tempshape.x[1];
			tempshape.y[3]=tempshape.y[1];
			//drawing++; //drawing = 2
		}
		else if((tempshape.shape==4)&&(drawing==2)){ //bezier drawing  first click, step 3
			tempshape.x[2]=x;
			tempshape.y[2]=y;
			tempshape.x[1]=x;
			tempshape.y[1]=y;
			//drawing++; //drawing = 3;
		}
		else if((tempshape.shape==4)&&(drawing==3)){ //bezier drawing  second click, finish
			tempshape.x[2]=x;
			tempshape.y[2]=y;
			//drawing++; //drawing = 4;
		}
		/*if(tempshape.shape==4){ //handle bezier, follow mouse after release
			tempshape.x[1]=x;
			tempshape.y[1]=y;
		}
		else*/ if(((tempshape.shape<4)&&(drawing==2))||(drawing==4)){
			drawing = 0; //not drawing anymore
			for(n=0;n<4;n++){ //store tempshape in order to be redrawn every frame
				shapes[shapeinc].x[n] = tempshape.x[n];
				shapes[shapeinc].y[n] = tempshape.y[n];
			}
			//printf("store x0:%i y0:%i x1:%i y1:%i x2:%i y2:%i x3:%i y3:%i\n",
			// shapes[shapeinc].x[0],shapes[shapeinc].y[0],shapes[shapeinc].x[1],shapes[shapeinc].y[1],
			// shapes[shapeinc].x[2],shapes[shapeinc].y[2],shapes[shapeinc].x[3],shapes[shapeinc].y[3]);
			shapes[shapeinc].shape = tempshape.shape;
			shapes[shapeinc].r = tempshape.r;
			shapes[shapeinc].g = tempshape.g;
			shapes[shapeinc].b = tempshape.b;
			shapes[shapeinc].filled = tempshape.filled; //finish storing
			shapeinc++; //move to next in shapes array
			if(shapeinc>shapelimit){ //prevent going over array size
				shapeinc = shapelimit;
			}
			for(n=0;n<3;n++){ //reset tempshape
				tempshape.x[0]=0;
				tempshape.y[0]=0;
				tempshape.x[1]=0;
				tempshape.y[1]=0;
				tempshape.x[2]=0;
				tempshape.y[2]=0;
				tempshape.x[3]=0;
				tempshape.y[3]=0;
			}
			//tempshape.shape=0;
			//tempshape.r=1;
			//tempshape.g=1;
			//tempshape.b=1;
			//tempshape.filled=0; //finish reset
			printf("finished drawing shape: %i of %i\n", shapeinc,shapelimit); //alert drawing done when user lets go of left click
		}
	}
	//printf("passive mouse: %i, %i\n",x,y);
	//printf("tempshape x0:%i y0:%i x1:%i y1:%i\n",tempshape.x[0],tempshape.y[0],tempshape.x[1],tempshape.y[1]);
	//Sleep(10);
}

void process_menu (int value){
	int menuvalue = glutGetMenu();
	tempshape.filled = menuvalue%2; //1 = filled
	//printf("filled: %i\n", tempshape.filled);
	if((menuvalue==1)||(menuvalue==2)){ //Square
		tempshape.shape = 1;
	}
	else if((menuvalue==3)||(menuvalue==4)){ //Circle
		tempshape.shape = 2;
	}
	else{
		tempshape.shape = menuvalue-2; //Line and Bezier
	}
	/*if(tempshape.shape == 4){ //Prevent bezier drawing. Not ready.
		printf("Bezier is unavailable. Switching to outline square.\n");
		tempshape.shape = 1;
	}*/
	switch (value){
		case 1: //Red
			tempshape.r=1;
			tempshape.g=0;
			tempshape.b=0;
			//printf("menu: %i\n",glutGetMenu());
			break;
		case 2: //Green
			tempshape.r=0;
			tempshape.g=1;
			tempshape.b=0;
			break;
		case 3: //Blue
			tempshape.r=0;
			tempshape.g=0;
			tempshape.b=1;
			//printf("menu: %i\n",glutGetMenu());
			break;
		case 4: //White
			tempshape.r=1;
			tempshape.g=1;
			tempshape.b=1;
			break;
		case 5: //Magenta
			tempshape.r=1;
			tempshape.g=0;
			tempshape.b=1;
			break;
		case 6: //Cyan
			tempshape.r=0;
			tempshape.g=1;
			tempshape.b=1;
			break;
		case 7: //Yellow
			tempshape.r=1;
			tempshape.g=1;
			tempshape.b=0;
			break;
		case 8: //Purple
			tempshape.r=0.75;
			tempshape.g=0;
			tempshape.b=1;
			break;
		case 9: //Orange
			tempshape.r=1;
			tempshape.g=.5;
			tempshape.b=0;
			break;
		case 10: //Black
			tempshape.r=0;
			tempshape.g=0;
			tempshape.b=0;
			break;
		case 11:
			break;
		case 12:
			break;
		case 13:
			break;
		case 14:
			break;
		case 15:
			break;
		case 16:
			break;
		case 17:
			break;
		case 18:
			break;
		case 19:
			break;
		case 20:
			break;
		case 21:
			break;
		case 22:
			break;
		default:
			break;
	}
	printf("tempshape shape:%i r:%f g:%f b:%f filled:%i\n",tempshape.shape, tempshape.r, tempshape.g, tempshape.b, tempshape.filled); //show what was chosen
	glutPostRedisplay();
}

void keyboard (unsigned char key, int x, int y)
{
    switch (key)
    {
        case 8: //Backspace
            if(shapeinc > 0)
                printf("deleted drawing shape: %i\n", shapeinc); //alert deletion
                shapeinc--;
        break;
        case 27: //ESC key
            exit (0);
        break;
    }
}

int main(int argc, char *argv[]){
    //printf("Hello world!\n");
    int shapemenu, squaremenu, ellipsemenu; //ALL SHAPES
	int squarecolorfilledmenu, squarecoloroutlinemenu, ellipsecolorfilledmenu, ellipsecoloroutlinemenu; //SQUARE, ELLIPSE COLOR
	int linecolormenu, beziercolormenu; //LINE, BEZIER COLOR
	int n;
    //struct shape shapes[shapelimit];
	for(n=0;n<3;n++){ 		//Initialize temp shape
		tempshape.x[n]=0;
		tempshape.y[n]=0;
	}
	tempshape.shape=1;
	tempshape.r=1;
	tempshape.g=1;
	tempshape.b=1;
	tempshape.filled=0; 	//finish initializing temp shape
	drawing = 0; 	//bool, true if drawing
	shapeinc = 0; //increments shapes array
	printf("left click: place shape\nright click: open shapes\n1: square\n2: circle\n3: line\n4: bezier\n"); //instructions
	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize (640, 480);
	glutInitWindowPosition (0,0);
	glutCreateWindow ("Programming Project 1");

	//MENU STUFF
	squarecolorfilledmenu = glutCreateMenu (process_menu); //square outline color menu
		glutAddMenuEntry ("red", 1);
		glutAddMenuEntry ("green", 2);
		glutAddMenuEntry ("blue", 3);
		glutAddMenuEntry ("white", 4);
		glutAddMenuEntry ("magenta", 5);
		glutAddMenuEntry ("cyan", 6);
		glutAddMenuEntry ("yellow", 7);
		glutAddMenuEntry ("purple", 8);
		glutAddMenuEntry ("orange", 9);
		glutAddMenuEntry ("black", 10);
	squarecoloroutlinemenu = glutCreateMenu (process_menu); //square filled color menu
		glutAddMenuEntry ("red", 1);
		glutAddMenuEntry ("green", 2);
		glutAddMenuEntry ("blue", 3);
		glutAddMenuEntry ("white", 4);
		glutAddMenuEntry ("magenta", 5);
		glutAddMenuEntry ("cyan", 6);
		glutAddMenuEntry ("yellow", 7);
		glutAddMenuEntry ("purple", 8);
		glutAddMenuEntry ("orange", 9);
		glutAddMenuEntry ("black", 10);
	ellipsecolorfilledmenu = glutCreateMenu (process_menu); //ellipse filled color menu
		glutAddMenuEntry ("red", 1);
		glutAddMenuEntry ("green", 2);
		glutAddMenuEntry ("blue", 3);
		glutAddMenuEntry ("white", 4);
		glutAddMenuEntry ("magenta", 5);
		glutAddMenuEntry ("cyan", 6);
		glutAddMenuEntry ("yellow", 7);
		glutAddMenuEntry ("purple", 8);
		glutAddMenuEntry ("orange", 9);
		glutAddMenuEntry ("black", 10);
	ellipsecoloroutlinemenu = glutCreateMenu (process_menu); //ellipse outline color menu
		glutAddMenuEntry ("red", 1);
		glutAddMenuEntry ("green", 2);
		glutAddMenuEntry ("blue", 3);
		glutAddMenuEntry ("white", 4);
		glutAddMenuEntry ("magenta", 5);
		glutAddMenuEntry ("cyan", 6);
		glutAddMenuEntry ("yellow", 7);
		glutAddMenuEntry ("purple", 8);
		glutAddMenuEntry ("orange", 9);
		glutAddMenuEntry ("black", 10);
	linecolormenu = glutCreateMenu (process_menu); //line color menu
		glutAddMenuEntry ("red", 1);
		glutAddMenuEntry ("green", 2);
		glutAddMenuEntry ("blue", 3);
		glutAddMenuEntry ("white", 4);
		glutAddMenuEntry ("magenta", 5);
		glutAddMenuEntry ("cyan", 6);
		glutAddMenuEntry ("yellow", 7);
		glutAddMenuEntry ("purple", 8);
		glutAddMenuEntry ("orange", 9);
		glutAddMenuEntry ("black", 10);
	beziercolormenu = glutCreateMenu (process_menu); //bezier color menu
		glutAddMenuEntry ("red", 1);
		glutAddMenuEntry ("green", 2);
		glutAddMenuEntry ("blue", 3);
		glutAddMenuEntry ("white", 4);
		glutAddMenuEntry ("magenta", 5);
		glutAddMenuEntry ("cyan", 6);
		glutAddMenuEntry ("yellow", 7);
		glutAddMenuEntry ("purple", 8);
		glutAddMenuEntry ("orange", 9);
		glutAddMenuEntry ("black", 10);
	squaremenu = glutCreateMenu (process_menu); //square options
		glutAddSubMenu ("filled", squarecolorfilledmenu);
		glutAddSubMenu ("outline", squarecoloroutlinemenu);
	ellipsemenu = glutCreateMenu (process_menu); //square options
		glutAddSubMenu ("filled", ellipsecolorfilledmenu);
		glutAddSubMenu ("outline", ellipsecoloroutlinemenu);
	shapemenu = glutCreateMenu (process_menu); //start of shapes
		glutAddSubMenu ("squares", squaremenu);
		glutAddSubMenu ("ellipse", ellipsemenu);
		glutAddSubMenu ("line", linecolormenu);
		glutAddSubMenu ("bezier curve", beziercolormenu);
		glutAttachMenu (GLUT_LEFT_BUTTON); //open menu with right click
	//END MENU STUFF

    glutKeyboardFunc (keyboard);
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(motionmouse);
	glutPassiveMotionFunc(passivemouse);
	glutMainLoop ();
    return 0;
}
