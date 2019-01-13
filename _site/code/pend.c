#include <stdio.h>
#include <math.h>
#include <stdlib.h>

double alpha (double th)
{
  return -sin(th);
}

void euler (double &th, double &om, double dt)
{
  double thnew;
  thnew = th + om * dt;
  om += alpha(th) * dt;
  th=thnew;
}

void eulercromer (double &th, double &om, double dt)
{
  th = th + om * dt;
  om += alpha(th) * dt;
}

void leap (double &th, double &om, double dt)
{
  th = th + om * dt/2;
  om += alpha(th) * dt;
  th = th + om * dt/2;
}



void rk2 (double &th, double &om, double dt)
{
  double thhalf, omhalf;
  thhalf=th+om*dt/2;
  omhalf=om+alpha(th)*dt/2;
  th += omhalf*dt;
  om += alpha(thhalf)*dt;
}

double E(double th, double om)
{
  return 0.5*om*om-cos(th);
}

int main(void)
{
  int N=1000;
  double th;
  double om;
  double dt=4e-4,t;
  int frameskip=1000,i=0,j;

  th=1; om=0;

  for (t=0; 1; t+=dt)
  {
    for (j=0; j<N; j++)
    {
      eulercromer(th,om,dt);
    }
    if (i % frameskip == 0)
    {
      for (j=0; j<N; j++)
      {
        printf("l 0 0 %e %e\n",sin(th), -cos(th));
      }
      printf("F\n");
      printf("!%e %e\n",t,E(th,om)-E(1,0));
    } 
    i++;
  }
}
 
