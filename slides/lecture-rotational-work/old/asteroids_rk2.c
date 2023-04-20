#include<stdio.h>
#include<math.h>
#include <time.h>
#include<stdlib.h>
#define N 800
#define FRAMESKIP 30
double m_sol = 1;

void wait ( double seconds )
{
  clock_t endwait;
  endwait = clock () + seconds * CLOCKS_PER_SEC ;
  while (clock() < endwait) {}
}



main()
{
  
  double m[N], vx[N], vy[N], x[N], y[N], vxm[N], vym[N], xm[N], ym[N], xmid[N], ymid[N], vxmid[N], vymid[N];
  double G = 4*M_PI*M_PI;
  double dt=0.02,L,e,E;
  double t,ax,ay;
  double r;
  int i,j;
  int s=0;
  double a;
  double cd=100;
  // Sun
  m[0] = 1;
  vx[0] = 0;
  vy[0] = 0;
  x[0]=0;
  y[0]=0;

  // Jupiter -- data from wikipedia
  m[1] = 1./1047.;
  x[1] = 5.20;
  y[1] = 0;
  vx[1] = 0;
  vy[1] = (x[1]*2*M_PI)/11.85;
  
  printf("S 10\nFPS 0\n"); // we want our box to be 6 AU big, and we want to draw no more than 40 FPS

  srand48(1); // seed random number generator

  // scatter a crapload of asteroids everywhere
  for (i=2;i<N;i++)
    {
      m[i]=1;
      r=1+i*.004;

      x[i]=r;
      y[i]=0;
      vx[i]=0;
      vy[i]=2*M_PI/sqrt(r);
    }

  for (t=0; t<100000;t +=dt)
  {
    if (dt < 0.002) dt *= 1.001;
    if (dt > cd/100) dt=cd/100;
    s++;
  //    fprintf(stderr,"Timestep %d.\n",s);
      if (s%FRAMESKIP == 0){ // do this only every 2000th frame
         printf("C 1 1 1\n");
    //     fprintf(stderr,"Sending frame %d to anim.\n",s);
    //     printf("!Frame %d received by anim.\n",s);

         // draw planets
         for (i=0;i<N;i++)
         {
           if (i > 1) printf("c %f %f 0.02\n",x[i],y[i]);  // draw the planet
             else {
               printf("c %f %f 0.2\n",x[i],y[i]);     // but if it's Jupiter, make it huge!
               if (i==1) printf("t %lf %lf\nJupiter\n",x[i]+.08,y[i]+.08);
        }
         } // end loop over planets

         printf("C 0.5 0.25 0.1\n");
         for (i=0; i<N; i++) // make plot of eccentricity vs. radius
         {
           L = x[i]*vy[i]-y[i]*vx[i];         // cross product: L = v x r
           r=sqrt(x[i]*x[i]+y[i]*y[i]);
           E = -4*M_PI*M_PI/r + 0.5*(vx[i]*vx[i] + vy[i]*vy[i]); // total energy
           e = sqrt(1+2*E*L*L/(16*M_PI*M_PI*M_PI*M_PI));         // eccentricity
           printf("d %lf %lf 0.02\n",-10+r*4,e*3);
         }  // end loop over planets
     
         // print time
         printf("C 1 1 1\n");
         printf("T 0 -.95\n");
         printf("%.2g y \n",t);
         printf("T 0 .95\n");
         printf("rmin %.3g dt %.3g\n",cd,dt);

         printf("F\n");
         fflush(stdout); // update now, dammit (useful on slow internet connection)
      } // end animation output
     
      

    // Euler half-step
    for (i=1;i<N;i++)  // i counts the planet we are moving -- don't move the sun 
                       // (it'll drift a bit over time since V_COM != 0)
    { 
      ax=ay=0;
    
      for (j=0;j<2;j++)  // j counts the planet whose gravity we care about
      {
        if (i==j) continue; // no self-gravity (that'd be cool!)
	r=sqrt((x[i]-x[j])*(x[i]-x[j]) + (y[i]-y[j])*(y[i]-y[j])); // distance from i to j
        ax += (x[j]-x[i])*G*m[j]/(r*r*r);
        ay += (y[j]-y[i])*G*m[j]/(r*r*r);
      } // end loop over j

      vxmid[i] = vx[i] + ax*dt*.5; // update velocities based on acceleration
      vymid[i] = vy[i] + ay*dt*.5; 
    } // end loop over i    

    for (i=0;i<N;i++) // move the i'th planet
    {
      xmid[i] = x[i] + vx[i]*dt*.5;
      ymid[i] = y[i] + vy[i]*dt*.5;
    }
    cd=1000;
    // RK2 full-step

    for (i=1;i<N;i++)  // i counts the planet we are moving -- don't move the sun
                       // (it'll drift a bit over time since V_COM != 0)
    {
      ax=ay=0;

      for (j=0;j<2;j++)  // j counts the planet whose gravity we care about
      {
        if (i==j) continue; // no self-gravity (that'd be cool!)
        r=sqrt((xmid[i]-xmid[j])*(xmid[i]-xmid[j]) + (ymid[i]-ymid[j])*(ymid[i]-ymid[j])); // distance from i to j
        if (r<cd) cd=r;
        ax += (xmid[j]-xmid[i])*G*m[j]/(r*r*r);
        ay += (ymid[j]-ymid[i])*G*m[j]/(r*r*r);
      } // end loop over j

      vx[i] += ax*dt; // update velocities based on acceleration
      vy[i] += ay*dt;
    } // end loop over i

    for (i=0;i<N;i++) // move the i'th planet
    {
      x[i] += vxmid[i]*dt;
      y[i] += vymid[i]*dt;
    }
    
  
    
  }
}


