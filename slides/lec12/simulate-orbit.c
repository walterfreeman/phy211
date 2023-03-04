#include <stdio.h>
#include <math.h>
#include "vector.h"

double GM=4*M_PI*M_PI; // G has a value of 4pi^2 in natural units for the Solar System
double step=0.002;      // step forward 0.01 year at a time
double earth_mass=1e-6;

// A function to draw the planet and Sun using an external tool
void draw_planet(vector position)
{
  printf("C 0.5 0.5 1\n");
  printf("ct3 0 %e %e %e %e\n",position.x,position.y,position.z,0.05);
  printf("C 1 0.5 0.5\n");
  printf("c3 0 0 0 0.1\n");
  printf("F\n");
}

int main(void)
{
  FILE *power, *totalwork, *radiusfile, *kineticenergy;
  kineticenergy=fopen("kinetic_energy","w");
  power=fopen("power","w");
  totalwork=fopen("total_work","w");
  radiusfile=fopen("radius","w");
  // these lines just tell the computer what quantities it will need to keep track of and calculate
  vector earth_position(1,0,0);                             
  vector earth_velocity(0,7.8,0);
  vector gravitational_force;
  double radius;
  vector direction;
  double power_current;
  double work_total=0;
  double t=0;
  printf("font large\n");
  while (1)
  {
	  t+=step;
    radius = mag(earth_position);                           // calculate the distance from Earth to Sun
    direction = -1 * earth_position/radius;                 // calculate the direction gravity goes (back toward Sun
    
    gravitational_force = GM*earth_mass / (radius*radius) * direction; // Newton's law of gravity
    power_current = (gravitational_force * earth_velocity);
    work_total += power_current * step;
    fprintf(kineticenergy,"%e %e\n",t,0.5*earth_mass*earth_velocity*earth_velocity);
    fprintf(radiusfile,"%e %e\n",t,radius*1e-5);
    fprintf(totalwork,"%e %e\n",t,work_total);
    fprintf(power,"%e %e\n",t,power_current);
    earth_velocity += gravitational_force/earth_mass * step;           // acceleration is rate of change of velocity
    earth_position += earth_velocity * step;                // velocity is rate of change of position
    printf("T -0.8 -0.8\nPower (rate of work) : %.5e\n",power_current);
    printf("T -0.8 -0.85\nTotal work done      : %.5e\n",work_total);
    draw_planet(earth_position);                            // draw the Earth and Sun on the screen
  }
}

