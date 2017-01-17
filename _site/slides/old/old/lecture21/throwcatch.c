#include <stdio.h>
#include <math.h>


void sled(double x, double y, double th)
{
  printf("l %e %e %e %e\n",x-cos(th)+0.0*sin(th),y-sin(th)+0.0*cos(th),x+cos(th)-0.0*sin(th),y+sin(th)-0.0*cos(th));
  printf("C 1 0 0\nc %e %e 0.15\nC 1 1 1\n",x,y);
}

void loop(void)
{
  int flag=0;
  double t,x1=-10,y1=1,x2=-10,y2=0,vx1=2,vy1=0,vx2=2,vy2=0,xs=-10,ys=1,vxs=2,vys=0,w2=0,th2=0,dt=0.01;
  for (t=0;t<8;t+=dt)
  {
    if (t>3 && flag==0)
    {
      flag=1;
      vx1*=7./6.;
      vxs=0;
      vys=-5;
      vy1=1./3.;
    }
    if (t>3.2 && flag==1)
    {
      flag=2;
      vx2*=6./7.;
      vy2=-1./3.;
      w2=1;
      vxs=vx2;
      vys=vy2;
    }
 x1+=vx1*dt;
    y1+=vy1*dt;
    y2+=vy2*dt;
    x2+=vx2*dt;
    xs+=vxs*dt;
    ys+=vys*dt;
    th2+=w2*dt;
    if (flag==2)
    {
      xs=x2-0.4*cos(th2);
      ys=y2-0.4*sin(th2);
    }
     sled(x1,y1,0);
    sled(x2,y2,th2);
    printf("c %e %e 0.1\n",xs,ys);
    printf("F\n");
  }
}

int main(void)
{
  while (1) loop();
}
