#include <stdio.h>
#include <math.h>
#include <stdlib.h>
double w,c,k,l;

double A1,A2;

double Y1(double x, double t)
{
      return A1*sin(w*t-k*x)+0;
      }

double Y2(double x, double t)
{
      return A2*sin(-w*t-k*x)+3;
      }

double Y3(double x, double t)
{
      return Y1(x,t) + Y2(x,t) + 6.5;
      }


int main(int argc, char **argv)
{
  fprintf(stderr,"Usage: <this> <wavelength> <wavespeed> <amp 1> <amp 2> <x to label>\n");
  l=atof(argv[1]);
  printf("!wavelength is %f\n",l);
  k=2*M_PI/l;
  double xd,r,x,t,dx=0.1;
  c=atof(argv[2]);
  A1=atof(argv[3]);
  A2=atof(argv[4]);
  xd=atof(argv[4]);
  w=c*k;
  printf("!wave number is %f; frequency is %f; wave speed is %f\n",k,w,c);
  for (t=0; 1; t+=1./60.)
  {
    printf("C 1 0 0\n");
    for (x=-15; x<35; x+=0.01)
    {
      printf("l %e %e %e %e\n",x,Y1(x,t),x-dx,Y1(x-dx,t));
    }
    printf("C 0 1 0\n");
    for (x=-15; x<35; x+=0.01)
    {
      printf("l %e %e %e %e\n",x,Y2(x,t),x-dx,Y2(x-dx,t));
    }
    printf("C .3 .3 1\n");
    for (x=-15; x<35; x+=0.01)
    {
      printf("l %e %e %e %e\n",x,Y3(x,t),x-dx,Y3(x-dx,t));
    }
    printf("C 1 1 1\n");
    for (r=0;r<0.05;r+=0.01)
    {
      printf("c %e %e %e\n",xd,Y3(xd,t),r);
    }
    printf("l 0 5 0 8\n");
    printf("l 6 5 6 8\n");
    printf("T -0.5 0.85\nWavelength %.3f Frequency %.3f\n",l,w/(2*M_PI));
      printf("F\n");
  }
}
