#include <stdio.h>
#include <math.h>

int main(void)
{
  double dt=2,t;
  double Ta=30;
  double T=100;
  double Thalf;
  double k=0.1;
  for (t=0; t<10; t+=dt)
  {
    printf("%e %e\n",t,T);
    Thalf = T - k*(T-Ta)*dt/2;
    T = T - k*(Thalf-Ta)*dt;
     
  }
  printf("%e %e\n",t,T);
}
