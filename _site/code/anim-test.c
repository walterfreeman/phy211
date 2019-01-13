#include <stdio.h>
#include <math.h>

int main()
{
  double theta;
  for (theta=0; theta<200000; theta = theta + 0.01)
  {
    printf("l 0 0 %e %e\n",sin(theta),cos(theta));
    printf("c %e %e 0.1\n",sin(theta),cos(theta));
    printf("F\n");
  }
}
