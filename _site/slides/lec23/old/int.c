#include<stdio.h>
#include<math.h>

double f(double x)
{
  return exp(-x*x);
}

main()
{
  double x, i, dx=0.000001;
  for (x=5; x<9;x+=dx)
    {
      i+=f(x)*dx;
    }
  printf("%le\n",i);
}
