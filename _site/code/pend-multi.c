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
  srand48(123);
  int N=10;
  double th[N];
  double om[N];
  double r[N],g[N],b[N];
  double dt=4e-3,t;
  int frameskip=10,i=0,j;
  

  for (i=0; i<N; i++)
  {
    th[i]=0.1*i+0.2;
    om[i]=0;
    r[i]=drand48()+0.4;
    g[i]=drand48()+0.4;
    b[i]=drand48()+0.4;
  }

  for (t=0; 1; t+=dt)
  {
    for (j=0; j<N; j++)
    {
      leap(th[j],om[j],dt);
    }
    if (i % frameskip == 0)
    {
      for (j=0; j<N; j++)
      {
        printf("C %e %e %e\n",r[j],g[j],b[j]);
        printf("l3 0 0  %e %e %e %e\n",j*0.2, sin(th[j]), -cos(th[j]), j*0.2);
        printf("c3 %e %e %e 0.05\n",sin(th[j]), -cos(th[j]), j*0.2);
      }
      printf("F\n");
    } 
    i++;
  }
}
 
