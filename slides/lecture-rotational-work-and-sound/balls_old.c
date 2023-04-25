#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define N 2000
#define SKIP 15
#define SKIP2 30
#define NH 600
#define NG 100


struct list_el {
   int val;
   struct list_el * next;
};

typedef struct list_el item;


double g=-3;
double vm=.4;
double em=.6;
double hscale,wscale;

int getzone(double x)
{
  int z;
  z=(int)((x+1)*0.5*NG);
  if (z>=NG) z=NG-1;
  if (z<0) z=0;
  return z;
}

void add(item **list, int v)
{
//   printf("!Adding item %d\n",v);
   item *curr;
   curr=(item *)malloc(sizeof(item));
   curr->val = v;
   curr->next = *list;
   *list=curr;
}

void del (item **list, int v)
{
  item *curr;
  curr=*list;
  if (curr == NULL) {return;}
//  printf("!Deleting item %d... list starts with %d (%d)\n",v,*list,(*list)->val);
  if (curr->val == v) {*list=curr->next; free(curr);}
  else
  while (curr -> next != NULL)
  {
    if (curr->next->val == v)
    {
      free(curr->next); curr->next=curr->next->next;
      break;
    }
   curr=curr->next;
  }
   
}

int check_zonelist(item *zonelist[NG][NG],int xzone[N],int yzone[N],double x[N],double y[N])
{
//  printf("!Start update\n");
  int i,j,xx,yy,n=0,aaa=0;

  item *curr, *head, *curr2;

  for (i=0;i<N;i++)
  {
    xx = getzone(x[i]);
    yy = getzone(y[i]);
    if (xzone[i] != xx || yzone[i] != yy)
    {
//    printf("!Ball %d is in box %d,%d, should be in %d,%d\n",i,xzone[i],yzone[i],xx,yy);
     del(&zonelist[xzone[i]][yzone[i]],i);
     add(&zonelist[xx][yy],i);
    }
    xzone[i]=xx; yzone[i]=yy;

   }
  
//  printf("!End update\n");
}
     

main()
{




  int i;



  item * zonelist[NG][NG];
  item * curr;
  int xzone[N],yzone[N];
  srand48(13091);
  printf("FPS 60\n");
  int vhisto[NH];
  int vhisto2[NH];
  double theta,phi;
  double T, EX, EY;
  int ehisto[NH];
  int ehisto2[NH];
  double x[N],y[N],vx[N],vy[N],mass[N],R[N];
  int j,k,l,m,n,s,vbin,ebin,zx,zy;
  double t=-8,dt=0.00001480;
  double angle,mag1,mag2,vxhalf,vyhalf;
  double vxcom,vycom;
  double E;
  printf("A\n");
  for (i=0;i<NG;i++)   for (j=0;j<NG;j++)
  {
    zonelist[i][j]=NULL;
  }

  for (i=0;i<N;i++) 
  {
    xzone[i]=0; yzone[i]=0;
    mass[i]=1;
    if (i % 5 == 0) mass[i]=5;
    if (i % 20 == 0) mass[i]=25;
   R[i]=sqrt(mass[i])*0.002;
    x[i]=drand48()*1.8-.9;
    y[i]=(drand48()*1.8-.9)/2-0.5;
    for (j=0;j<i;j++)
    {
      if (hypot(x[i]-x[j],y[i]-y[j]) < R[i]+R[j])
      {
    x[i]=drand48()*1.8-.9;
    y[i]=(drand48()*1.8-.9)/2-0.5;
         j=-1;
      }
    vx[i]=(drand48()-.5)*.0;
    vy[i]=(drand48()-.5)*.0; 
    }
  }
  printf("B\n");
  check_zonelist(zonelist,xzone,yzone,x,y);
  
  for (t=-1;1;t+=dt)
  {
    printf("C -1 1 -1\nl -1 -1 1 -1\nl 1 -1 1 1\nl 1 1 -1 1\nl -1 1 -1 -1\n");
    s++;
    for (i=0;i<N;i++)
    {
      x[i] += vx[i]*dt;
      y[i] += (vy[i]+g*0.5*dt)*dt;
      vy[i] += g*dt;
      if (x[i] < -1 && vx[i] < 0) vx[i] = -vx[i];
      if (y[i] < -1 && vy[i] < 0) vy[i] = -vy[i];
      if (x[i] > 1 && vx[i] > 0) vx[i] = -vx[i];
      if (y[i] > 1 && vy[i] > 0) vy[i] = -vy[i];
    }
  
// Collision checking
  
    for (i=0;i<N;i++)
    {
// We only need to check for collisions if they are in the same zone or adjacent
      zx=getzone(x[i]);
      zy=getzone(y[i]);
      for (k=zx-1;k<=zx+1;k++)       for (l=zy-1;l<=zy+1;l++)
      {
        if (k<0 || k>=NG || l<0 || l>=NG) continue;
        curr=zonelist[k][l];
        while (curr)
        {
          j=curr->val; if (i<=j) {curr=curr->next; continue;}
          //printf("!Checking ball %d to see if it hits ball %d... separation is %f, required %f\n",j,i,(x[i]-x[j])*(x[i]-x[j]) + (y[i]-y[j])*(y[i]-y[j]),(R[i]+R[j])*(R[i]+R[j]));
        if ((x[i]-x[j])*(x[i]-x[j]) + (y[i]-y[j])*(y[i]-y[j]) < (R[i]+R[j])*(R[i]+R[j]) && (vx[i]-vx[j])*(x[i]-x[j]) + (vy[i]-vy[j])*(y[i]-y[j]) < 0)
        {
//          printf("!COLLISION between balls %d and %d\n",i,j);
          printf("lC %f %f %f %f 1 0 0 \n",x[i],y[i],x[j],y[j]);
          vxcom = (vx[i]*mass[i]+vx[j]*mass[j])/(mass[i]+mass[j]);
          vycom = (vy[i]*mass[i]+vy[j]*mass[j])/(mass[i]+mass[j]);
          vx[i] -= vxcom; vy[i] -= vycom;  
          vx[j] -= vxcom; vy[j] -= vycom;
          phi = atan2(-vy[i]*mass[i],-vx[i]*mass[i]);
          theta = atan2(y[i]-y[j],x[i]-x[j]) ;
          angle = phi - 2*(phi-theta); 
          mag1=sqrt((mass[i]*(vx[i]*vx[i] + vy[i]*vy[i]) + mass[j]*(vx[j]*vx[j] + vy[j]*vy[j])) / (mass[i]+mass[i]*mass[i]/mass[j])); 
          mag2=sqrt((mass[j]*(vx[j]*vx[j] + vy[j]*vy[j]) + mass[i]*(vx[i]*vx[i] + vy[i]*vy[i])) / (mass[j]+mass[j]*mass[j]/mass[i])); 

hypot(vx[i]*mass[i],vy[i]*mass[i]); 
          vx[i] = cos(angle)*mag1 + vxcom;
          vy[i] = sin(angle)*mag1 + vycom; 
          vx[j] = -cos(angle)*mag2+ vxcom;
          vy[j] = -sin(angle)*mag2+ vycom;

        }
        curr=curr->next;
      }
    }
  }
  
  if (s%SKIP == 0) {
  E=0; EX=0; EY=0; T=0;
  if (t < 0) {
  for (j=0;j<NH;j++)
  {
    vhisto[j]=ehisto[j]=0;
    vhisto2[j]=ehisto2[j]=0;
  } 
  }
  for (i=0;i<N;i++)
  {
    vbin=(vx[i]+vm)/(vm*2)*NH;
    if (vbin<0) vbin=0;
    if (vbin>=NH) vbin=NH-1;
    if (mass[i] == 1) vhisto[vbin]++; else vhisto2[vbin]++;

    vbin=(vy[i]+vm)/(vm*2)*NH;
    if (vbin<0) vbin=0;
    if (vbin>=NH) vbin=NH-1;
    if (mass[i] == 1) vhisto[vbin]++; else vhisto2[vbin]++;
    
    ebin=sqrt(vx[i]*vx[i]+vy[i]*vy[i])/em*NH;
    if (ebin<0) ebin=0;
    if (ebin>=NH) ebin=NH-1;
    if (mass[i] == 1) ehisto[ebin]++; else {ehisto2[ebin]++;}
    if (mass[i] == 1) printf("C 0.6 0.6 0.6\n"); else printf("C 0.6 0 0.6\n");
    printf("c %f %f %f\n",x[i],y[i],R[i]);
    E += 0.5*mass[i]*(vx[i]*vx[i] + vy[i]*vy[i])-y[i]*g;
    T += 0.5*mass[i]*(vx[i]*vx[i] + vy[i]*vy[i])/N;
    EX += 0.5*mass[i]*(vx[i]*vx[i])/N;
    EY += 0.5*mass[i]*(vy[i]*vy[i])/N;
    
  }
  wscale = (double)1/NH;
  hscale = 0.04/N/wscale*dt/t*SKIP;
/*
  for (j=0;j<NH;j++)
  {
  printf("C 0.7 0 0\n");
    printf("l %f %f %f %f\n",wscale*j,-0.4,wscale*j,-0.4+vhisto[j]*hscale);
  printf("C 0.7 0 0.7\n");
    printf("l %f %f %f %f\n",wscale*j,-0.4,wscale*j,-0.4-vhisto2[j]*hscale);
  } 

  for (j=0;j<NH;j++)
  {
  printf("C 0 0.7 0\n");
    printf("l %f %f %f %f\n",-0.7+wscale*j,-0.4,-0.7+wscale*j,-0.4+ehisto[j]*hscale);
  printf("C 0 0.7 0.7\n");
    printf("l %f %f %f %f\n",-0.7+wscale*j,-0.4,-0.7+wscale*j,-0.4-ehisto2[j]*hscale);
  }

  printf("C 1 1 1\n");
  printf("T -.75 0.9\nTotal energy: %.3e\tkT: %.3e\tKinetic energy in x: %.3e\tKinetic energy in y: %.3e\n",E,T,EX,EY);*/
  printf("T -.75 0.85\nTime: %f\n",t);
  check_zonelist(zonelist,xzone,yzone,x,y);
  }
  if (s%SKIP2 == 0) printf("F\n");

}

}  
