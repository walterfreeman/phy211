#include <stdio.h>
#include <math.h>
#define FRAMESKIP 15 

double hypot3(double x,double y, double z)
{
  return sqrt(x*x+y*y+z*z);
}

int main()
{
  printf("FPS 50\n");
  int N=12;
  double x[N+1][N+1];
  double y[N+1][N+1];
  double z[N+1][N+1];
  double vx[N+1][N+1];
  double vy[N+1][N+1];
  double vz[N+1][N+1];
  double xmid[N+1][N+1];
  double ymid[N+1][N+1];
  double zmid[N+1][N+1];
  double vxmid[N+1][N+1];
  double vymid[N+1][N+1];
  double vzmid[N+1][N+1];
  double m;
  double vlast;
  double k;
  double L=2;
  double T=1.5;
  int i,j,f=0;
  double dt=0.0000,t;
  int periods=0;
  double c;
  printf("S 1.6\n"); 
  // make string go from 0 to L

  for (i=0;i<N+1;i++) for (j=0;j<N+1;j++)
    {
    x[i][j]=L/N*i-L/2;
    y[i][j]=L/N*j-L/2;
    vx[i][j]=vy[i][j]=vz[i][j]=0;
    z[i][j] = pow(-1,j+i)*.0; 

    xmid[i][j]=x[i][j];     ymid[i][j]=y[i][j]; zmid[i][j]=z[i][j];

    

  //  if (x[i] > 0) y[i]=0;
  }

  double massperlength = 1;
  double stiffness_per_length = 20;
  double r0,r,F;
  c = sqrt(T/massperlength);
  printf("!Speed of sound: %g.\n",c);
  printf("!Speed of information: %g.\n",L/N/dt);
  double damp=0.02;
  k = stiffness_per_length / L * N;
  m = massperlength * L / N;
  
  // decide on "unstretched" length here
  // if we have N-1 segments of spring constant k, then
  // the tension is (N-1)(L-L0)k, so L-L0=T/((N-1)k)

  r0 = L/N - T/k;  // dimensions: force over (force per length) 
  printf("!Eq length: %f Real length: %f.\n",r0,L/N);
  for (t=0;1==1;t+=dt)
  { f++;
    for (i=1;i<N;i++) for (j=1;j<N;j++)// rk2 halfstep
    {
      // left side
      r=hypot3(x[i-1][j]-x[i][j],y[i-1][j]-y[i][j],z[i-1][j]-z[i][j]);
      F=k*(r-r0);
      vxmid[i][j] = vx[i][j]+F/m*(x[i-1][j]-x[i][j])/r*dt/2;
      vymid[i][j] = vy[i][j]+F/m*(y[i-1][j]-y[i][j])/r*dt/2;
      vzmid[i][j] = vz[i][j]+F/m*(z[i-1][j]-z[i][j])/r*dt/2;

// right side
      r=hypot3(x[i+1][j]-x[i][j],y[i+1][j]-y[i][j],z[i+1][j]-z[i][j]);
      F=k*(r-r0);
      vxmid[i][j]+= F/m*(x[i+1][j]-x[i][j])/r*dt/2;
      vymid[i][j]+= F/m*(y[i+1][j]-y[i][j])/r*dt/2;
      vzmid[i][j]+= F/m*(z[i+1][j]-z[i][j])/r*dt/2;
// frontside
      r=hypot3(x[i][j+1]-x[i][j],y[i][j+1]-y[i][j],z[i][j+1]-z[i][j]);
      F=k*(r-r0);
      vxmid[i][j]+= F/m*(x[i][j+1]-x[i][j])/r*dt/2;
      vymid[i][j]+= F/m*(y[i][j+1]-y[i][j])/r*dt/2;
      vzmid[i][j]+= F/m*(z[i][j+1]-z[i][j])/r*dt/2;

// back side
     
r=hypot3(x[i][j-1]-x[i][j],y[i][j-1]-y[i][j],z[i][j-1]-z[i][j]);
      F=k*(r-r0);
      vxmid[i][j]+= F/m*(x[i][j-1]-x[i][j])/r*dt/2;
      vymid[i][j]+= F/m*(y[i][j-1]-y[i][j])/r*dt/2;
      vzmid[i][j]+= F/m*(z[i][j-1]-z[i][j])/r*dt/2;


    
      // move masses
      xmid[i][j] = x[i][j]+vx[i][j]*dt/2;
      ymid[i][j] = y[i][j]+vy[i][j]*dt/2;
     zmid[i][j] = z[i][j]+vz[i][j]*dt/2;
      
      
    //  printf("!Halfstep position of mass %d: %f,%f\n",i,xmid[i],ymid[i]);
    }
    for (i=1;i<N;i++) for (j=1;j<N;j++) // rk2 fullstep
    {
      // left side
      r=hypot3(xmid[i-1][j]-xmid[i][j],ymid[i-1][j]-ymid[i][j],zmid[i-1][j]-zmid[i][j]);
      F=k*(r-r0);
      vx[i][j] = vx[i][j]+F/m*(xmid[i-1][j]-xmid[i][j])/r*dt;
      vy[i][j] = vy[i][j]+F/m*(ymid[i-1][j]-ymid[i][j])/r*dt;
      vz[i][j] = vz[i][j]+F/m*(zmid[i-1][j]-zmid[i][j])/r*dt;

// left side
      r=hypot3(xmid[i+1][j]-xmid[i][j],ymid[i+1][j]-ymid[i][j],zmid[i+1][j]-zmid[i][j]);
      F=k*(r-r0);
      vx[i][j]+= F/m*(xmid[i+1][j]-xmid[i][j])/r*dt;
      vy[i][j]+= F/m*(ymid[i+1][j]-ymid[i][j])/r*dt;
      vz[i][j]+= F/m*(zmid[i+1][j]-zmid[i][j])/r*dt;
// frontside
      r=hypot3(xmid[i][j+1]-xmid[i][j],ymid[i][j+1]-ymid[i][j],zmid[i][j+1]-zmid[i][j]);
      F=k*(r-r0);
      vx[i][j]+= F/m*(xmid[i][j+1]-xmid[i][j])/r*dt;
      vy[i][j]+= F/m*(ymid[i][j+1]-ymid[i][j])/r*dt;
      vz[i][j]+= F/m*(zmid[i][j+1]-zmid[i][j])/r*dt;

// back side
     
      r=hypot3(xmid[i][j-1]-xmid[i][j],ymid[i][j-1]-ymid[i][j],zmid[i][j-1]-zmid[i][j]);
      F=k*(r-r0);
      vx[i][j]+= F/m*(xmid[i][j-1]-xmid[i][j])/r*dt;
      vy[i][j]+= F/m*(ymid[i][j-1]-ymid[i][j])/r*dt;
      vz[i][j]+= F/m*(zmid[i][j-1]-zmid[i][j])/r*dt;


    
      // move masses
      x[i][j] = x[i][j]+vxmid[i][j]*dt;
      y[i][j] = y[i][j]+vymid[i][j]*dt;
 z[i][j] = z[i][j]+vzmid[i][j]*dt;

}


    if (f%FRAMESKIP == 0)
    {
      for (i=0;i<N;i++) for (j=0;j<N;j++)
      {
//           printf("C %f 0.8 %f\n",0.8+3*z[i][j],0.8-3*z[i][j]);
	   printf("l3 %f %f %f %f %f %f\n",x[i][j],  y[i][j],z[i][j], x[i+1][j],y[i+1][j],z[i+1][j]);
	   printf("l3 %f %f %f %f %f %f\n",x[i][j],  y[i][j],z[i][j], x[i][j+1],y[i][j+1],z[i][j+1]);
      }
 
for (i=0;i<N;i++)
{
  printf("l3 %f %f %f %f %f %f\n",x[i][N],  y[i][N],z[i][N], x[i+1][N],y[i+1][N],z[i+1][N]);
    printf("l3 %f %f %f %f %f %f\n",x[N][i],  y[N][i],z[N][i], x[N][i+1],y[N][i+1],z[N][i+1]);
}
      printf("C 1 1 1\n");
      printf("F\n");
    }
  }
}
