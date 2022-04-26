#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#define N 300

int delay=800;
double K=1;
double T=0.1;
double M=1;
double dt=0.00004;

void va(double x, double y, double x2, double y2)
{

  double xo,x3,x4,x5;
  double yo,y3,y4,y5;
                 printf("l %e %e %e %e\n",x,y,x2,y2);
                     xo=-(y2-y);
                         yo=(x2-x);
                         x3=x2*0.85+x*0.15 + xo*0.07;
                         x4=x2*0.85+x*0.15 - xo*0.07;
                         y3=y2*0.85+y*0.15 + yo*0.07;
                         y4=y2*0.85+y*0.15 - yo*0.07;
                         x5=x2*0.92+x*0.08;
                         y5=y2*0.92+y*0.08;
                         printf("l %e %e %e %e\n",x2,y2,x3,y3); 
                         printf("l %e %e %e %e\n",x2,y2,x4,y4); 
                         printf("l %e %e %e %e\n",x3,y3,x5,y5); 
                         printf("l %e %e %e %e\n",x4,y4,x5,y5);
}


int main(int argc,char **argv)
{

  printf("center 0.5 0 0\nS 1\n");
  double pw;
  int n=atoi(argv[1]);
  printf("!%dth harmonic\n",n);
  int i,j,f=0;
  double x[N+1],y[N+1],vx[N+1],vy[N+1],m[N+1],xh[N+1],yh[N+1],vxh[N+1],vyh[N+1],t,fx[N+1],fy[N+1];
  double k=K*N;
  double yegg=1e5;
  double xmax=1+K*T;
  double r,r0=1./N;
  double F,Fx,Fy;
  for (i=0;i<N+1;i++)
  {
    m[i]=M/N;
    x[i]=i*xmax/N;
    y[i]=0;
    vx[i]=0;
    vy[i]=0.2*n/yegg*sin(n*M_PI*(double)(i)/N);
  }
  for (t=0;1;t+=dt)
  {
//    y[0]=yh[0]=0.000001*exp(-(t-2)*(t-2)*pw);
    for (i=1;i<N+1;i++)
    {
      xh[i]=x[i]+vx[i]*dt/2;
      yh[i]=y[i]+vy[i]*dt/2;
      r=hypot(x[i]-x[i-1],y[i]-y[i-1]);
      F=-k*(r-r0);
      Fx=F*(x[i]-x[i-1])/r;
      Fy=F*(y[i]-y[i-1])/r;
      vxh[i]=vx[i]+Fx/m[i]*dt/2;
      vyh[i]=vy[i]+Fy/m[i]*dt/2;
      if (i>0)
      {
      vxh[i-1]=vxh[i-1]-Fx/m[i-1]*dt/2;
      vyh[i-1]=vyh[i-1]-Fy/m[i-1]*dt/2;
      }
   }
    for (i=1;i<N+1;i++)
    {
      fx[i]=fy[i]=0;
      if (i<N)
      {
        x[i]=x[i]+vxh[i]*dt;
        y[i]=y[i]+vyh[i]*dt;
      }
      r=hypot(xh[i]-xh[i-1],yh[i]-yh[i-1]);
      F=-k*(r-r0);
      Fx=F*(xh[i]-xh[i-1])/r;
      Fy=F*(yh[i]-yh[i-1])/r;
      fx[i]=Fx;
      fy[i]=Fy;
      if (i<N)
      {
      vx[i]=vx[i]+Fx/m[i]*dt;
      vy[i]=vy[i]+Fy/m[i]*dt;
      }
      if (i>1){
      vx[i-1]=vx[i-1]-Fx/m[i-1]*dt;
      vy[i-1]=vy[i-1]-Fy/m[i-1]*dt;
      fx[i-1]-=Fx;
      fy[i-1]-=Fy;
      }
    }

    if (f++%delay==0)
    {
    
    for (i=0;i<N;i++)
    {
//    printf("C 0 0.5 0\n");
//      va(x[i],y[i]*yegg,x[i]+fx[i]*3,(y[i]+fy[i]*3)*yegg);
//    printf("C .3 .3 1\n");
//      va(x[i],y[i]*yegg,x[i]+vx[i]*1,(y[i]+vy[i]*1)*yegg);
    }
     printf("C 1 0 0\n");
    for (i=0;i<N;i++)
    {
      printf("l %e %e %e %e\n",x[i],y[i]*yegg,x[i+1],y[i+1]*yegg);
    }
    printf("C 1 1 1\n");
    for (i=0;i<N+1;i++)
    {
      printf("c %e %e %e\n",x[i],y[i]*yegg,0.1/N);
    }
   
    
  printf("T 0 0.8\nt=%.3g\n",t);
  printf("F\n");
    }
  }
}
