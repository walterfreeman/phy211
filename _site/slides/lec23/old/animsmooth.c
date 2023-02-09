/**********************************************
anim.c, simple animation routine using OpenGL/GLUT
Written by Walter Freeman

Compile (on most systems) with
gcc anim.c -lGL -lGLU -lglut -lm
**********************************************/


#include <GL/glut.h>         // this might have to be changed on Macs
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#define window_size 800
double scale=1.25;
int fpsdisplay=1;
int help=0;
double fps=60;
double lastdraw=0;
double xcenter=0;
double ycenter=0;
double zcenter=0;
int axes=0;
double maxscale = 0;
double contrast=1;
double vdist=12;
int ctog=1;
double theta=M_PI/4,phi=0,psi=4*M_PI/3;
double theta2=M_PI/4;
double phi2=0;
FILE *debug;
double psi2=4*M_PI/3;
double costheta=1,sintheta=0,cosphi=0,sinphi=1,cospsi=1,sinpsi=0;
int td=0;   // flag for 3D mode
int adef=1; // flag for whether or not we should just use the default preference for axes (off in 3d, on in 2d)
int inverse=0;


void myColor4f(double r, double g, double b, double a)
{
  a=pow(a,1/contrast);
  if (inverse == 0) glColor4f(r,g,b,a);
  if (inverse == 1) glColor4f(1-(g+b)*0.5,1-(b+r)*0.5,1-(g+r)*0.5,a);
}
    

double cachetrig()
{
  costheta=cos(theta);
  sintheta=sin(theta);
  sinphi=sin(phi);
  cosphi=cos(phi);
  sinpsi=sin(psi);
  cospsi=cos(psi);
  
}

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



void idle(void)
{
  int n,i, num_lines=9;
  int decimals;
  float angle;
  float r;
  float zavg;
  float xc,yc,zc,x,y,z,xc1,xc2,yc1,yc2,zc1,zc2;
  float waittime;
  float spacing;
  static float x1,y1,x2,y2;
  static float red=1;
  static float green=1;
  static float blue=1;
  static char c;
  static char num[200];
  static char line2[300];
  static int frameskip=1;
  static char line[300];
  fgets(line,300,stdin);
  if (feof(stdin)) {sleep(1);return;}
  static int lastframe=5;
  static float framerate;
  if (line[0] == 'F' && (axes || (adef == 1 && td == 0))) {
  myColor4f(.5,.5,.5,1);
  glBegin(GL_LINES);
  lin(-.8,-.8,-.8,.8);
  lin(-.8,.8,.8,.8);
  lin(.8,.8,.8,-.8);
  lin(.8,-.8,-.8,-.8);
  
  glEnd(); 

  lastframe = glutGet(GLUT_ELAPSED_TIME);  
  spacing=orderofmagnitude(scale/3);
  for(x=rounddown(xcenter-scale,spacing);x<roundup(xcenter+scale,spacing);x+=spacing)
  {
    if (fabs(x) < spacing*.001) x=0;
    glBegin(GL_LINES);
    sprintf(num,"%.6g",x);
    if ((x-xcenter)/scale > -.801 && (x-xcenter)/scale < .801)
      lin((x-xcenter)/scale,-.8,(x-xcenter)/scale,-.75);
    glEnd();
   
    if ((x-xcenter)/scale > -.801 && (x-xcenter)/scale < .801)
      renderBitmapString((x-xcenter)/scale,-.9,0,GLUT_BITMAP_TIMES_ROMAN_10,num);
   
  }
 
  for(y=rounddown(ycenter-scale,spacing);y<roundup(ycenter+scale,spacing);y+=spacing)
  {
    if (fabs(y) < spacing*.001) y=0;
    glBegin(GL_LINES);
    sprintf(num,"%.6g",y);
    if ((y-ycenter)/scale > -.801 && (y-ycenter)/scale < .801)
      lin(-.8,(y-ycenter)/scale,-.75,(y-ycenter)/scale);
    glEnd();

    if ((y-ycenter)/scale > -.801 && (y-ycenter)/scale < .801)
      renderBitmapString(-.9,(y-ycenter)/scale,0,GLUT_BITMAP_TIMES_ROMAN_10,num);

  }


  }

  if (line [0] == 'c' && line [1] == 'e' && line[2] == 'n' && line[3] == 't' && line[4] == 'e' && line[5] == 'r')
  {
    if (line[6] == '3' || td) sscanf(&line[8],"%lf %lf %lf",&xcenter,&ycenter,&zcenter);
      else  sscanf(&line[7],"%lf %lf",&xcenter,&ycenter);
  }


  else if (line[0] == '!') { // bypass
    printf("%s",&line[1]);
  }
  else if (line[0] == 'F' && line[1] == 'P' && line[2] == 'S')
  {
    sscanf(&line[4],"%lf",&fps);
  }
  else if (line[0] == 'l' && line[1] == ' ') { // line, no color specified
    sscanf(line,"%c %f %f %f %f",&c,&x1,&y1,&x2,&y2);

    x1 -= xcenter; y1 -= ycenter; x2 -= xcenter; y2 -= ycenter;

    glBegin(GL_LINES);
      glVertex3f(x1/scale,y1/scale,0);
      glVertex3f(x2/scale,y2/scale,0);
    glEnd(); }

  else if (line[0] == 'c' && line[1] == '3' && line[2] == ' ') { // 3d circle
      td=1;
      sscanf(&line[3],"%f %f %f %f",&xc1,&yc1,&zc1,&r);
 xc1 -= xcenter; yc1 -= ycenter; zc1 -= zcenter;
    
      xc=xc1;yc=yc1;zc=-zc1;

      x = xc * costheta*cosphi
	+ yc * sintheta*cosphi
	+ zc * sinphi;

      y = (-xc * sintheta + yc*costheta) * cospsi + 
	(-xc* costheta*sinphi-yc*sintheta*sinphi+zc*cosphi)*sinpsi;
      z = (-xc * sintheta + yc*costheta) * cos(psi) * -sinpsi +
         (-xc* costheta*sinphi-yc*sintheta*sinphi+zc*cosphi)*cospsi;
        r = r/(z+vdist)*(vdist/scale);
	      x = x/(z+vdist)*(vdist/scale);
	 y = y/(z+vdist)*(vdist/scale);
if (ctog)            myColor4f(red,green,blue,exp(-(z*4+vdist)/vdist)+.15);



    if (td==0) num_lines=sqrt(window_size*r/scale)*2+4; else num_lines=12;
    glBegin(GL_LINE_LOOP);
    for (i=0;i<num_lines;i++)
    {
      angle = i*2*M_PI/num_lines;
      glVertex3f((x+cos(angle)*r), (y+sin(angle)*r),0);
    }
    glEnd();
  }

else if (line[0] == 'd' && line[1] == '3' && line[2] == ' ') { // 3d diamond
      td=1;
      sscanf(&line[3],"%f %f %f %f",&xc1,&yc1,&zc1,&r);
      xc1 -= xcenter; yc1 -= ycenter; zc1 -= zcenter;
      xc=xc1;yc=yc1;zc=-zc1;

      x = xc * costheta*cosphi
	+ yc * sintheta*cosphi
	+ zc * sinphi;

      y = (-xc * sintheta + yc*costheta) * cospsi + 
	(-xc* costheta*sinphi-yc*sintheta*sinphi+zc*cosphi)*sinpsi;

      z = (-xc * sintheta + yc*costheta) * cos(psi) * -sinpsi +
         (-xc* costheta*sinphi-yc*sintheta*sinphi+zc*cosphi)*cospsi;
        r = r/(z+vdist)*(vdist/scale);
	      x = x/(z+vdist)*(vdist/scale);
	 y = y/(z+vdist)*(vdist/scale);
if (ctog)            myColor4f(red,green,blue,exp(-(z*4+vdist)/vdist)+.15);



    num_lines=4;
    glBegin(GL_LINE_LOOP);
    for (i=0;i<num_lines;i++)
    {
      angle = i*2*M_PI/num_lines;
      glVertex3f((x+cos(angle)*r), (y+sin(angle)*r),0);
    }
    glEnd();
  }


  else if (line[0] == 'l' && line[1] == '3' && line[2] == ' ') { // 3d line
      td=1;
      sscanf(&line[3],"%f %f %f %f %f %f",&xc1,&yc1,&zc1,&xc2,&yc2,&zc2);
      xc1 -= xcenter; xc2 -= xcenter;
      yc1 -= ycenter; yc2 -= ycenter;
      zc1 -= zcenter; zc2 -= zcenter;
      
      xc=xc1;yc=yc1;zc=-zc1;

      x = xc * costheta*cosphi
	+ yc * sintheta*cosphi
	+ zc * sinphi;

      y = (-xc * sintheta + yc*costheta) * cospsi + 
	(-xc* costheta*sinphi-yc*sintheta*sinphi+zc*cosphi)*sinpsi;

      z = (-xc * sintheta + yc*costheta) * cospsi * -sinpsi +
         (-xc* costheta*sinphi-yc*sintheta*sinphi+zc*cosphi)*cospsi;
      glBegin(GL_LINES);

      x = x/(z+vdist)*(vdist/scale);
      y = y/(z+vdist)*(vdist/scale);
if (ctog)            myColor4f(red,green,blue,exp(-(z*20+vdist)/vdist)+.15);
	    glVertex3f(x,y,0);
      
 xc=xc2;yc=yc2;zc=-zc2;


      x = xc * costheta*cosphi
	+ yc * sintheta*cosphi
	+ zc * sinphi;

      y = (-xc * sintheta + yc*costheta) * cospsi + 
	(-xc* costheta*sinphi-yc*sintheta*sinphi+zc*cosphi)*sinpsi;
      zavg += z/2;
      z = (-xc * sintheta + yc*costheta) * cospsi * -sinpsi +
         (-xc* costheta*sinphi-yc*sintheta*sinphi+zc*cosphi)*cospsi;
      zavg += z/2;

if (ctog)      myColor4f(red,green,blue,exp(-(z*20+vdist)/vdist)+.15);
      x = x/(z+vdist)*(vdist/scale);
      y = y/(z+vdist)*(vdist/scale);

 glVertex3f(x,y,0);
 glEnd();
    }

  else if (line[0] == 'T')
  {
    sscanf(&line[1],"%f %f",&x,&y);
    fgets(line2,300,stdin);
    renderBitmapString(x,y,0,GLUT_BITMAP_TIMES_ROMAN_10,line2);
  }
  else if (line[0] == 't' && line[1] == '3')
  {
    td=1;
    sscanf(&line[2],"%f %f %f",&xc,&yc,&zc);
      xc -= xcenter; yc -= ycenter; zc -= zcenter;
      zc = -zc;

      x = xc * costheta*cosphi
        + yc * sintheta*cosphi
        + zc * sinphi;

      y = (-xc * sintheta + yc*costheta) * cospsi +
        (-xc* costheta*sinphi-yc*sintheta*sinphi+zc*cosphi)*sinpsi;
      zavg += z/2;
      z = (-xc * sintheta + yc*costheta) * cospsi * -sinpsi +
         (-xc* costheta*sinphi-yc*sintheta*sinphi+zc*cosphi)*cospsi;      

      zavg += z/2;

if (ctog)      myColor4f(red,green,blue,exp(-(z*20+vdist)/vdist)+.15);
      x = x/(z+vdist)*(vdist/scale);
      y = y/(z+vdist)*(vdist/scale);
    fgets(line2,300,stdin);
    renderBitmapString(x,y,0,GLUT_BITMAP_TIMES_ROMAN_10,line2);
  }



  else if (line[0] == 't' && line[1] != '3') // text, not 3d
  {
    sscanf(&line[1],"%f %f",&x,&y);
    x -= xcenter; y -= ycenter;
    fgets(line2,300,stdin);
    renderBitmapString(x/scale,y/scale,0,GLUT_BITMAP_TIMES_ROMAN_10,line2);
  }
  else if (line[0] == 'C') { //just set color
    sscanf(line,"%c %f %f %f",&c,&red,&green,&blue);
    myColor4f(red,green,blue,1);
  }
  else if (line[0] == 'A') { // toggle gridlines
    sscanf(&line[1],"%d",&axes);
  }
  
  else if (line[0] == 'S' && line[1] == ' ') { // set scale
    sscanf(&line[1],"%lf", &scale);  
  }
  else if (line[0] == 'S' && line[1] == '3') { // set scale
    sscanf(&line[2],"%lf", &vdist); 
    fprintf(stderr,"Viewing distance set to %e\n",vdist); 
    scale=vdist; // they probably wanted to do this too
  }
  else if (line[0] == 'l' && line[1] == 'C') { // line, color
    sscanf(line,"%c%c %f %f %f %f %f %f %f",&c,&c,&x1,&y1,&x2,&y2,&red,&green,&blue);

    x1 -= xcenter; x2 -= xcenter; y1 -= ycenter; y2 -= ycenter;

    myColor4f(red,green,blue,1); 
    glBegin(GL_LINES);
      glVertex3f(x1/scale,y1/scale,0);
      glVertex3f(x2/scale,y2/scale,0);
    glEnd(); }

  else if (line[0] == 'c' && line[1] == ' ') { // circle
    sscanf(line,"%c %f %f %f",&c,&x1,&y1,&r);

     x1 -= xcenter; y1 -= ycenter;

    if (r < scale/window_size*1.5)
      {  
	r= scale/window_size*1.5;
      }
    num_lines=sqrt(window_size*r/scale)*2+4;
    glBegin(GL_LINE_LOOP);
    for (i=0;i<num_lines;i++)
    {
      angle = i*2*M_PI/num_lines;
      glVertex3f((x1+cos(angle)*r)/scale, (y1+sin(angle)*r)/scale,0);
    }
    glEnd();
  }
   else if (line[0] == 'd' && line[1] == ' ') { // diamond
    sscanf(line,"%c %f %f %f",&c,&x1,&y1,&r);

     x1 -= xcenter; y1 -= ycenter;

    num_lines=4;
    glBegin(GL_LINE_LOOP);
    for (i=0;i<num_lines;i++)
    {
      angle = i*2*M_PI/num_lines;
      glVertex3f((x1+cos(angle)*r)/scale, (y1+sin(angle)*r)/scale,0);
    }
    glEnd();
  }
  else if (line[0] == 'F' && line[1] != 'P') {
    cachetrig();  // flush the frame
  if (help && !td)
  {
    glColor4f(1,1,1,1);
    sprintf(num,"WASD: move around     -/=: zoom       ,/.: change brightness     h: toggle this help     shift-Q: exit"); renderBitmapString(-.9,.96,0,GLUT_BITMAP_TIMES_ROMAN_10,num); 
    sprintf(num,"shift-I: invert brightness      shift-F: toggle display of framerate, etc     shift-A: toggle display of axes"); renderBitmapString(-.9,.93,0,GLUT_BITMAP_TIMES_ROMAN_10,num); 
  }
  
   if (help && td)
  {
    glColor4f(1,1,1,1);
    sprintf(num,"WASDQE: rotate     -/=: zoom     shift -/=: change angle of view     ,/.: change brightness     h: toggle this help     shift-Q: exit"); renderBitmapString(-.9,.96,0,GLUT_BITMAP_TIMES_ROMAN_10,num); 
    sprintf(num,"shift-I: invert brightness      shift-F: toggle display of framerate, etc     shift-A: toggle display of axes"); renderBitmapString(-.9,.93,0,GLUT_BITMAP_TIMES_ROMAN_10,num); 
  }


  if (fpsdisplay) {
  myColor4f(1,1,1,0.5);
  sprintf(num,"%.3f FPS\t target %.2f\n",framerate,fps);
  renderBitmapString(-.9,-.96,0,GLUT_BITMAP_TIMES_ROMAN_10,num);
  if (td) {sprintf(num,"Camera distance: %.3g\tAngle of view: %.3g degrees\t",vdist,2*atan(scale/vdist)*180/M_PI);
  renderBitmapString(-.9,-.93,0,GLUT_BITMAP_TIMES_ROMAN_10,num);}
  }
  
    if (fps > 0) 
    {
      waittime = (-glutGet(GLUT_ELAPSED_TIME)+lastdraw+1000./fps);
      if (waittime > 0)
      {
//      if (waittime > 100 || waittime < 0) printf("Waiting %f msec. Last drew at %f, it is now %d.\n",waittime,lastdraw,glutGet(GLUT_ELAPSED_TIME));
      
      usleep(waittime*1000);
      }
    }
    
    theta=(theta*0.96+theta2*0.04);
    phi=(phi*0.96+phi2*0.04);
    psi=(psi*0.96+psi2*0.04);
    myColor4f(red,green,blue,1);     
    glutSwapBuffers(); 
    glClear(GL_COLOR_BUFFER_BIT);
    framerate = 1000/(glutGet(GLUT_ELAPSED_TIME)-lastdraw);
    lastdraw=glutGet(GLUT_ELAPSED_TIME);
  }
}

void disp(void)
{
}

void keyb(unsigned char key, int x, int y)     
{
  if (key == 'Q') {exit(0);}
  if (key == 'A') {axes=1-axes; adef=0;}
  if (key == 'F') {fpsdisplay = 1-fpsdisplay;}
  if (key == ',') {contrast /= 1.2;}
  if (key == '.') {contrast *= 1.2;}
  if (key == '=') {scale /= 1.02;}
  if (key == '-') {scale *= 1.02;}
  if (td) {
    if (key == 'a') {theta2 += 0.1;}
    if (key == 'd') {theta2 -= 0.1;}
    if (key == 'q') {phi2   += 0.1;}
    if (key == 'e') {phi2   -= 0.1;}
    if (key == 'w') {psi2   += 0.1;}
    if (key == 's') {psi2   -= 0.1;}
    if (key == '_') {vdist *= 1.02;}
    if (key == '+') {vdist /= 1.02;}
  }
  else
  {
    if (key == 'a') {xcenter += 0.02*scale;;}
    if (key == 's') {ycenter += 0.02*scale;;}
    if (key == 'd') {xcenter -= 0.02*scale;;}
    if (key == 'w') {ycenter -= 0.02*scale;;}
  }

  if (key == 'I') {inverse=1-inverse; glClearColor(inverse, inverse, inverse, 1.0); if (inverse) contrast*3; else contrast/3; printf("ASDFASDF\n");}
  if (key == 'C') {ctog = 1-ctog;}
  if (key == 'H') {help = 1-help;}
  if (key == 'h') {help = 1-help;}
}    

int main(int argc, char **argv)
{
  debug=fopen("anim-debug","w");
  xcenter=ycenter=zcenter=0; 
  cachetrig();
  int size=window_size;
  if (argc == 2)
  {
    sscanf(argv[1],"%d",&size);
  }
  //INITIALIZATION
  glutInit(&argc, argv);
  
  //set rgba and double buffering  
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

  //set window size and position and title
  glutInitWindowSize(size,size);
  glutInitWindowPosition(100,100);
  glutCreateWindow("Animation");

  //SET CALLBACKS
  glutDisplayFunc(disp);
  glutKeyboardFunc(keyb);
  glutIdleFunc(idle);

  //DO OPENGL INIT
  glEnable(GL_BLEND);
  glEnable(GL_LINE_SMOOTH);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
  glClearColor(0.0, 0.0, 0.0, 1.0);
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  
  glutMainLoop();
}
