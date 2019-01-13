#include <stdio.h>
#include <math.h>


int main(void)
{
  int N=2;
  double x[N],y[N];
  double vx[N],vy[N];
  double m[N];

  m[0]=0.5; x[0]=0; y[0]=0; vx[0]=0; vy[0]=0;
  m[1]=0.5; x[1]=1; y[1]=0; vx[1]=0; vy[1]=2*M_PI;
  m[2]=0.01; x[2]=1.1; y[2]=0; vx[2]=0; vy[2]=4*M_PI;

  int i,j;
  double t;
  double dt=1e-5;
  double r;
  double G=4*M_PI*M_PI;
  double px, py,mt;
  double drag = 0.01;
  double T, U, E0;
  for (i=0; i<N; i++)
  {
    mt+=m[i];
    px+=m[i]*vx[i];
    py+=m[i]*vy[i];
  }
  for (i=0; i<N; i++)
  {
    vx[i]-=px/mt;
    vy[i]-=py/mt;
  }
  int frame=0,skip=200;

  for (t=0; 1; t+=dt)
  {
    if (frame % skip == 0)
    {
      printf("T -0.5 -0.5\nTotal energy: %e\n",T+U);
      printf("T -0.5 -0.55\nEnergy lost: %e\n",E0-(T+U));
      for (i=0; i<N; i++)
      {
        printf("c %e %e %e\n",x[i],y[i],pow(m[i],0.3)*0.03);
      }
      printf("F\n");
    }
    T=U=0;
    frame++;
    for (i=0; i<N; i++)
    {
      x[i]+=vx[i]*dt/2;
      y[i]+=vy[i]*dt/2;
    }
    for (i=0; i<N; i++)
      for (j=i+1; j<N; j++)
      {
        r=hypot(x[i]-x[j],y[i]-y[j]);
        vx[i]-=G*(x[i]-x[j])*m[j]/(r*r*r)*dt;
        vy[i]-=G*(y[i]-y[j])*m[j]/(r*r*r)*dt;
        vx[j]+=G*(x[i]-x[j])*m[i]/(r*r*r)*dt;
        vy[j]+=G*(y[i]-y[j])*m[i]/(r*r*r)*dt;
        U = U - G * m[i] * m[j]/r;
      }
    for (i=0; i<N; i++)
    {
      T = T + 0.5 * m[i] * (vx[i]*vx[i] + vy[i] * vy[i]);
      vx[i] = vx[i] - (drag * vx[i] * hypot(vx[i],vy[i])) * dt;
      vy[i] = vy[i] - (drag * vy[i] * hypot(vx[i],vy[i])) * dt;
    }
    for (i=0; i<N; i++)
    {
      x[i]+=vx[i]*dt/2;
      y[i]+=vy[i]*dt/2;
    }
    if (E0 == 0) E0=T+U;
  }
}
