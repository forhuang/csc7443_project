//////////////////////////////////////////////////////////////////////////////////////////////
// Project 4: Raycasting Volume Rendering
// Authors: Xinbo Huang and Neha Jose
// version: 0.1.0
// Description: This program reads the data from DataDensity file, processes the data through 
// raycasting method, and findlly output the image through OpenGL in linux environment. The
// image size can be modified by the parameter, IMGSIZE, and the initial opacity can be
// modified by the parameter, opacity.
// Compile Command: make
// Run Command: ./run
//////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <math.h>

#include <GL/glut.h>

#define SIZE 80

// The image size is modified from here
#define IMGSIZE 512

using namespace std;

double dataDensity[SIZE][SIZE][SIZE];
double maxValue = 0, minValue = 999;

// The initial opacity is modified from here
double opacity = 0.1;

double dataColor[SIZE][SIZE][4];
/*double imgColor[IMGSIZE][IMGSIZE][4];*/

double imgColor[IMGSIZE*IMGSIZE*4];

unsigned int window_width = 512, window_height = 512;

//Read DataDensity file 
void readFile() {

	ifstream myfile("DataDensity");
	
	if (myfile.is_open()) {
		int i = 0, j = 0, k = 0, len_line = 0;
		string line;
		while (getline(myfile, line)) {
			double d;
			istringstream iss(line);
			if (len_line > 2) {
				while (iss >> d) {
					if (d > maxValue) maxValue = d;
					if (d < minValue) minValue = d;
					dataDensity[i][j][k] = d;
					if (k == SIZE-1) {
						k = 0;
						if (j == SIZE-1) {
							j = 0;
							i += 1;
						}
						else j += 1;
					}
					else k += 1;
				}	
			}
			len_line += 1;
		}
		myfile.close();
	}
	else cout<<"Unable to open file";
}

// Transfer color and composite color and opacity
void tranferAndComposite() {
	for (int i=0; i<SIZE; i++) {
		for (int j=0; j<SIZE; j++) {
			double compositeOpacity, compositeBlue, compositeGreen;
			for (int k=0; k<SIZE; k++) {
				double dataBlue = (dataDensity[i][j][k] - minValue) / (maxValue - minValue);
				double dataGreen = 1 - dataBlue;
				if (k == 0) {
					compositeOpacity = opacity;
					compositeBlue = dataBlue;
					compositeGreen = dataGreen;
				}
				compositeOpacity = opacity + compositeOpacity * (1 - opacity);
				compositeBlue = dataBlue + compositeBlue * (1 - compositeOpacity);
				compositeGreen = dataGreen + compositeGreen * (1 - compositeOpacity);
			}
			dataColor[i][j][0] = 0;
			dataColor[i][j][1] = compositeGreen;
			dataColor[i][j][2] = compositeBlue;
			dataColor[i][j][3] = compositeOpacity;
		}
	}
}

//Resample data to create image
void resample() {

	double delta = double(SIZE)/IMGSIZE;

	for (int i=0; i<IMGSIZE; i++) {

		int a1 = ceil(i * delta);
		double a2 = a1 - i * delta;
		
		int b1 = floor((i+1) * delta);
		double b2 = (i+1) * delta - b1;
		
		for (int j=0; j<IMGSIZE; j++) {

			int c1 = ceil(j * delta);
			double c2 = c1 - j * delta;
			
			int d1 = floor((j+1) * delta);
			double d2 = (j+1) * delta - d1;

			double dataGreen, dataBlue, dataOpacity;

			if (a1 > b1 && c1 > d1) {
				dataGreen = pow(delta,2)*dataColor[b1][d1][1];
				dataBlue = pow(delta,2)*dataColor[b1][d1][2];
				dataOpacity = pow(delta,2)*dataColor[b1][d1][3];
			}
			else if (a1 > b1) {
				dataGreen = delta*c2*dataColor[b1][c1-1][1] + delta*d2*dataColor[b1][d1][1];
				dataBlue = delta*c2*dataColor[b1][c1-1][2] + delta*d2*dataColor[b1][d1][2];
				dataOpacity = delta*c2*dataColor[b1][c1-1][3] + delta*d2*dataColor[b1][d1][3];
			}
			else if (c1 > d1) {
				dataGreen = a2*delta*dataColor[a1-1][d1][1] + b2*delta*dataColor[b1][d1][1];
				dataBlue = a2*delta*dataColor[a1-1][d1][2] + b2*delta*dataColor[b1][d1][2];
				dataOpacity = a2*delta*dataColor[a1-1][d1][3] + b2*delta*dataColor[b1][d1][3];
			}
			else {
				dataGreen = a2*c2*dataColor[a1-1][c1-1][1] + b2*c2*dataColor[b1][c1-1][1] + a2*d2*dataColor[a1-1][d1][1] + b2*d2*dataColor[b1][d1][1];
				dataBlue = a2*c2*dataColor[a1-1][c1-1][2] + b2*c2*dataColor[b1][c1-1][2] + a2*d2*dataColor[a1-1][d1][2] + b2*d2*dataColor[b1][d1][2];
				dataOpacity = a2*c2*dataColor[a1-1][c1-1][3] + b2*c2*dataColor[b1][c1-1][3] + a2*d2*dataColor[a1-1][d1][3] + b2*d2*dataColor[b1][d1][3];
			}

			if (a1 < b1) {
				for (int k=0; k<b1-a1; k++) {
					dataGreen = dataGreen + c2*dataColor[a1+k][c1-1][1] + d2*dataColor[a1+k][d1][1];
					dataBlue = dataBlue + c2*dataColor[a1+k][c1-1][2] + d2*dataColor[a1+k][d1][2];
					dataOpacity = dataOpacity + c2*dataColor[a1+k][c1-1][3] + d2*dataColor[a1+k][d1][3];
				}
			}

			if (c1 < d1) {
				for (int k=0; k<d1-c1; k++) {
					dataGreen = dataGreen + a2*dataColor[a1-1][c1+k][1] + b2*dataColor[b1][c1+k][1];
					dataBlue = dataBlue + a2*dataColor[a1-1][c1+k][2] + b2*dataColor[b1][c1+k][2];
					dataOpacity = dataOpacity + a2*dataColor[a1-1][c1+k][3] + b2*dataColor[b1][c1+k][3];
				}
			}

			if (a1 < b1 && c1 < d1) {
				for (int p=0; p<b1-a1; p++) {
					for (int q=0; q<d1-c1; q++) {
						dataGreen = dataGreen + dataColor[a1+p][c1+q][1];
						dataBlue = dataBlue + dataColor[a1+p][c1+q][2];
						dataOpacity = dataOpacity + dataColor[a1+p][c1+q][3];
					}
				}
			}

			int imgOrder = i*IMGSIZE*4+j*4;

			imgColor[imgOrder] = 0;
			imgColor[imgOrder+1] = dataGreen/pow(delta,2);
			imgColor[imgOrder+2] = dataBlue/pow(delta,2);
			imgColor[imgOrder+3] = dataOpacity/pow(delta,2);

/*			imgColor[i][j][0] = 0;
			imgColor[i][j][1] = dataGreen/pow(delta,2);
			imgColor[i][j][2] = dataBlue/pow(delta,2);
			imgColor[i][j][3] = dataOpacity/pow(delta,2);*/

/*			cout << i << ", " << j << ",  " << imgColor[i][j][1] << ", " << imgColor[i][j][2] << ", " << imgColor[i][j][3] << endl;*/

		}

	}
}

// Display Image
void display() {

	glClear(GL_COLOR_BUFFER_BIT);

	glDrawPixels(IMGSIZE,IMGSIZE,GL_RGBA,GL_FLOAT,imgColor);

	glutSwapBuffers();
}

int main(int argc, char** argv) {

	readFile();
	tranferAndComposite();
	resample();

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(IMGSIZE, IMGSIZE);
	glutCreateWindow("RayCasting");

	glutDisplayFunc(display);

	glClearColor(0.0, 0.0, 0.0, 1.0);

	glutMainLoop();

	return 0;

}