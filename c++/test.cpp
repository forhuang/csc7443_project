#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <GL/glut.h>

#define size 80
#define SIZE_X 128
#define SIZE_Y 128

using namespace std;

double dataDensity[size][size][size];
double maxValue = 0, minValue = 999;

double v[] = {0.0, 0.0, 1.5};
double xmin = -1.0, xmax = 1.0;
double ymin = -1.0, ymax = 1.0;
float image[SIZE_X][SIZE_Y][3];

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
					cout << i << ", "<< j << ", " << k << ", " << d << endl;
					if (d > maxValue) maxValue = d;
					if (d < minValue) minValue = d;
					dataDensity[i][j][k] = d;
					if (k == size-1) {
						k = 0;
						if (j == size-1) {
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

	cout << "Max Value: " << maxValue << endl; 
	cout << "Min Value: " << minValue << endl;
}




int main() {

	readFile();

	return 0;

}


