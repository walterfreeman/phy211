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
  double th[N];
  double om[N];
  double thhalf,omhalf;
  double dt=4e-2,t;
  int frameskip=1000,i=0,j;

  for (j=0; j<N; j++)
  {
    th[j]=1+0.1*drand48();
    om[j]=0+0.1*drand48();
  }

  for (t=0; 1; t+=dt)
  {
    for (j=0; j<N; j++)
    {
    thhalf=th[j]+om[j]*dt/2;
    omhalf=om[j]+alpha(th[j])*dt/2;
    th[j]=th[j]+omhalf*dt;
    om[j]=om[j]+alpha(thhalf)*dt;
    }
    if (i % frameskip == 0)
    {
      for (j=0; j<N; j++)
      {
        printf("c %e %e 0.005\n",th[j],om[j]);
      }
      printf("F\n");
    }
    i++;
  }
}
 
