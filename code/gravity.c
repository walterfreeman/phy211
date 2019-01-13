#include <stdio.h>
#include <math.h>

int main(void)
{
  double GM=4*M_PI*M_PI;
  double x=1,y=0,vx=0,vy=1*M_PI;
  double t,dt=1e-5;
  double r;
  int fs=1000,f=0;
  double rbx[5000],rby[5000];  
  int i,rbl=0,j,k;
  for (i=0; i<5000; i++) {rbx[i]=rby[i]=0;}

  for (t=0; 1; t+=dt)
  {
    if (f%fs==0)
    {
      rbx[rbl]=x; rby[rbl]=y;
      printf("c 0 0 0.1\n");
      printf("c %e %e 0.05\n",x,y);
      printf("F\n");
    }
    f++;
    
    x+=vx*dt/2;
    y+=vy*dt/2;
    r=hypot(x,y);
    vx-=(x/r)*GM/(pow(r,2.01))*dt;
    vy-=(y/r)*GM/(pow(r,2.01))*dt;
    x+=vx*dt/2;
    y+=vy*dt/2;
  }
}
