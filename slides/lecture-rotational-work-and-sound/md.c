#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define N 10000
#define SKIP 20
#define NH 600
#define NG 200


struct list_el {
   int val;
   struct list_el * next;
};

typedef struct list_el item;

double rmax=0.02;
double box_size=10;
double g=-0;
double vm=.4;
double em=.6;
double hscale,wscale;

int getzone(double x)
{
  x /= box_size;
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
  if (curr == NULL) return;
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
     

void drawballs(double x[], double y[], double R[], double mass[],int num)
{
  printf("!  Beginning drawing: number is %d.\n",num);
  int i;
  for (i=0;i<num;i++)
  {
    printf("!   Drawing inside function: i = %d of %d\n",i,num);
    if (mass[i] == 1) printf("C 1 1 1\n"); else printf("C 1 0 1\n");
    printf("c %f %f %f\n",x[i],y[i],R[i]);
  }
}


void initpos(int i, double *x, double *y, double boxsize)
{
  double theta,r;
  if (i==1)
  {
   *x=(drand48()-.5)*1.8;
   *y=(drand48()-.5)-0.5;
   }
  if (i==2)
  {
  do {
    *x=(drand48()-.5)*0.25;
    *y=(drand48()-.5)*0.25+0.4;
   } while (hypot(*x,*y-0.4) > 0.05);
  }
  *x *= boxsize;
  *y *= boxsize;

}


main()
{




  int i;

  printf ("S %f\n",box_size);

  item * zonelist[NG][NG];
  item * curr;
  int xzone[N],yzone[N];
  srand48(13091);
  printf("FPS 30\n");
  int vhisto[NH];
  int vhisto2[NH];
  double theta,phi,maxv;
  double T, EX, EY;
  int ehisto[NH];
  int ehisto2[NH];
  double x[N],y[N],vx[N],vy[N],mass[N],R[N];
  int j,k,l,m,n,s,vbin,ebin,zx,zy;
  double t=-8,dt=0.014480;
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
    R[i]=0.03;
    xzone[i]=0; yzone[i]=0;
    printf("!Trying to place ball %d\n",i);
    if (i < N*0.99)
    {
      printf("!Placing it in main group\n");
    mass[i]=1;
    initpos(1,&x[i],&y[i],box_size);
    for (j=0;j<i;j++)
    {
      if (hypot(x[i]-x[j],y[i]-y[j]) < R[i]+R[j])
      {
         printf("!Position %f,%f rejected because of conflict with ball %d at %f,%f\n",x[i],y[i],j,x[j],y[j]);
    initpos(1,&x[i],&y[i],box_size);
         j=-1;
      }
    }
    vx[i]=0;
    vy[i]=0; 
  }
  else
  {
    printf("!Placing it in small group\n");
    mass[i]=2;
    initpos(2,&x[i],&y[i],box_size);
  for (j=0;j<i;j++)
    {
      if (hypot(x[i]-x[j],y[i]-y[j]) < R[i]+R[j])
      {
         printf("!Position %f,%f rejected because of conflict with ball %d at %f,%f\n",x[i],y[i],j,x[j],y[j]);
         j=-1;
    initpos(2,&x[i],&y[i],box_size);
      }
    }
    vx[i]=0;
    vy[i]=-0.1; 

  }
  }
  check_zonelist(zonelist,xzone,yzone,x,y);
  
  for (t=-1;1;t+=dt)
  {
    s++;
    for (i=0;i<N;i++)
    {
      x[i] += vx[i]*dt;
      y[i] += (vy[i]+g*0.5*dt)*dt;
      vy[i] += g*dt;
      if (x[i] < -box_size && vx[i] < 0) vx[i] = -vx[i];
      if (y[i] < -box_size && vy[i] < 0) vy[i] = -vy[i];
      if (x[i] > box_size && vx[i] > 0) vx[i] = -vx[i];
      if (y[i] > box_size && vy[i] > 0) vy[i] = -vy[i];
    }
  
// Hard-sphere collision checking
  
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
        if ((x[i]-x[j])*(x[i]-x[j]) + (y[i]-y[j])*(y[i]-y[j]) < (R[i]+R[j])*(R[i]+R[j]))
        {
          if ( (x[i]-x[j]) * (vx[i]-vx[j]) + (y[i]-y[j])*(vy[i]-vy[j]) < 0)
          {
           
//          printf("!COLLISION between balls %d and %d\n",i,j);
          printf("l %f %f %f %f\n",x[i],y[i],x[j],y[j]);
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
    if (mass[i] == 1) printf("C 1 1 1\n"); else printf("C 1 0 1\n");
    printf("c %f %f %f\n",x[i],y[i],R[i]);
    if (hypot(vx[i],vy[i]) > maxv) maxv=hypot(vx[i],vy[i]);
  }
  if (dt > R[0]/maxv*0.1) {dt=R[0]/maxv*0.1; printf("!Timestep changed to %f to preserve your mom.\n",dt);}
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
  printf("F\n");
//  fprintf(stderr,"drawing a frame: time %f\n",t);
}
}

}  
