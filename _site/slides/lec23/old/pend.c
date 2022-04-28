#include <stdio.h>
#include <math.h>

main()
{
  double theta = 0.5;
  double L=1;
  double g=9.8;
  double omega = 0;
  double t, dt=0.001;
  for (t=0; t<100; t += dt)
  {
    theta += omega*dt;
    omega -= sin(theta)*g/L;
    printf("l 0 0 %lf %lf\n",sin(theta)*(L-0.05),-cos(theta)*(L-0.05)); // draw a line
    printf("c %lf %lf 0.05\n",sin(theta)*L,-cos(theta)*L); // with a ball
    printf("F\n"); // Flush this to screen
  }
}
