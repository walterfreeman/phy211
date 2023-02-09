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
#define window_size 720
#define N 720
#define DN 720

double smag[800];
int sct[800];
double smag2[800];
int tbin;
int tdir=1;
int warmup=-1000;

double tmin=1.1,tmax=1.5,tstep=0.01;

int record=0;

struct list_el {
     int x;
     int y;
     struct list_el * next;
     int n;
};

typedef struct list_el item;

void add(item **list, int x, int y) 
{ 
  item *curr; 
  curr=(item *)malloc(sizeof(item)); 
  curr->x = x; 
  curr->y = y; 
  curr->next = *list; 
  *list=curr; 
} 

void del (item **list, int x, int y) 
{
  item *curr; 
  curr=*list; 
//  printf("Trying to delete %d,%d from a list whose head is %d...\n",x,y,curr); 
  if (curr == NULL) {return;} 
  if (curr->x == x && curr->y == y) {*list=curr->next; free(curr); return;} 
  else 
  {
//    printf("About to start while loop\n");
    while (curr -> next != NULL) 
    { 
//      printf("Checking %d,%d for deletion to see if it matches %d,%d\n",curr->next->x,curr->next->y,x,y);
      if (curr->next->x == x && curr->next->y == y) 
      { 
        free(curr->next); curr->next=curr->next->next; 
        return; 
      } 
      curr=curr->next; 
    }
  }
//  printf("WARNING: Asked to delete %d,%d, but it was not present\n",x,y);
}


double r[DN][DN],g[DN][DN],b[DN][DN];
short int spin[N][N];
int m;
int upd_per_draw=25;
int therm=200;
int max=1000;
double T=tmax;
int maxsz=1;
int rate=0, n=-5000,accept=0;
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


void init_color(int x, int y)
{
  int i,j;
  i=x*DN/N;
  j=y*DN/N;
  if (spin[x][y] == 1) 
  {
  r[i][j] += ((float)DN/N)*((float)DN/N);
  }
  else
  {
  b[i][j] += ((float)DN/N)*((float)DN/N);
  }
}
void update_color(int x, int y)
{
  int i,j;
  i=x*DN/N;
  j=y*DN/N;
  r[i][j] += ((float)DN/N)*((float)DN/N) *spin[x][y];
  b[i][j] -= ((float)DN/N)*((float)DN/N) *spin[x][y];
}

int build_cluster(int x, int y, short int spin[N][N], short int mark[N][N])
{
//  printf("Starting to build cluster\n");
  item* list;
  list=NULL;
  int n=1;
  int nold;
  add(&list,x,y);
  spin[x][y]*=-1;
  update_color(x,y);
  double P;
  while (list != NULL)
  {
    x=list->x;
    y=list->y;
//    printf("\t%d elements remain (head pointer: %d -> %d)\n",n,list,list->next);
//    printf("\t\tExamining %d,%d\n",x,y);
    del(&list,x,y);
//    n--;
    nold=n;
    mark[x][y]=1;
    P = 1-exp(1/T*spin[(x+1)%N][y]*spin[x][y]);  
    if (P > drand48() && mark[(x+1)%N][y]==0)
    {      
      spin[(x+1)%N][y]*=-1;
      update_color((x+1)%N,y);
      add(&list,(x+1)%N,y); 
      n++;
    }
    P = 1-exp(1/T*spin[(x-1+N)%N][y]*spin[x][y]);
    if (P > drand48() && mark[(x-1+N)%N][y]==0)
    { 
      spin[(x-1+N)%N][y]*=-1;
      add(&list,(x-1+N)%N,y); 
      update_color((x-1+N)%N,y);
      n++;}
    P = 1-exp(1/T*spin[x][(y+1)%N]*spin[x][y]);
    if (P > drand48() && mark[x][(y+1)%N]==0)
    { 
      spin[x][(y+1)%N]*=-1;
      add(&list,x,(y+1)%N);
      n++;
      update_color(x,(y+1)%N);
    }  
    P = 1-exp(1/T*spin[x][(y-1+N)%N]*spin[x][y]);
    if (P > drand48() && mark[x][(y-1+N)%N]==0)
    { 
      spin[x][(y-1+N)%N]*=-1;
      add(&list,x,(y-1+N)%N);
      update_color(x,(y-1+N)%N); 
      n++;
    }
//    if (nold==n) g[x][y]=1;
  }
// printf("Done building cluster, %d sites\n",n);
  return n;
}




int activate(int x, int y, short int spin[N][N], short int mark[N][N])
{
  int nn=1;
  x=(x+N)%N;
  y=(y+N)%N;
  printf("activate %d,%d\n",x,y);
  g[x][y]+=gc;
  double P;
  spin[x][y]*=-1;
  mark[x][y]=1;
  P = 1-exp(T*spin[(x+1)%N][y]*spin[x][y]);
  printf("Check right\n");
  if (P > drand48() && mark[(x+1)%N][y]==0)
    nn+=activate(x+1,y, spin, mark);
  P = 1-exp(T*spin[(x-1+N)%N][y]*spin[x][y]);
  printf("Check left\n");
  if (P > drand48() && mark[(x-1+N)%N][y]==0)
    nn+=activate(x-1,y, spin, mark);
  P = 1-exp(T*spin[x][(y+1)%N]*spin[x][y]);
  printf("Check up\n");
  if (P > drand48() && mark[x][(y+1)%N]==0)
    nn+=activate(x,y+1, spin, mark);
  P = 1-exp(T*spin[x][(y-1+N)%N]*spin[x][y]);
  printf("Check down\n");
  if (P > drand48() && mark[x][(y-1+N)%N]==0)
    nn+=activate(x,y-1, spin, mark);
  printf("... done\n");
  return nn;
  
}

int cluster(short int spin[N][N])
{
  int nn;
  static short int mark[N][N];
  int i,j;
  for (i=0;i<N;i++) for (j=0;j<N;j++) mark[i][j]=0;
  int x,y;
  x=lrand48()%N;  y=lrand48()%N;

  nn=build_cluster(x,y,spin,mark);
//  printf("Cluster done, size %d\n",nn);
  return nn;
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
  int accmax=N*N*40;
  int accrec=N*N*4;
  nc++;
  for (i=0;i<DN;i++)
  {
    for (j=0;j<DN;j++)
    {
      M+=spin[i][j]*N*N/(float)DN/DN;
      
      glColor3f(r[i][j],g[i][j],b[i][j]); g[i][j]=0;
      glBegin(GL_QUADS);
      
      x=(((double)i/DN)-0.5)*1.8;
      x2=(((double)(i+1)/DN)-0.5)*1.8;
      y=(((double)j/DN)-0.415)*1.8;
      y2=(((double)(j+1)/DN)-0.415)*1.8;
      glVertex2f(x,y);
      glVertex2f(x2,y);
      glVertex2f(x2,y2);
      glVertex2f(x,y2);

      glEnd();
    }
  }

  /*
//printf("Starting AC measurement\n");
  measure_ac(ac,16);
//printf("Done with AC measurement\n");

  glColor3f(0.8,0.8,0.8);
  glBegin(GL_LINE_STRIP);
  for (i=0;i<N/2;i++)
  {
    x=(((double)(i*2)/N)-0.5)*1.8;
    y=-1+(double)ac[i]/ac[0]*0.25;
    glVertex2f(x,y);
  }
  glEnd();
*/
  mbuf[nc%1000]=M;
  glBegin(GL_LINES);
  glColor3f(0.5,0.5,0.5);
  glVertex2f(-1,-.75);
  glVertex2f(1,-.75);
  glVertex2f(-1,-1);
  glVertex2f(1,-1);
  glEnd();
  glBegin(GL_LINE_STRIP);
  static int acc=0,rec=0;
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
    acc+=cluster(spin);
    n++;
    if (acc>accmax) break;
  }
  glColor3f(1,1,1);
  sprintf(text,"T=%.3e\tmagnetization %.4e\tflip rate %.3e\taccept rate %.3e\tsteps %d\tflips %d/%d (%d)\tlogged %d\n",T,(double)M/(N*N),(double)rate/n,(double)accept/n,n,acc,accmax,accrec,rec);
  if (record)
  {
    rec++;
    smag[tbin]+=fabs((double)M/(N*N));
    sct[tbin]++;
  } 
  glColor3f(0.8,0.8,0);
  glBegin(GL_LINE_STRIP);
  for (i=0;i<(tmax-tmin)/tstep;i++)
  {
//    printf("!x = %e for bin %d\n",-1+2*i*tstep/(tmax-tmin),i);
    if (!sct[i]) continue;
      glVertex2f(-1+2*i*tstep/(tmax-tmin),-1+0.2*smag[i]/sct[i]);
  }
  glEnd();
  glColor3f(0.3,0.3,0.6);
  glBegin(GL_LINES);
    glVertex2f(-1+2*tbin*tstep/(tmax-tmin),-1);
    glVertex2f(-1+2*tbin*tstep/(tmax-tmin),-0.8);
  glEnd();
  if (record) glColor3f(1,1,1); else glColor3f(0.4,0.4,0.4);
  renderBitmapString(-0.5,-0.9,0,GLUT_BITMAP_TIMES_ROMAN_10,text);
  glutSwapBuffers();
  glClear(GL_COLOR_BUFFER_BIT);
  if (n>therm || acc>accrec) record=1;
  if (n>max)
  {
    rec=acc=rate=n=0; T-=tstep*tdir; printf("New temp %e\n",T);accept=0;record=0;
    if (T<tmin || T>tmax) tdir*=-1;
    tbin=(T-tmin)/tstep;
  }
}



void disp(void)
{
}

void keyb(unsigned char key, int x, int y)     
{
  if (key == 'Q') {exit(0);}
  if (key == 'a') {rate=n=0; maxsz+=1; printf("New max size %d\n",maxsz); accept=0;}
  if (key == 'z') {rate=n=0; maxsz-=1; if (maxsz==0) maxsz=1; printf("New max size %d\n",maxsz);accept=0; }
  if (key == '=') {rate=n=0; T-=0.001; printf("New temp %e\n",T);accept=0;record=0;}
  if (key == '-') {rate=n=0; T+=0.001; printf("New temp %e\n",T);accept=0;record=0;}
  if (key == 's') {gc*=1.5;}
  if (key == 'x') {gc/=1.5;}
  if (key == 'l') {record=1;}
  tbin=(T-1)*1000;
}    

int main(int argc, char **argv)
{
  //INITIALIZATION
  glutInit(&argc, argv);
  
  //set rgba and double buffering  
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE |  GLUT_MULTISAMPLE);

  //set window size and position and title
  glutInitWindowSize(720,720);
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
  
  for (int i=0; i<DN; i++) for (int j=0; j<DN; j++) {r[i][j]=b[i][j]=g[i][j]=0;}

  for (int i=0; i<N;i++) {
    for (int j=0; j<N;j++) {
      if (drand48()<.9999) spin[i][j]=1; else spin[i][j]=1;
      smag[i]=sct[i]=0;
      init_color(i,j);
    }
  }
  for (int i=0; i<DN; i++) for (int j=0; j<DN; j++) {printf("Colors at %d,%d are %f, %f\n",i,j,r[i][j],b[i][j]);}


  glutMainLoop();
}
