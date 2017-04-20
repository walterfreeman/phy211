#include "vector.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int N=6;


typedef struct charge
{
  int shape;
  vector pos;
  vector vel;
  double q;
  double r;
} charge;

charge *q;

void initialize_charges(void)
{
  N=12; // set number of charges
  q=(charge *)malloc(sizeof(charge)*N);
  
  for (int i=0;i<N;i++)
  {
    q[i].q = i%2*2-1; // alternate positive and negative charge
    q[i].vel.x = drand48()-0.5;
    q[i].vel.y = drand48()-0.5;
    q[i].vel.z = drand48()-0.5;
    q[i].pos.x = drand48()-0.5;
    q[i].pos.y = drand48()-0.5;
    q[i].pos.z = drand48()-0.5;
    q[i].r=0.02;
  }
  printf("S3 5\nS 2\n");
}


double pot(vector r,double &rm)
{
  rm=99999;
  vector sep;
  double V=0;
  double rad;
  int i;
  for (i=0;i<N;i++)
  {
    sep=vmadd(r,q[i].pos,-1);
    rad=vmag(sep);
    if (rad-q[i].r < rm && q[i].q<0) rm=rad-q[i].r;
    if (rad < q[i].r*0.99 && q[i].q<0) {return -99999;}
    V=V-q[i].q/rad;
  }
  return V;
}

vector field(vector r)
{
  vector F,sep;
  vclear(F);
  double rad;
  int i;
  for (i=0;i<N;i++)
  {
    sep=vmadd(r,q[i].pos,-1);
    rad=vmag(sep);
    vipmadd(F,sep,q[i].q/(rad*rad*rad));
  }
  return F;
}

void follow_field_line(vector r, int steps, double size)
{
  vector F,lastF;
  vclear(F);
  double V,dot,step,rm;
  for (int i=0;i<steps;i++)
  {
    lastF=F;
    F=field(r);
    vipm(F,1/vmag(F));
    dot=(lastF.x*F.x + lastF.y*F.y + lastF.z*F.z)*2;
    V=pot(r,rm);
    if (dot < 0.1) dot=0.1; 
    step=size*dot;
    if (step > rm) {step = rm;
      printf("l3 %e %e %e %e %e %e\n",r.x,r.y,r.z,r.x+F.x*step,r.y+F.y*step,r.z+F.z*step);
      break;}
    if (V == -99999) {break;}
    printf("C %e %e %e\n",1-V*8/N,1.0,1+V*8/N);
    printf("l3 %e %e %e %e %e %e\n",r.x,r.y,r.z,r.x+F.x*step,r.y+F.y*step,r.z+F.z*step);
    vipmadd(r,F,step);
  }

}

void draw_charges(void)
{
  for (int i=0;i<N;i++)
  {
    if (q[i].q > 0) printf("C 1 0 0\n"); else printf("C 0 0 1\n");
    printf("c3 %e %e %e %e\n",q[i].pos.x,q[i].pos.y,q[i].pos.z,q[i].r);
  }
}

void follow_all(int steps, double size, double density)
{
  vector r;
  for (int i=0;i<N;i++)
  {
    for (double ps=0;ps<M_PI+0.01;ps+=M_PI/(density))
    for (double th=0.1;th<2*M_PI+0.101;th+=2*M_PI/(density*sin(ps)))
    {
      
      if (q[i].q > 0)
      {
        r.x=q[i].pos.x+q[i].r*sin(th)*sin(ps);
        r.y=q[i].pos.y+q[i].r*cos(th)*sin(ps);
        r.z=q[i].pos.z+q[i].r*cos(ps);
        follow_field_line(r,steps,q[i].r);
      }
    }
  }
}

void draw_grid(void)
{
  printf("C 0.1 0.3 0.1\n");
  for (double x=-2;x<2.001;x+=0.1)
   {
     printf("l3 -2 %e 0 2 %e 0\n",x,x);
     printf("l3 %e -2 0 %e 2 0\n",x,x);
   }
}
int main(void)
{
  double density=5;
  vector r;
  initialize_charges();
  double t=0,sep;
  r.x=-1;
  r.y=-1;
  r.z=0;
  printf("start loop\n");
  printf("S3 100\n");
  printf("S 10\n");
  while (1==1)
  {
    for (int i=0;i<N;i++)
    {
      vipmadd(q[i].pos,q[i].vel,0.01);
      if (q[i].pos.x>1 || q[i].pos.x<-1) q[i].vel.x*=-1;
      if (q[i].pos.y>1 || q[i].pos.y<-1) q[i].vel.y*=-1;
      if (q[i].pos.z>1 || q[i].pos.z<-1) q[i].vel.z*=-1;
    }
    
    t+=0.04;
    draw_charges();
    //draw_grid();
    follow_all(4000,0.2,density);
    printf("F\n");
  }
}
