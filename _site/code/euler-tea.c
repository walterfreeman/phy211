#include <stdio.h>
#include <math.h>

int main(void)
{
  double dt=1,t;
  double Ta=30;
  double T=100;
  double k=0.1;
  
  for (t=0; t<=10; t+=dt)
  {
    printf("%e %e\n",t,T);
    T = T - k*(T-Ta)*dt;
     
  }
  printf("%e %e\n",t,T);
}
