/*# include <windows.h>*/
# include <GL/glut.h>
# include <stdio.h>
# include <math.h>
# define SIZE_X 512
# define SIZE_Y 512

void init(void);
void display(void);
void keyboard(unsigned char c, int x, int y);
void traceIt(void);
void write2file(void);
void normalize(double v[3]); 
void computeColor(double *image, double contact[], double dotprod, double specular);

double v[] = {0.0, 0.0, 1.5};
double r = 0.75, sindex = 500.0;
double xmin = -1.0, xmax = 1.0;
double ymin = -1.0, ymax = 1.0;
float image[SIZE_X][SIZE_Y][3];
//double l[] = {0.57735, -0.57735, 0.57735};
double lc[3] = {0.9, 0.9, 0.9};
double la[3] = {0.3, 0.3, 0.3};
double ma[4][3] = {
       {0.1, 0.5, 0.1},
       {0.5, 0.1, 0.1},
       {0.5, 0.5, 0.1},
       {0.1, 0.1, 0.5}
};
double ms[4][3] = {
       {0.5, 0.5, 0.5},
       {0.5, 0.5, 0.5},
       {0.5, 0.5, 0.5},
       {0.5, 0.5, 0.5}
};

double l[3] = {0.0, 0.0, 1.0};
/*
double l[2][3] = {
	{0.0, 0.0, 1.0}
	{1.0, 1.0, 0.0}
};
*/
//int nl = 2;
static GLuint texName;
int main(int argc, char *argv[])
{
    traceIt();
//    write2file();
   glutInit(&argc,argv);
   glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
   glutInitWindowSize(SIZE_X, SIZE_Y);
   glutInitWindowPosition(100,0);
   glutCreateWindow("Ray Traced Ellipsoid");
   init();
   glutDisplayFunc(display);
   glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0;
}
void traceIt(void) {
     double dx, dy;
     dx = (xmax - xmin)/SIZE_X;
     dy = (ymax - ymin)/SIZE_Y;
     double delx, dely, delz;
     double a, b, c, disc, v_n[3];
	  double ax = 0.8, bx = 0.5, cx = 0.8;
//     c = v[0]*v[0] + v[1]*v[1] + v[2]*v[2] - r*r;
     c = (v[0]*v[0])/(ax*ax) + (v[1]*v[1])/(bx*bx) + (v[2]*v[2])/(cx*cx) - 1;
     delz = -v[2];
     double t, t1, t2, dotprod, x, y, viewprod;
     double contact[3], norm, h[3], specular;     
     int i, j, k;
     for(x = xmin, i = 0; x < xmax; x += dx, ++i) {
           delx = x;
           for(y = ymin, j = 0 ; y < ymax; y += dy, ++j) {
                 dely = y;
                 v_n[0] = -delx;
                 v_n[1] = -dely;
                 v_n[2] = -delz;
                 a = (delx*delx)/(ax*ax) + (dely*dely)/(bx*bx) + (delz*delz)/(cx*cx);
                 b = 2.0 * ((delx * v[0])/(ax*ax) + (dely * v[1])/(bx*bx) + (delz * v[2])/(cx*cx));
                 disc = b * b - 4.0 * a * c;
/*                printf("a: %lf b: %lf c: %lf disc: %lf\n", a, b, c, disc);*/
                 if(disc < 0.0) {
                         image[i][j][0] = 0.5;
                         image[i][j][1] = 0.5;
                         image[i][j][2] = 0.5;
                         continue;
                 }
                 else {
                      t1 = (-b + sqrt(disc))/(2*a);
                      t2 = (-b - sqrt(disc))/(2*a);
                      if(t1 <= t2) t = t1;
                      else t = t2;
                 }
                 contact[0] = (v[0] + t * delx)/(ax*ax);
                 contact[1] = (v[1] + t * dely)/(bx*bx);
                 contact[2] = (v[2] + t * delz)/(cx*cx);
                 normalize(contact);
                 normalize(l);
                 dotprod = 0.0;
                 normalize(v_n);
                 for(k = 0; k < 3; ++k)
                       dotprod += v_n[k]*contact[k];
                 for(k = 0; k < 3; ++k)
                       h[k] = l[k] + v_n[k];
/*                printf("(%lf, %lf, %lf)\n", h[0], h[1], h[2]);*/
                 normalize(h);
/*                printf("(%lf, %lf, %lf)\n", h[0], h[1], h[2]);*/
                 specular = 0.0;
                 for(k = 0; k < 3; ++k)
                       specular += contact[k]*h[k];
                 specular = pow(specular, sindex);
                printf("norm: %lf specular: %lf\n", norm, specular);
                 if(dotprod >= 0.0){
                            if(dotprod > 0.9) dotprod = pow(dotprod, 15);
                             if(contact[1] > 0.0) {
                                if(contact[0] > 0.0) {
                                   image[i][j][0] = la[0]*ma[0][0] + dotprod * ma[0][0]*lc[0] + specular * ms[0][0]*lc[0];
                                   image[i][j][1] = la[1]*ma[0][1] + dotprod * ma[0][1]*lc[1] + specular * ms[0][1]*lc[1];
                                   image[i][j][2] = la[2]*ma[0][2] + dotprod * ma[0][2]*lc[2] + specular * ms[0][2]*lc[2];
                                }
                                else if(contact[0] <= 0.0) {
                                   image[i][j][0] = la[0]*ma[1][0] + dotprod * ma[1][0]*lc[0] + specular * ms[1][0]*lc[0];
                                   image[i][j][1] = la[1]*ma[1][1] + dotprod * ma[1][1]*lc[1] + specular * ms[1][1]*lc[1];
                                   image[i][j][2] = la[2]*ma[1][2] + dotprod * ma[1][2]*lc[2] + specular * ms[1][2]*lc[2];
                                }
                             }
                             else if(contact[1] <= 0.0) {
                                if(contact[0] > 0.0) {
                                   image[i][j][0] = la[0]*ma[2][0] + dotprod * ma[2][0]*lc[0] + specular * ms[2][0]*lc[0];
                                   image[i][j][1] = la[1]*ma[2][1] + dotprod * ma[2][1]*lc[1] + specular * ms[2][1]*lc[1];
                                   image[i][j][2] = la[2]*ma[2][2] + dotprod * ma[2][2]*lc[2] + specular * ms[2][2]*lc[2];
                                }
                                else if(contact[0] <= 0.0) {
                                   image[i][j][0] = la[0]*ma[3][0] + dotprod * ma[3][0]*lc[0] + specular * ms[3][0]*lc[0];
                                   image[i][j][1] = la[1]*ma[3][1] + dotprod * ma[3][1]*lc[1] + specular * ms[3][1]*lc[1];
                                   image[i][j][2] = la[2]*ma[3][2] + dotprod * ma[3][2]*lc[2] + specular * ms[3][2]*lc[2];
                                }
                            }

//                            computeColor(&image[i][j][0], contact, dotprod, specular);
                 }
           }
     }
}

void write2file(void) {
     FILE *img;
     img = fopen("image.out", "w");
     int i, j;
      for(i = 0; i < SIZE_X; ++i)
           for(j = 0; j < SIZE_Y; ++j)
                 fprintf(img, "%lf %lf %lf\n", image[i][j][0], image[i][j][1], image[i][j][2]);
     fclose(img);
}

void init(void)
{
   glGenTextures(1, &texName);
   glBindTexture(GL_TEXTURE_2D, texName);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SIZE_X, SIZE_Y, 0, GL_RGB, GL_FLOAT, image);
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
   glClearColor(0.0f ,0.0f ,0.0f ,0.0f);
   glColor3f(1.0f,0.0f,0.0f);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   //gluPerspective(45.0f,(GLfloat)250/(GLfloat)250,0.1f,100.0f);
   glOrtho(0.0, 1.0, 0, 1.0, 0.0, 1.0);
}

void display(void)
{
   glClear(GL_COLOR_BUFFER_BIT);
   glEnable(GL_TEXTURE_2D);
   glBegin(GL_QUADS);
   	glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, 0.0);
   	glTexCoord2f(1.0, 0.0); glVertex3f(1.0, 0.0, 0.0);
   	glTexCoord2f(1.0, 1.0); glVertex3f(1.0, 1.0, 0.0);
   	glTexCoord2f(0.0, 1.0); glVertex3f(0.0, 1.0, 0.0);
   glEnd();
   glDisable(GL_TEXTURE_2D);
/*
   GLfloat dx, dy, x, y;

   dx = 1.0/SIZE_X; dy = 1.0/SIZE_Y;
   glBegin(GL_POINTS);
   x = 0.0; y = 0.0;
   for(int i = 0; i < SIZE_X; ++i) {
        for(int j = 0; j < SIZE_Y; ++j)  {
            glColor3f(image[i][j][0], image[i][j][1], image[i][j][2]);
            glVertex3f(x, y, 0.0);
            y += dy;
      }
      x += dx;
      y = 0.0;
   }
*/
   glutSwapBuffers();
}

void keyboard(unsigned char c, int x, int y) {
   switch(c) {
      case 27:
         exit(0);
   }
}

void normalize(double v[3]) {
     double norm;
     norm = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
     for(int i = 0; i < 3; ++i)
             v[i] /= norm;     
}

void computeColor(double *image, double contact[], double dotprod, double specular) {
//     if(dotprod > 0.8) dotprod = pow(dotprod, 9);
     
     image[0] = la[0]*contact[0]*contact[0] + dotprod * contact[0]*contact[0]*lc[0] + specular * cos(contact[0])*lc[0];
     image[1] = la[1]*contact[1]*contact[1] + dotprod * contact[1]*contact[1] * lc[1] + specular * cos(contact[1])*lc[1];
     image[2] = la[2]*contact[2]*contact[2] + dotprod * contact[2]*contact[2]*lc[2] + specular * cos(contact[2])*lc[2];
//     normalize(image);
}
