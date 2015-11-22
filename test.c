#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {

/*	double datadensity1[102401], datadensity2[102401], datadensity3[102401], datadensity4[102401], datadensity5[102401];*/

	double datadensity[102401][5];

	FILE *fp = fopen("DataDensity", "rt");

	if (fp == NULL) {
		fprintf(stderr, "Can't open DataDensity file!\n");
		return 1;
	}
	else {

		int i = 0;

		while(fscanf(fp, "%lf %lf %lf %lf %lf", &datadensity[i][3], &datadensity[i][4], &datadensity[i][0], &datadensity[i][1], &datadensity[i][2]) > 0) {
			i++;
		}
	}

	fclose(fp);

	double maxValue = 0;
	double minValue = 99999;

	for (int i=1; i<102401; i++) {
		for (int j=0; j<5; j++) {
			if (datadensity[i][j] > maxValue) {
				maxValue = datadensity[i][j];
			}
			if (datadensity[i][j] < minValue) {
				minValue = datadensity[i][j];
			}
		}
	}

	printf("%lf, %lf\n", maxValue, minValue);

/*	printf("%lf\n", datadensity[102400][0]);*/

/*	checkMaxMinInArray(datadensity1);*/

	

	return 0;

}

/*double checkMaxMinInArray(double[] array) {

	printf("%lf\n", array[1]);

	return array[1];

}*/

