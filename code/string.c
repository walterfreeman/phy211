#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define FLOAT_TYPE long double

FLOAT_TYPE t;

// #define ONE_PERIOD_ONLY // uncomment this to do only one period, then print stats and exit. 
void get_forces(FLOAT_TYPE *x, FLOAT_TYPE *y, FLOAT_TYPE *Fx, FLOAT_TYPE *Fy, int N, FLOAT_TYPE k, FLOAT_TYPE r0) // pointers here are array parameters
{
  int i;
  FLOAT_TYPE r;
  Fx[0]=Fy[0]=Fx[N]=Fy[N]=0; // have to set the end forces properly to avoid possible uninitialized memory shenanigans
  FLOAT_TYPE U=0,E=0,T=0;
 
  for (i=1;i<N;i++)
  {
    // left force
    r = hypot(x[i]-x[i-1],y[i]-y[i-1]);
    Fx[i] = -(x[i]-x[i-1]) * k * (r-r0)/r;
    Fy[i] = -(y[i]-y[i-1]) * k * (r-r0)/r;

    // right force
    r = hypot(x[i]-x[i+1],y[i]-y[i+1]);
    Fx[i] += -(x[i]-x[i+1]) * k * (r-r0)/r;
    Fy[i] += -(y[i]-y[i+1]) * k * (r-r0)/r;
  }
}

 void evolve_leapfrog(FLOAT_TYPE *x, FLOAT_TYPE *y, FLOAT_TYPE *vx, FLOAT_TYPE *vy, int N, FLOAT_TYPE k, FLOAT_TYPE m, FLOAT_TYPE r0, FLOAT_TYPE dt) 
{
  int i,min;

  FLOAT_TYPE Fx[N+1],Fy[N+1]; // this could be made faster by mallocing this once, but we leave it this way for students
                          // to avoid having to deal with malloc(). In any case memory allocation is faster than a bunch
                          // of square root calls in hypot().
  for (i=min;i<N;i++)
  {
    x[i] += vx[i]*dt/2;
    y[i] += vy[i]*dt/2;
  }
  get_forces(x,y,Fx,Fy,N,k,r0);
  for (i=min;i<N;i++)
  {
    vx[i] += Fx[i]/m*dt;
    vy[i] += Fy[i]/m*dt;
  }
  for (i=min;i<N;i++)
  {
    x[i] += vx[i]*dt/2;
    y[i] += vy[i]*dt/2;
  }
}


void evolve_euler(FLOAT_TYPE *x, FLOAT_TYPE *y, FLOAT_TYPE *vx, FLOAT_TYPE *vy, int N, FLOAT_TYPE k, FLOAT_TYPE m, FLOAT_TYPE r0, FLOAT_TYPE dt) 
{
  int i;


  FLOAT_TYPE Fx[N+1],Fy[N+1]; // this could be made faster by mallocing this once, but we leave it this way for students
                          // to avoid having to deal with malloc(). In any case memory allocation is faster than a bunch
                          // of square root calls in hypot().
  get_forces(x,y,Fx,Fy,N,k,r0);
  for (i=1;i<N;i++)
  {
    x[i] += vx[i]*dt;
    y[i] += vy[i]*dt;
    vx[i] += Fx[i]/m*dt;
    vy[i] += Fy[i]/m*dt;
  }
}

// this function is around to go from Euler-Cromer to leapfrog, if we want second-order precision
void evolve_velocity_half(FLOAT_TYPE *x, FLOAT_TYPE *y, FLOAT_TYPE *vx, FLOAT_TYPE *vy, int N, FLOAT_TYPE k, FLOAT_TYPE m, FLOAT_TYPE r0, FLOAT_TYPE dt)
{
  int i;

  FLOAT_TYPE Fx[N+1],Fy[N+1]; // this could be made faster by mallocing this once, but we leave it this way for students
  
  get_forces(x,y,Fx,Fy,N,k,r0);
  
  for (i=1;i<N;i++)
  {
    vx[i] += Fx[i]/m*dt/2;
    vy[i] += Fy[i]/m*dt/2;
  }
}

// Students might not be familiar with pass-by-reference as a trick for returning multiple values yet. 
// Ideally they should be coding this anyway, and there are a number of workarounds, in particular 
// just not using a function for this.
void get_energy(FLOAT_TYPE *x, FLOAT_TYPE *y, FLOAT_TYPE *vx, FLOAT_TYPE *vy, int N, FLOAT_TYPE k, FLOAT_TYPE m, FLOAT_TYPE r0, FLOAT_TYPE *E, FLOAT_TYPE *T, FLOAT_TYPE *U)
{
  *E=*T=*U=0;
  int i;
  FLOAT_TYPE r;
  for (i=0;i<N;i++)
  {
    *T+=0.5*m*(vx[i]*vx[i] + vy[i]*vy[i]);
    r = hypot(x[i]-x[i+1],y[i]-y[i+1]);
    *U+=0.5*k*(r-r0)*(r-r0);
  }
  *E=*T+*U;
}

// does what it says on the tin
void evolve_euler_cromer(FLOAT_TYPE *x, FLOAT_TYPE *y, FLOAT_TYPE *vx, FLOAT_TYPE *vy, int N, FLOAT_TYPE k, FLOAT_TYPE m, FLOAT_TYPE r0, FLOAT_TYPE dt)
{
  int i;
  FLOAT_TYPE Fx[N+1],Fy[N+1]; // this could be made faster by mallocing this once, but we leave it this way for students
  for (i=1;i<N;i++)
  {
    x[i] += vx[i]*dt;
    y[i] += vy[i]*dt;
  }
  
  get_forces(x,y,Fx,Fy,N,k,r0);
  
  for (i=1;i<N;i++)
  {
    vx[i] += Fx[i]/m*dt;
    vy[i] += Fy[i]/m*dt;
  }
}

// does what it says on the tin
void evolve_rk2(FLOAT_TYPE *x, FLOAT_TYPE *y, FLOAT_TYPE *vx, FLOAT_TYPE *vy, int N, FLOAT_TYPE k, FLOAT_TYPE m, FLOAT_TYPE r0, FLOAT_TYPE dt)
{
  int i;

  FLOAT_TYPE Fx[N+1],Fy[N+1]; // this could be made faster by mallocing this once, but we leave it this way for students
  FLOAT_TYPE xh[N+1],yh[N+1],vxh[N+1],vyh[N+1];
  vxh[0]=vyh[0]=vxh[N]=vyh[N]=0;
  get_forces(x,y,Fx,Fy,N,k,r0);

  for (i=0;i<=N;i++)
  {
    xh[i] = x[i] + vx[i]*dt/2;
    yh[i] = y[i] + vy[i]*dt/2;
    vxh[i] = vx[i] + Fx[i]/m*dt/2;
    vyh[i] = vy[i] + Fy[i]/m*dt/2;
  }
  
  get_forces(xh,yh,Fx,Fy,N,k,r0);
  
  for (i=0;i<=N;i++) // need two for loops -- can't interleave halfstep/fullstep updates (students have trouble with this sometimes!)
  {
    x[i] = x[i] + vx[i]*dt;
    y[i] = y[i] + vy[i]*dt;
    vx[i] = vx[i] + Fx[i]/m*dt;
    vy[i] = vy[i] + Fy[i]/m*dt;
  }
  
}

// function to encapsulate determining whether we need to shovel another frame to the animator. delay is the delay in msec.
// I usually aim to hide library calls, like clock() and CLOCKS_PER_SEC, from students in the beginning, since it's not really
// relevant to their development of computational skills, which are what I really care about.
int istime(int delay)
{
  static int nextdraw=0;
  if (clock() > nextdraw)
  {
    nextdraw = clock() + delay * CLOCKS_PER_SEC/1000.0;
    return 1;
  }
  return 0;
}

int main(int argc, char **argv)
{
  int i,N=80; //  number of links, not number of nodes!! Careful for the lurking fencepost errors
  int modenumber=3; // put in some defaults just in case 
  FLOAT_TYPE dt=2e-6, amplitude=0.1;
  FLOAT_TYPE stiffness=10, density=1, length=1; // unstretched properties of original string
  FLOAT_TYPE k, m, r0; // properties of single string
  FLOAT_TYPE tension=1,Ls;
  int frame=0, frameskip;
  FLOAT_TYPE modeamp=0, modeamplast=0, modeamplastlast=0;

  if (argc < 10) // if they've not given me the parameters I need, don't just segfault -- tell the user what to do, then let them try again
  {
    printf("!Usage: <this> <N> <modenumber> <dt> <amplitude> <stiffness> <density> <length> <tension> <frameskip>\n"); 
    exit(0);
  }
  N=atoi(argv[1]);
  modenumber=atoi(argv[2]);
  dt=atof(argv[3]);
  amplitude=atof(argv[4]);
  stiffness=atof(argv[5]);
  density=atof(argv[6]);
  length=atof(argv[7]);
  tension=atof(argv[8]);
  frameskip=atof(argv[9]);

  FLOAT_TYPE x[N+1], y[N+1], vx[N+1], vy[N+1], E, T, U;

  // compute microscopic properties from macroscopic ones 

  r0=length/N;
  m=density*length/N;
  k=stiffness*N/length;

  // figure out stretched length

  Ls=length + tension * length / stiffness;

  // make predictions based on what our freshman mechanics class taught us

  FLOAT_TYPE density_stretched = density * length / Ls;
  FLOAT_TYPE wavespeed = sqrt(tension/density_stretched);
  FLOAT_TYPE period_predict = 2 * Ls / wavespeed / modenumber;
  FLOAT_TYPE vym_last=0;

  int monitor_node = N/modenumber/2; // this is the node that we'll be watching to see when a period has elapsed.
  int nperiods=0;  

  for (i=0;i<=N;i++) // remember, we have N+1 of these
  {
    x[i] = Ls*i/N;
    y[i] = amplitude*sin(modenumber * M_PI * x[i] / Ls);
    vx[i]=0;
    vy[i]=0;
  }

  // now, loop over time forever...
  printf("!N\tmode\tstiffness\tL\t\tamp.\t\tdens.\tTension\tT\tT_pred\tDelta\t\tFreq (Hz)\tFreq_pred\n"); // print header
  for (t=0;1;t+=dt)
  {
    modeamplastlast=modeamplast;
    modeamplast=modeamp;
    modeamp=0;
    for (i=1; i<N; i++)
    {
      modeamp += y[i] * sin(modenumber * M_PI * x[i] / Ls);
    }
    vym_last=vy[monitor_node];
    evolve_leapfrog(x,y,vx,vy,N,k,m,r0,dt);
    
    // "if we were going up, but now we're going down, then a period is complete"
    // this is crude and will fail if it has enough "wobble", but it's sufficient for this project. 
    if (modeamp-modeamplast < 0 && modeamplast-modeamplastlast > 0 && frame>5)
    {
      nperiods++;
      printf("!%d\t%d\t%.2Le\t%.2Le\t%.4Le\t%.1Le\t%.1Le\t%.4Lf\t%.4Lf\t%.4Le\t%.4Le\t%.4Le\n",N,
modenumber,stiffness,length,amplitude,density,tension,t,period_predict*nperiods,1-t/period_predict/nperiods,nperiods/t,1.0/period_predict);
    #ifdef ONE_PERIOD_ONLY
      printf("Q\n"); // kill anim before we die ourselves
      exit(1);       
    #endif
    }
    frame++; 
  //  if (frame % frameskip == 0) // wait 15ms between frames sent to anim; this gives us about 60fps.
    if (istime(15))
    {
      printf("C 1 0 0\nc %Lf %Lf 0.02\nC 1 1 1\n",x[monitor_node],y[monitor_node]); // draw a big red blob around the node we're watching
      for (i=0;i<=N;i++)
      {
        printf("C %Le 0.5 %Le\n",0.5+y[i]/amplitude,0.5-y[i]/amplitude); // use red/blue shading; this will make vibrations visible even if amp<<1
	printf("c %Lf %Lf %Lf\n",x[i],y[i],length/N/3); // draw circles, with radius scaled to separation
	if (i<N) printf("l %Lf %Lf %Lf %Lf\n",x[i],y[i],x[i+1],y[i+1]); // the if call ensures we don't drive off the array
      }
      printf("T -0.5 -0.7\ntime = %Lf\n",t); 
      get_energy(x,y,vx,vy,N,k,m,r0,&E,&T,&U);
      printf("T -0.5 -0.63\nenergy = %Le + %Le = %Le\n",T,U,E);
      printf("F\n"); // flush frame
    }
  }
} 
