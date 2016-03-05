//Name: Shrey Gupta
//Period: 4

#include <stdio.h>
#include <GL/glut.h>
#include "mpi.h"

int width = 640;
int height = 480;
double scaledWidth = 4.0;
double scaledHeight = 3.0;
double startWidth = -2.0;
double startHeight = -1.5;
int iterations = 100;

int rank;
int size;
int current;
MPI_Status status;
int tag = 0;

void displayfunc() {
	int x, y, j, k, result[height];
	glClear(GL_COLOR_BUFFER_BIT); //white
	current = 0;
	double nodes[size];
	
	for(j=1; j<size; j++) {
		double values[6] = {scaledWidth, scaledHeight, startWidth, startHeight, (double)iterations, (double)current};
		MPI_Send(values, 6, MPI_DOUBLE, j, tag, MPI_COMM_WORLD);
		nodes[j] = current;
		current += 1;
	}
	while(current < width) {
		MPI_Recv(result, height, MPI_INT, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);
		j = status.MPI_SOURCE;
		for(y=0; y<height; y++) {
			if(result[y] == iterations) {
				glColor3f(0.0, 0.0, 0.0); //black
			} else {
				glColor3f(2.0*result[y]/iterations, 3.0*result[y]/iterations, 2.0*result[y]/iterations);
			}

			glBegin(GL_POINTS);
			glVertex2f(nodes[j], y);
			glEnd();
		}
		
		double values[6] = {scaledWidth, scaledHeight, startWidth, startHeight, (double)iterations, (double)current};
		MPI_Send(values, 6, MPI_DOUBLE, j, tag, MPI_COMM_WORLD);
		nodes[j] = current;
		current += 1;
	}
	for(j=1; j<size; j++) {
		MPI_Recv(result, height, MPI_INT, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);
		j = status.MPI_SOURCE;
		for(y=0; y<height; y++) {
			if(result[y] == iterations) {
				glColor3f(0.0, 0.0, 0.0); //black
			} else {
				glColor3f(2.0*result[y]/iterations, 3.0*result[y]/iterations, 2.0*result[y]/iterations);
			}

			glBegin(GL_POINTS);
			glVertex2f(nodes[j], y);
			glEnd();
		}
	}
	
	glutSwapBuffers();
}

void reshapefunc(int wscr, int hscr) {
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 1.0*width, 0.0, 1.0*height);
	glMatrixMode(GL_MODELVIEW);
}

void mousefunc(int button, int state, int xscr, int yscr) {
	if(button == GLUT_LEFT_BUTTON) {
		if(state == GLUT_DOWN) {
			startWidth += (1.0*xscr/width)*scaledWidth - scaledWidth/4.0;
			startHeight += (1.0*(height-yscr)/height)*scaledHeight - scaledHeight/4.0;
			scaledWidth *= 0.5;
			scaledHeight *= 0.5;
		}
	} else if(button == GLUT_RIGHT_BUTTON) {
		if(state == GLUT_DOWN) {
			iterations *= 2;
		}
	}
	glutPostRedisplay();
}

int main(int argc, char* argv[]) {
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size); //same
	MPI_Comm_rank(MPI_COMM_WORLD, &rank); //different

	if(rank == 0) {
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
		glutInitWindowSize(width, height);
		glutInitWindowPosition(100, 50);
		glutCreateWindow("");
		glClearColor(1.0, 1.0, 1.0, 0.0);
		glShadeModel(GL_SMOOTH);

		glutDisplayFunc(displayfunc);
		glutReshapeFunc(reshapefunc);
		glutMouseFunc(mousefunc);

		glutMainLoop();
	} else {
		while(1) {
			int y, current, result[height];
			double values[6];
			MPI_Recv(values, 6, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status);
			scaledWidth = values[0];
			scaledHeight = values[1];
			startWidth = values[2];
			startHeight = values[3];
			iterations = (int)values[4];
			current = (int)values[5];
			
			if(current >= width) break;

			for(y=0; y<height; y++) {
				double scaledX = (1.0*current/width)*scaledWidth + startWidth;
				double scaledY = (1.0*y/height)*scaledHeight + startHeight;
				double a = scaledX;
				double b = scaledY;
				int i = 1;

				while(i < iterations && a*a + b*b <= 4.0) {
					double tempA = a*a - b*b + scaledX;
					double tempB = 2.0*a*b + scaledY;
					a = tempA;
					b = tempB;
					i += 1;
				}
				
				result[y] = i;
			}
			
			MPI_Send(result, height, MPI_INT, 0, tag, MPI_COMM_WORLD);
		}
	}
	
	//exit status
	MPI_Finalize();
	return 0;
}
