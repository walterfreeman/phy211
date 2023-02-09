/**********************************************
anim.c, simple animation routine using OpenGL/GLUT
Written by Walter Freeman

Compile (on Linux) with
gcc anim.c -lGL -lGLU -lglut -lm

Compile (on Mac) with
gcc anim.c -framework GLUT -framework OpenGL -framework Cocoa
**********************************************/

#include <GL/glut.h>      // for Linux
#include <GL/glext.h>      // for Linux
//#include <GLUT/glut.h>       // for Macs 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#define window_size 1000
#define N 200

short int spin[N][N];
double r[N][N], g[N][N], b[N][N];
int m;
int upd_per_draw=200000;
double T=1;
int maxsz=1;
int rate=0, n=0,accept=0;
double gc=0.1;
void renderBitmapString(
		float x, 
		float y, 
		float z, 
		void *font, 
		char *string) {  
  char *c;
  glRasterPos3f(x, y,z);
  for (c=string; *c != '\0'; c++) {
    glutBitmapCharacter(font, *c);
  }
}

double orderofmagnitude(double v)
{
  if (v == 0.0) return(0.0);
  if (v >= 1.0 && v <= 2.0) return(1.0);
  if (v > 2.0 && v<=5.0) return (2.0);
  if (v > 5.0 && v<=10.0) return (5.0);
  if (v < 1.0) return (orderofmagnitude(v*10.0)*0.1);
  if (v > 10.0) return (orderofmagnitude(v*0.1)*10.0);

}
void lin (double x1,double y1,double x2,double y2)
{

     glVertex3f(x1,y1,0);
     glVertex3f(x2,y2,0);

}
double roundnearest(double v,double a)
{
  return(a*floor(v/a+.5));
}

double roundup(double v,double a)
{
  return(a*ceil(v/a+.5));
}
double rounddown(double v,double a)
{
  return(a*floor(v/a+.5));
}


int dE(int i, int j, int sx, int sy)
{
  int delE=0;
  int x,y;
  for (x=i;x<i+sx;x++)
  {
    delE += spin[x%N][j]*spin[x%N][(j-1+N)%N];
    delE += spin[x%N][(j+sy-1)%N]*spin[x%N][(j+sy)%N];
  }
  for (y=j;y<j+sy;y++)
  {
    delE += spin[i][y%N]*spin[(i-1+N)%N][y%N];
    delE += spin[(i+sx-1)%N][y%N]*spin[(i+sx)%N][y%N];
  }
  return delE;
}


void add_ac(int x, int y, int ac[N])
{
  int i;
  for (i=0;i<N/2;i++)
  {
    ac[i]+=spin[x][y]*spin[(x+i)%N][y];
    ac[i]+=spin[x][y]*spin[x][(y+i)%N];
    ac[i]+=spin[x][y]*spin[(x-i+N)%N][y];
    ac[i]+=spin[x][y]*spin[x][(y-i+N)%N];
  }
}

void measure_ac(int ac[N], int d)
{
  int x,y;
  for (x=0;x<N/2;x++) ac[x]=0;
  for (x=0;x<N;x+=N/d)
  {
    for (y=0;y<N;y+=N/d)
    {
      add_ac(x,y,ac);
    }
  }
}

void idle(void)
{
  static int i;
  static int j;
  static int k,sx,sy;
  static int delE;
  static int ii,jj;
  static double x,y,x2,y2;
  static double M;
  static int V;
  static char text[80];
  M=0;
  static int nc=0;
  static double mfrac;
  static int mbuf[1000];
  static int ac[N];
  nc++;
  for (i=0;i<N;i++)
  {
    for (j=0;j<N;j++)
    {
      M+=spin[i][j];
      if (spin[i][j]==-1) {r[i][j]=0; g[i][j]*=0.85; b[i][j]=0.7;}
      if (spin[i][j]==1) {r[i][j]=0.7; g[i][j]*=0.85; b[i][j]=0;}
      
      glColor3f(r[i][j],g[i][j],b[i][j]);
      glBegin(GL_QUADS);
      
      x=(((double)i/N)-0.5)*1.8;
      x2=(((double)(i+1)/N)-0.5)*1.8;
      y=(((double)j/N)-0.415)*1.8;
      y2=(((double)(j+1)/N)-0.415)*1.8;
      glVertex2f(x,y);
      glVertex2f(x2,y);
      glVertex2f(x2,y2);
      glVertex2f(x,y2);

      glEnd();
    }
  }

  measure_ac(ac,N);

  glColor3f(0.8,0.8,0.8);
  glBegin(GL_LINE_STRIP);
  for (i=0;i<N/2;i++)
  {
    x=(((double)(i*2)/N)-0.5)*1.8;
    y=-1+(double)ac[i]/ac[0]*0.25;
    glVertex2f(x,y);
  }
  glEnd();

  mbuf[nc%1000]=M;
  glBegin(GL_LINES);
  glColor3f(0.5,0.5,0.5);
  glVertex2f(-1,-.75);
  glVertex2f(1,-.75);
  glVertex2f(-1,-1);
  glVertex2f(1,-1);
  glEnd();
  glBegin(GL_LINE_STRIP);
  for (i=0;i<1000;i++)
  {
    j=((i+nc)+1000)%1000;
    mfrac=(double)mbuf[j]/N/N;
    if (mfrac < 0) glColor3f(1,0,0); else glColor3f(0,0,1);
    x=(i-500)/500.0;
    y=-1+pow(fabs(mfrac),0.25)*0.25;
    glVertex2f(x,y);
  }
  glEnd();

  for (k=0;k<upd_per_draw;k++)
  {
    i=lrand48()%N;
    j=lrand48()%N;
    sx=lrand48()%maxsz+1;
    sy=lrand48()%maxsz+1;
    V=sx*sy;

//    printf("Suggest flip %2d %2d (%d):",i,j,spin[i][j]);
    delE=dE(i,j,sx,sy);
//    printf(" dE = %d: ",dE);
    if (drand48() < exp(-delE*T)) {rate+=V; accept++; for (ii=i;ii<i+sx;ii++) for (jj=j;jj<j+sy;jj++) {spin[ii%N][jj%N]*=-1; g[ii%N][jj%N]+=gc/((double)rate/(n+1)); if (g[ii%N][jj%N]>1) g[ii%N][jj%N]=1;}}
    n++;
  }
  glColor3f(1,1,1);
  sprintf(text,"magnetization %.4e\tflip rate %.3e\taccept rate %.3e\tsteps %d\n",(double)M/(N*N),(double)rate/n,(double)accept/n,n);
  renderBitmapString(-0.5,-0.9,0,GLUT_BITMAP_TIMES_ROMAN_10,text);
  glutSwapBuffers();
  glClear(GL_COLOR_BUFFER_BIT);
}



void disp(void)
{
}

void keyb(unsigned char key, int x, int y)     
{
  if (key == 'Q') {exit(0);}
  if (key == 'a') {rate=n=0; maxsz+=1; printf("New max size %d\n",maxsz); accept=0;}
  if (key == 'z') {rate=n=0; maxsz-=1; if (maxsz==0) maxsz=1; printf("New max size %d\n",maxsz);accept=0; }
  if (key == '=') {rate=n=0; T*=1.01; printf("New temp %e\n",1/T);accept=0;}
  if (key == '-') {rate=n=0; T/=1.01; printf("New temp %e\n",1/T);accept=0;}
  if (key == 's') {gc*=1.5;}
  if (key == 'x') {gc/=1.5;}
}    

int main(int argc, char **argv)
{
  //INITIALIZATION
  glutInit(&argc, argv);
  
  //set rgba and double buffering  
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE |  GLUT_MULTISAMPLE);

  //set window size and position and title
  glutInitWindowSize(1000,1000);
  glutInitWindowPosition(100,100);
  glutCreateWindow("Animation");

  //SET CALLBACKS
  glutDisplayFunc(disp);
  glutKeyboardFunc(keyb);
  glutIdleFunc(idle);

  //DO OPENGL INIT
  glEnable(GL_BLEND);
  glEnable(GL_MULTISAMPLE);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
  glClearColor(0.0, 0.0, 0.0, 1.0);
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  
  for (int i=0; i<N;i++) {
    for (int j=0; j<N;j++) {
      if (drand48()<.5) spin[i][j]=-1; else spin[i][j]=1;
    }
  }


  glutMainLoop();
}
