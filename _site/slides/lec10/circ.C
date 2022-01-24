#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define BSZ 1000
#define UNIT cm

void arc(double x, double y, double t1, double t2, double rad, double r, double g, double b)
{
  printf("C %f %f %f\n",r,g,b);
  for (double th=t1;th<t2;th+=(t2-t1)/400)
    printf("l %e %e %e %e\n",x+rad*cos(th),y+rad*sin(th),x+rad*cos(th+(t2-t1)/400),y+rad*sin(th+(t2-t1)/400));
}

void arrow (double x, double y, double lx, double ly, double r, double g, double b)
{
  printf("C %f %f %f\n",r,g,b);
  double xo=-ly;
  double yo=lx;
  double x3=x+lx*0.85 + xo*0.07;
  double x4=x+lx*0.85 - xo*0.07;
  double y3=y+ly*0.85 + yo*0.07;
  double y4=y+ly*0.85 - yo*0.07;
    double x5=x+lx*0.92;
    double y5=y+ly*0.92;
    printf("l %f %f %f %f\n",x,y,x+lx,y+ly);
    printf("l %f %f %f %f\n",x3,y3,x+lx,y+ly);
    printf("l %f %f %f %f\n",x4,y4,x+lx,y+ly);
    printf("l %f %f %f %f\n",x3,y3,x5,y5);
    printf("l %f %f %f %f\n",x4,y4,x5,y5);
}
int main(int argv, char **argc)
{
  double r;
  r=atof(argc[1]);
  double wo=atof(argc[2]);
  double w=wo;
  double fact=atof(argc[3]);
  double dt=1./60.;
  for (double t=0;1;t+=dt)
  {
    if (w*t>2*M_PI) {t-=2*M_PI/w; w=w*fact; if (w>wo*2 || w<wo) fact=1/fact;}
    double x=cos(w*t)*r;
    double y=sin(w*t)*r;
    double vx=-w*sin(w*t)*r*0.5;
    double vy=w*cos(w*t)*r*0.5;
    double ax=-w*w*cos(w*t)*r*0.15;
    double ay=-w*w*sin(w*t)*r*0.15;
  printf("C 1 1 1\nc %e %e 0.05\n",x,y);
    printf("T -0.5 0.9\nAngular velocity: %5.2f rad/s\n",w);
    printf("T -0.5 0.85\nTangential velocity: %5.2f m/s\n",w*r);
    printf("T 0.2 0.9\nAngle: %5.2f radians\n",w*t);
    printf("T 0.2 0.85\nCentripetal accel: %5.2f m/s^2\n",w*w*r);
  printf("C 0 0 %e\nl 0 0 %e %e\n",r,x,y);
  arrow(x,y,vx,vy,1,0,0);
  arrow(x,y,ax,ay,0,1,0);
  arc(0,0,0,2*M_PI,r,0.4,0.4,0.4);
  arc(0,0,0,w*t,0.3,0.8,0.8,0.8);
  printf("F\n");
  }
}
