#include <stdio.h>
#include <math.h>

double t=0;
double dt=0.0002;
int draw=20,cache=60;
int cd=500;
double C=1;
double cdist=.4;
double extra_energy=0.5;


double V(double r)
{
  return -1./r * 4*M_PI*M_PI + C/(r*r);
}
double F(double r)
{
  return -(V(r+.000001) - V(r-0.000001)) / (0.000002);
}


int main(void)
{
  double x,y,vx,vy,xh,yh,vxh,vyh,r,fmag;
  double dth=dt/2;
  x=1; y=0; vx=0; vy=sqrt(4*M_PI*M_PI-2*C)*extra_energy;
  int f=0,i,cloc=0,a1,a2;
  double cx[cd],cy[cd];
  double L;
  double xmin=10,xmax=0;
  double Vee,Vp,LVp,LV,Vs=0.1;
  double maxr=3.5,tone;
  double st=0.015;
  double fade=0;
  printf("S %e\n",maxr*1.2);

  for (i=0;i<cd;i++) {cx[i]=cy[i]=0;}

  while (1==1)
  {
    f++;
    xh = x+vx*dth;
    yh = y+vy*dth;
    r=hypot(x,y); fmag=F(r);
    vxh = vx + fmag*(x/r) * dth;
    vyh = vy + fmag*(y/r) * dth;

    x = x + vxh * dt;
    y = y + vyh * dt;
    r=hypot(xh,yh); fmag=F(r);
    vx = vx + fmag*(xh/r) * dt;
    vy = vy + fmag*(yh/r) * dt;

    if (r<xmin) xmin=r; if (r>xmax) xmax=r;

    t += dt;

    if (f%draw == 0)
    {
      
      
      r=hypot(x,y);
      L=x*vy-y*vx;


      Vee=V(r); Vp=Vee + L*L/(r*r*2);
      printf("C 1 1 1\n");
      printf("c %e %e 0.03\n",r,Vp*Vs);
      printf("C 0.4 0 0.4\n");
      printf("l %e %e %e %e\n",xmin,(Vee+0.5*vx*vx+0.5*vy*vy)*Vs,xmax,(Vee+0.5*vx*vx+0.5*vy*vy)*Vs);
      printf("C 0.7 0.7 0\n");
      printf("l %e %e %e %e\n",r,Vp*Vs,r,Vee*Vs);

      LV=V(maxr*st*5); 
      LVp=LV + L*L/(maxr*maxr*st*st*2*25); 
   
      for (r=(maxr*st*6);r<maxr;r+=maxr*st)
      {
        L=x*vy-y*vx;
        Vee=V(r); Vp=Vee + L*L/(r*r*2);
        printf("C 0 1 0\n");
        printf("l %e %e %e %e\n",r,Vee*Vs,r-st*maxr,LV*Vs);
        printf("C 0 0 1\n");
        printf("l %e %e %e %e\n",r,Vp*Vs,r-st*maxr,LVp*Vs);
        LV=Vee; LVp=Vp;
      }

     for (i=2; i<cd; i++)
      {
        tone=(double)i/cd;
        if (tone > (1-fade)) {tone=(1-tone)/fade;} else {tone /= (1-fade);}
        printf("C %e 0 0\n",tone);
        a1=(i+cloc)%cd;
        a2=(i-1+cloc+cd)%cd;
        printf("l %e %e %e %e\n",cx[a1],cy[a1],cx[a2],cy[a2]);
      } 
      printf("C 1 1 1\n");printf("c 0 0 0.1\n");
      printf("c %e %e 0.08\n",x,y);

      printf("F\n");
    }

    if (hypot(x-cx[cloc],y-cy[cloc])>cdist)
    {
      cloc = (cloc+1)%cd;
      cx[cloc]=x;
      cy[cloc]=y;
    }
    
   

  }
}
