#include <stdio.h>
#include <math.h>

int main(void)
{
  for (double t=0; t<10; t+=0.01)
  {
    printf("%e %e\n",t,30+70*exp(-0.1*t));
  }
}
