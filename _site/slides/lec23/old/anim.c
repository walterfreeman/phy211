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

#define timing_hack 0
#define ANIM_FONT GLUT_BITMAP_HELVETICA_18
int window_size=800,window_size_x=800,window_size_y=800;
double scale=4.25;
int fpsdisplay=1;
int help=0;
int need_guesses=1;
int lmx=-1,lmy=-1;
double mcurx, mcury;
double fps=60;
double lastdraw=0;
double xcenter=0,xfcenter=0;
double ycenter=0,yfcenter=0;
double zcenter=0,zfcenter=0;
int axes=0,track=0;
double tx,ty,tz=0;
double maxscale = 0;
double contrast=1;
double vdist=4;
int ctog=1;
double theta=M_PI/4,phi=0,psi=-4*M_PI/3;
double theta2=M_PI/4;
double phi2=0;
double xocenter,yocenter,zocenter;
double psi2=-4*M_PI/3;
double costheta=1,sintheta=0,cosphi=0,sinphi=1,cospsi=1,sinpsi=0;
int td=0;   // flag for 3D mode
int adef=1; // flag for whether or not we should just use the default preference for axes (off in 3d, on in 2d)
int inverse=0;
double gx=0.5,gy=0.4,gz=0.3;

void myColor4f(double r, double g, double b, double a)
{
  a=pow(a,1/contrast);
  if (inverse == 0) glColor4f(r,g,b,a);
  if (inverse == 1) glColor4f(1-(g+b)*0.5,1-(b+r)*0.5,1-(g+r)*0.5,a);
}

void transform(float x, float y, float z, float *X, float *Y, float *Z);


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

void renderBitmapString3(
    float x, 
    float y, 
    float z, 
    void *font, 
    char *string) {  
  char *c;
  float X,Y,Z;
  transform(x,y,z,&X,&Y,&Z);
  glRasterPos3f(X,Y,0);
  for (c=string; *c != '\0'; c++) {
    glutBitmapCharacter(font, *c);
  }
}
void transform3(float x, float y, float z, float *X, float *Y, float *Z)
{
    double xc,yc,zc;
    xc=x-xocenter;
    yc=y-yocenter;
    zc=z-zocenter;
    *X = xc * costheta*cosphi
      + yc * sintheta*cosphi
      + zc * sinphi;

    *Y = (-xc * sintheta + yc*costheta) * cospsi + 
      (-xc* costheta*sinphi-yc*sintheta*sinphi+zc*cosphi)*sinpsi;

    *Z = (-xc * sintheta + yc*costheta) * cosphi * -sinpsi +
      (-xc* costheta*sinphi-yc*sintheta*sinphi+zc*cosphi)*cospsi;
    *X = *X/(*Z+vdist)*(vdist/scale);
    *Y = *Y/(*Z+vdist)*(vdist/scale);
}

void transform4(float x, float y, float z, float *X, float *Y, float *Z)
{
    double xc,yc,zc;
    xc=x-xcenter;
    yc=y-ycenter;
    zc=z-zcenter;
    *X = xc * costheta*cosphi
      + yc * sintheta*cosphi
      + zc * sinphi;

    *Y = (-xc * sintheta + yc*costheta) * cospsi + 
      (-xc* costheta*sinphi-yc*sintheta*sinphi+zc*cosphi)*sinpsi;

    *Z = (-xc * sintheta + yc*costheta) * cosphi * -sinpsi +
      (-xc* costheta*sinphi-yc*sintheta*sinphi+zc*cosphi)*cospsi;
    *X = *X/(*Z+vdist)*(vdist/scale);
    *Y = *Y/(*Z+vdist)*(vdist/scale);
}



void transform(float x, float y, float z, float *X, float *Y, float *Z)
{
    double xc,yc,zc;
    xc=x-xfcenter;
    yc=y-yfcenter;
    zc=z-zfcenter;
    *X = xc * costheta*cosphi
      + yc * sintheta*cosphi
      + zc * sinphi;

    *Y = (-xc * sintheta + yc*costheta) * cospsi + 
      (-xc* costheta*sinphi-yc*sintheta*sinphi+zc*cosphi)*sinpsi;

    *Z = (-xc * sintheta + yc*costheta) * cosphi * -sinpsi +
      (-xc* costheta*sinphi-yc*sintheta*sinphi+zc*cosphi)*cospsi;
    *X = *X/(*Z+vdist)*(vdist/scale);
    *Y = *Y/(*Z+vdist)*(vdist/scale);
}

void transform2(float x, float y, float z, float *X, float *Y, float *Z)
{
    double xc,yc,zc;
    xc=x;
    yc=y;
    zc=z;
    *X = xc * costheta*cosphi
      + yc * sintheta*cosphi
      + zc * sinphi;

    *Y = (-xc * sintheta + yc*costheta) * cospsi + 
      (-xc* costheta*sinphi-yc*sintheta*sinphi+zc*cosphi)*sinpsi;

    *Z = (-xc * sintheta + yc*costheta) * cosphi * -sinpsi +
      (-xc* costheta*sinphi-yc*sintheta*sinphi+zc*cosphi)*cospsi;
    *X = *X/(*Z+vdist)*(vdist/scale);
    *Y = *Y/(*Z+vdist)*(vdist/scale);
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
void lin3(double x1,double y1,double z1,double x2,double y2,double z2)
{
  static float X,Y,Z;
  transform(x1,y1,z1,&X,&Y,&Z);
  if (ctog)  myColor4f(0.5,0.5,0.5,exp(-(Z*4+vdist)/vdist)+.15);
  glVertex3f(X,Y,0);
  transform(x2,y2,z2,&X,&Y,&Z);
  if (ctog)  myColor4f(0.5,0.5,0.5,exp(-(Z*4+vdist)/vdist)+.15);
  glVertex3f(X,Y,0);
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
  static int n,i, num_lines=9;
  static int decimals;
  static float angle;
  static float r;
  static float zavg;
  static float xc,yc,zc,x,y,z,xc1,xc2,yc1,yc2,zc1,zc2;
  static double mx,my,mz,gtx,gty,gtz;
  static float waittime;
  static float spacing;
  static float x1,y1,x2,y2;
  static float red=1;
  static float green=1;
  static float blue=1;
  static char c;
  static char num[200];
  static char line2[300];
  static int frameskip=1;
  static char line[300];
  static int lastframe=5;
  static float framerate;
  static float sep,sep2;
  static float boxsize;
  fgets(line,300,stdin);
  if (feof(stdin)) {sleep(1);return;}


  mx=((float)lmx/window_size_x-0.5)*2;
  my=-((float)lmy/window_size_y-0.5)*2;
  mz=0;

  // transform guess coordinates to screen coordinates
       
   while (need_guesses == 1)
   {
   transform4(gx,gy,gz,&x,&y,&z);
   sep=(x-mx)*(x-mx) + (y-my)*(y-my) + (z-mz)*(z-mz);
   
   gtx=gx+(drand48()-0.5)*scale*0.01;
   gty=gy+(drand48()-0.5)*scale*0.01;
   gtz=gz+(drand48()-0.5)*scale*0.01;
   transform4(gtx,gty,gtz,&x,&y,&z);
   sep2=(x-mx)*(x-mx) + (y-my)*(y-my) + (z-mz)*(z-mz);
   if (sep2<sep) {gx=gtx; gy=gty; gz=gtz;}  
   if (sep < 1.0/window_size) need_guesses=0;
   }
   
//   printf("mouse coords % g, % g, % g; guess coords % g, % g, % g\n",mx,my,mz,x,y,z);

   
   if (track == 1)
     while (1)
   {
     transform4(tx,ty,tz,&x,&y,&z);
     sep=(x-mx)*(x-mx) + (y-my)*(y-my) + (z-mz)*(z-mz);
     gtx=(drand48()-0.5)*scale*0.01;
     gty=(drand48()-0.5)*scale*0.01;
     gtz=(drand48()-0.5)*scale*0.01;
     xcenter+=gtx;
     ycenter+=gty;
     zcenter+=gtz;
     transform4(tx,ty,tz,&x,&y,&z);
     sep2=(x-mx)*(x-mx) + (y-my)*(y-my) + (z-mz)*(z-mz);
     if (sep2 > sep)
     {
       xcenter-=gtx;
       ycenter-=gty;
       zcenter-=gtz;
     }
     if (sep < 1.0/window_size) break;
   }
   

   // draw framelines, 2D
   if (line[0] == 'F' && (axes || adef) && td == 0) {
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
        renderBitmapString((x-xcenter)/scale,-.9,0,ANIM_FONT,num);

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
        renderBitmapString(-.9,(y-ycenter)/scale,0,ANIM_FONT,num);

    }
    }
   // draw frame, 3D
   if (line[0] == 'F' && axes && td) {
    boxsize=scale*0.7;
    myColor4f(.5,.5,.5,1);
    glBegin(GL_LINES);
    lin3(-boxsize+xfcenter,-boxsize+yfcenter,-boxsize+zfcenter, -boxsize+xfcenter, boxsize+yfcenter,-boxsize+zfcenter);
    lin3(-boxsize+xfcenter, boxsize+yfcenter,-boxsize+zfcenter,  boxsize+xfcenter, boxsize+yfcenter,-boxsize+zfcenter);
    lin3( boxsize+xfcenter, boxsize+yfcenter,-boxsize+zfcenter,  boxsize+xfcenter,-boxsize+yfcenter,-boxsize+zfcenter);
    lin3( boxsize+xfcenter,-boxsize+yfcenter,-boxsize+zfcenter, -boxsize+xfcenter,-boxsize+yfcenter,-boxsize+zfcenter);
    lin3(-boxsize+xfcenter,-boxsize+yfcenter, boxsize+zfcenter, -boxsize+xfcenter, boxsize+yfcenter, boxsize+zfcenter);
    lin3(-boxsize+xfcenter, boxsize+yfcenter, boxsize+zfcenter,  boxsize+xfcenter, boxsize+yfcenter, boxsize+zfcenter);
    lin3( boxsize+xfcenter, boxsize+yfcenter, boxsize+zfcenter,  boxsize+xfcenter,-boxsize+yfcenter, boxsize+zfcenter);
    lin3( boxsize+xfcenter,-boxsize+yfcenter, boxsize+zfcenter, -boxsize+xfcenter,-boxsize+yfcenter, boxsize+zfcenter);
    lin3(-boxsize+xfcenter,-boxsize+yfcenter, boxsize+zfcenter, -boxsize+xfcenter,-boxsize+yfcenter,-boxsize+zfcenter);
    lin3( boxsize+xfcenter,-boxsize+yfcenter, boxsize+zfcenter,  boxsize+xfcenter,-boxsize+yfcenter,-boxsize+zfcenter);
    lin3(-boxsize+xfcenter, boxsize+yfcenter, boxsize+zfcenter, -boxsize+xfcenter, boxsize+yfcenter,-boxsize+zfcenter);
    lin3( boxsize+xfcenter, boxsize+yfcenter, boxsize+zfcenter,  boxsize+xfcenter, boxsize+yfcenter,-boxsize+zfcenter);
    spacing=orderofmagnitude(scale/3);
    for(x=rounddown(xfcenter-boxsize,spacing);x<roundup(xfcenter+boxsize,spacing);x+=spacing)
    {
      if (fabs(x) < spacing*.001) x=0;
      sprintf(num,"%.6g",x);
        if (x > xfcenter-boxsize && x < xfcenter+boxsize)
        {
          glBegin(GL_LINES);
          lin3(x,boxsize+yfcenter,-boxsize+zfcenter,x,boxsize*0.96+yfcenter,-boxsize+zfcenter);
          lin3(x,boxsize+yfcenter,-boxsize+zfcenter,x,boxsize+yfcenter,-boxsize*0.96+zfcenter);
          lin3(x,boxsize+yfcenter,boxsize+zfcenter,x,boxsize*0.96+yfcenter,boxsize+zfcenter);
          lin3(x,boxsize+yfcenter,boxsize+zfcenter,x,boxsize+yfcenter,boxsize*0.96+zfcenter);
          lin3(x,-boxsize+yfcenter,boxsize+zfcenter,x,-boxsize*0.96+yfcenter,boxsize+zfcenter);
          lin3(x,-boxsize+yfcenter,boxsize+zfcenter,x,-boxsize+yfcenter,boxsize*0.96+zfcenter);
          lin3(x,-boxsize+yfcenter,-boxsize+zfcenter,x,-boxsize*0.96+yfcenter,-boxsize+zfcenter);
          lin3(x,-boxsize+yfcenter,-boxsize+zfcenter,x,-boxsize+yfcenter,-boxsize*0.96+zfcenter);
          glEnd(); 
          renderBitmapString3(x,-boxsize*1.04+yfcenter,-boxsize*1.04+zfcenter,ANIM_FONT,num);
        }
    }
    for(y=rounddown(yfcenter-boxsize,spacing);y<roundup(yfcenter+boxsize,spacing);y+=spacing)
    {
      if (fabs(y) < spacing*.001) y=0;
      sprintf(num,"%.6g",y);
        if (y > yfcenter-boxsize && y < yfcenter+boxsize)
        {
          glBegin(GL_LINES);
          lin3(boxsize+xfcenter,y,boxsize+zfcenter,boxsize*0.96+xfcenter,y,boxsize+zfcenter);
          lin3(boxsize+xfcenter,y,boxsize+zfcenter,boxsize+xfcenter,y,boxsize*0.96+zfcenter);
          lin3(-boxsize+xfcenter,y,boxsize+zfcenter,-boxsize*0.96+xfcenter,y,boxsize+zfcenter);
          lin3(-boxsize+xfcenter,y,boxsize+zfcenter,-boxsize+xfcenter,y,boxsize*0.96+zfcenter);
          lin3(boxsize+xfcenter,y,-boxsize+zfcenter,boxsize*0.96+xfcenter,y,-boxsize+zfcenter);
          lin3(boxsize+xfcenter,y,-boxsize+zfcenter,boxsize+xfcenter,y,-boxsize*0.96+zfcenter);
          lin3(-boxsize+xfcenter,y,-boxsize+zfcenter,-boxsize*0.96+xfcenter,y,-boxsize+zfcenter);
          lin3(-boxsize+xfcenter,y,-boxsize+zfcenter,-boxsize+xfcenter,y,-boxsize*0.96+zfcenter);
          glEnd(); 
          renderBitmapString3(-boxsize*1.04+xfcenter,y,-boxsize*1.04+zfcenter,ANIM_FONT,num);
        }
    }
    for(z=rounddown(zfcenter-boxsize,spacing);z<roundup(zfcenter+boxsize,spacing);z+=spacing)
    {
      if (fabs(z) < spacing*.001) z=0;
      sprintf(num,"%.6g",z);
        if (z > zfcenter-boxsize && z < zfcenter+boxsize)
        {
          glBegin(GL_LINES);
          lin3(boxsize+xfcenter,boxsize+yfcenter,z,boxsize*0.96+xfcenter,boxsize+yfcenter,z);
          lin3(boxsize+xfcenter,boxsize+yfcenter,z,boxsize+xfcenter,boxsize*0.96+yfcenter,z);
          lin3(boxsize+xfcenter,-boxsize+yfcenter,z,boxsize*0.96+xfcenter,-boxsize+yfcenter,z);
          lin3(boxsize+xfcenter,-boxsize+yfcenter,z,boxsize+xfcenter,-boxsize*0.96+yfcenter,z);
          lin3(-boxsize+xfcenter,boxsize+yfcenter,z,-boxsize*0.96+xfcenter,boxsize+yfcenter,z);
          lin3(-boxsize+xfcenter,boxsize+yfcenter,z,-boxsize+xfcenter,boxsize*0.96+yfcenter,z);
          lin3(-boxsize+xfcenter,-boxsize+yfcenter,z,-boxsize*0.96+xfcenter,-boxsize+yfcenter,z);
          lin3(-boxsize+xfcenter,-boxsize+yfcenter,z,-boxsize+xfcenter,-boxsize*0.96+yfcenter,z);
          glEnd(); 
          renderBitmapString3(-boxsize*1.04+xfcenter,-boxsize*1.04+yfcenter,z,ANIM_FONT,num);
        }
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

  // set FPS
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
    transform(xc1,yc1,zc1,&x,&y,&z);
    r = r/(z+vdist)*(vdist/scale);

    if (ctog)            myColor4f(red,green,blue,exp(-(z*4+vdist)/vdist)+.15);



    if (td==0) num_lines=sqrt(window_size*r/scale)*2+4; else num_lines=40;
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
    transform(xc1,yc1,zc1,&x,&y,&z);
    r = r/(z+vdist)*(vdist/scale);
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

    transform(xc1,yc1,zc1,&x,&y,&z);
    glBegin(GL_LINES);
    if (ctog)            myColor4f(red,green,blue,exp(-(z*20+vdist)/vdist)+.15);
    glVertex3f(x,y,0);
    transform(xc2,yc2,zc2,&x,&y,&z);    
    if (ctog)      myColor4f(red,green,blue,exp(-(z*20+vdist)/vdist)+.15);
    glVertex3f(x,y,0);
    glEnd();
  }

  else if (line[0] == 'T')
  {
    sscanf(&line[1],"%f %f",&x,&y);
    fgets(line2,300,stdin);
    renderBitmapString(x,y,0,GLUT_BITMAP_TIMES_ROMAN_24,line2);
  }
  else if (line[0] == 't' && line[1] == '3')
  {
    td=1;
    sscanf(&line[2],"%f %f %f",&xc,&yc,&zc);

    transform(xc,yc,zc,&x,&y,&z);

//    if (ctog)      myColor4f(red,green,blue,exp(-(z*20+vdist)/vdist)+.15);
//    x = x/(z+vdist)*(vdist/scale);
//    y = y/(z+vdist)*(vdist/scale);
    fgets(line2,300,stdin);
    //    glColor3f(0,0,0);
    renderBitmapString(x,y,0,GLUT_BITMAP_TIMES_ROMAN_24,line2);
  }



  else if (line[0] == 't' && line[1] != '3') // text, not 3d
  {
    sscanf(&line[1],"%f %f",&x,&y);
    x -= xcenter; y -= ycenter;
    fgets(line2,300,stdin);
    renderBitmapString(x/scale,y/scale,0,GLUT_BITMAP_TIMES_ROMAN_24,line2);
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
    xfcenter=xcenter;
    yfcenter=ycenter;
    zfcenter=zcenter;
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

        if (timing_hack) usleep(waittime*1000);
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


void mouse(int button, int state,int x, int y)
{
  if (button == 4) {scale *= 1.03; vdist *= 1.03;}
  if (button == 3) {scale /= 1.03; vdist /= 1.03;}
  if (button == 0 && state == GLUT_DOWN) {track=1;tx=gx;ty=gy;tz=gz;xocenter=xcenter;yocenter=ycenter;zocenter=zcenter;} 
  if (button == 0 && state == GLUT_UP) {track=0;} 
}

void track_mouse(int x, int y)
{
  lmx=x;
  lmy=y;
  need_guesses=1;
}

void move_mouse(int x, int y)
{
  lmx=x;
  lmy=y;
  return;
  double DX,DY,pf=1,xt,yt,zt;
  if (lmx == -1)
  {
    lmx=x;
    lmy=y;
    return;
  }
  DX=(double)(x-lmx)/(window_size_x)*2;
  DY=(double)(y-lmy)/(window_size_y)*2;
  lmx=x;
  lmy=y;
//  xcenter -= DY/(sintheta * cospsi);
//  ycenter -= DY/(costheta * cospsi);
//  zcenter -= DY/(cosphi * sinpsi);
//  xcenter -= DX/(costheta * cosphi);
//  ycenter -= DX/(sintheta * cosphi);
//  zcenter -= DX/(costheta * cosphi);

//  pf = (cosphi*costheta*sinpsi + cospsi*sinphi*sintheta)*(cosphi*cosphi*costheta*sinpsi + costheta*sinphi*sinphi*sinpsi + 
//          cospsi*sinphi*sintheta) + (-costheta*sinphi*sinpsi + cosphi*sinpsi*sintheta)*(-cospsi*costheta*sinphi + 
//                cosphi*cosphi*sinpsi*sintheta + sinphi*sinphi*sinpsi*sintheta) + 
 //          cosphi*cospsi*(cosphi*cospsi*costheta*costheta + cosphi*cospsi*sintheta*sintheta);
 
// pf = cospsi*cospsi+cosphi*sinpsi*sinpsi;
  
//  xt = DX * pf *( cosphi*costheta*(cospsi*cospsi+cosphi*sinpsi*sinpsi));
//  ycenter -= DX * pf *( -cosphi*costheta*sinphi*sinpsi-cosphi*cosphi*cospsi*sintheta-cospsi*sinphi*sinphi*sintheta); 
//  zcenter -= DX * pf *( -cospsi*costheta*sinphi+1);
//  yt = DX * pf * (  cosphi*(cospsi*costheta*sinphi*sinpsi + cospsi*cospsi*sintheta + sinpsi*sinpsi*(-costheta*sinphi+cosphi*sintheta)));
//  zt = DX * pf * ( -sinphi*(cospsi*cospsi*sintheta*sintheta + cospsi*costheta*sinpsi*(costheta + sinphi*sintheta) + 
//                         sinpsi*sinpsi*(-costheta*sinphi*sintheta+cosphi*(costheta*costheta+sintheta*sintheta))));
//  xt = DX * pf * (cosphi*costheta*(cospsi*cospsi+cosphi*sinpsi*sinpsi));
//  yt = DX * pf * (cosphi*(cospsi*cospsi+cosphi*sinpsi*sinpsi)*sintheta);
//  zt = DX * pf * (sinphi*(1 + cosphi*sinpsi*sinpsi));  
//  xt-= DY * pf * (-cosphi*costheta*sinphi*sinpsi-1);
//  yt-= DY * pf * (cospsi*costheta - cosphi*sinphi*sinpsi*sintheta);
//  zt+= DY * pf * (cosphi*cosphi*sinpsi);
  
  //  yt = DX * pf * (-cospsi*costheta*sinphi + sinpsi*sintheta);

  printf("translate: % g, % g, % g (angles over pi are th/ps/ph % g, % g, % g)\n",xt,yt,zt,theta/M_PI,psi/M_PI,phi/M_PI);
//  xcenter -= xt;
//  ycenter -= yt;
//  zcenter -= zt;


//    xcenter -= DX;
//    ycenter -= DY;

//  printf("mouse at %d, %d\n",x,y);
}

void resize(int w, int h)
{
  window_size_x=w;
  window_size_y=h;
  if (h>w) window_size=h; else window_size=w;
   glViewport(0,0,w,h);
}

void keyb(unsigned char key, int x, int y)     
{
  if (key == 'Q') {exit(0);}
  if (key == 'A') {axes=1-axes; adef=0;}
  if (key == 'F') {fpsdisplay = 1-fpsdisplay;}
  if (key == ',') {contrast /= 1.2;}
  if (key == '.') {contrast *= 1.2;}
  if (key == '=') {scale /= 1.02;vdist/=1.02;}
  if (key == '-') {scale *= 1.02;vdist*=1.02;}
  if (td) {
    if (key == 'a') {theta2 += 0.1;}
    if (key == 'd') {theta2 -= 0.1;}
    if (key == 'q') {phi2   += 0.1;}
    if (key == 'e') {phi2   -= 0.1;}
    if (key == 'w') {psi2   -= 0.1;}
    if (key == 's') {psi2   += 0.1;}
    if (key == '_') {vdist *= 1.02;}
    if (key == '+') {vdist /= 1.02;}
    if (key == 'h') {xcenter += 0.02*scale;;}
    if (key == 'j') {ycenter += 0.02*scale;;}
    if (key == 'k') {xcenter -= 0.02*scale;;}
    if (key == 'u') {ycenter -= 0.02*scale;;}
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
  //  if (key == 'h') {help = 1-help;}
}    

int main(int argc, char **argv)
{
  xcenter=ycenter=zcenter=0; 
  theta=theta2;
  psi=psi2;
  phi=phi2;
  cachetrig();

  if (argc == 2)
  {
    sscanf(argv[1],"%d",&window_size);
  }
  //INITIALIZATION
  glutInit(&argc, argv);

  //set rgba and double buffering  
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE |  GLUT_MULTISAMPLE);

  //set window size and position and title
  glutInitWindowSize(window_size,window_size);
  glutInitWindowPosition(100,100);
  glutCreateWindow("Animation");

  //SET CALLBACKS
  glutDisplayFunc(disp);
  glutKeyboardFunc(keyb);
  glutIdleFunc(idle);
  glutMouseFunc(mouse);
  glutPassiveMotionFunc(track_mouse);
  glutMotionFunc(move_mouse);
  glutReshapeFunc(resize);
  //DO OPENGL INIT
  glEnable(GL_BLEND);
  glEnable(GL_MULTISAMPLE);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
  glClearColor(0.0, 0.0, 0.0, 1.0);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glutMainLoop();
}
