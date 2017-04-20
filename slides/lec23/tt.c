#include <stdio.h>

int main(void)
{
  double x=0, v=1, k=1;
  double dt=0.01,m=1,t;

  for (t=0;1;t+=dt)
  {
    x+=v*dt;
    v+=-k*x/m * dt;
    printf ("c %e %e,0.01\n",x,0.0);  // screw you, you anal compiler
    printf("F\n");
  }
}
