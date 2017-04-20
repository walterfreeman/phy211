#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define N 100
#define SKIP 20
#define NH 600
#define NG 200

struct list_el {
   int val;
   struct list_el * next;
}; typedef struct list_el item;

typedef struct {
   double x;
   double y;
} vector; 

typedef struct{
   double r;
   double g;
   double b;
}color_t;

typedef struct {
   vector pos;
   vector vel;
   double mass;
   double radius;
   color_t color;
   int zone_x;
   int zone_y;
}atom_t;

typedef struct {
   vector dpos;
   vector dvel;
}deriv_t;

vector vMultAdd(vector a, vector b, double x)
{
  // returns a + xb
  vector c;
  c.x = a.x + b.x*x;
  c.y = a.y + b.y*x;
  return c;
} 

double evolve(atom_t *state, deriv_t deriv[], double dt)
{
  int i;
  for (i=0;i<N;i++)
  {
    state[i].pos = vMultAdd(state[i].pos,deriv[i].dpos,dt);
  }
}

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

int check_zonelist(item *zonelist[NG][NG],atom_t state[N])
{
//  printf("!Start update\n");
  int i,j,xx,yy,n=0,aaa=0;

  item *curr, *head, *curr2;

  for (i=0;i<N;i++)
  {
    xx = getzone(state[i].pos.x);
    yy = getzone(state[i].pos.y);
    if (state[i].zone_x != xx || state[i].zone_y != yy)
    {
//    printf("!Ball %d is in box %d,%d, should be in %d,%d\n",i,xzone[i],yzone[i],xx,yy);
     del(&zonelist[state[i].zone_x][state[i].zone_y],i);
     add(&zonelist[xx][yy],i);
    }
    state[i].zone_x=xx; state[i].zone_y=yy;

   }
  
//  printf("!End update\n");
}
     

void drawballs(atom_t state[N],int num)
{
  static color_t currcol;
  int i;
  for (i=0;i<num;i++)
  {
    if (currcol.r != state[i].color.r) {printf("C %f %f %f\n",state[i].color.r,state[i].color.g,state[i].color.b); currcol = state[i].color;}
    printf("c %f %f %f\n",state[i].pos.x, state[i].pos.y, state[i].radius);
  }
}


void init(int i, atom_t *atom, double boxsize)
{
  atom->radius = 0.01;
  double theta,r;
  if (i==1)
  {
   atom->color.r=1;
   atom->color.g=1;
   atom->color.b=1;
   atom->mass=1;
   atom->pos.x=(drand48()-.5)*1.8;
   atom->pos.y=(drand48()-.5)-0.5;
   atom->vel.x=0;
   atom->vel.y=0;
   }
  if (i==2)
  {
   atom->color.r=1;
   atom->color.g=0;
   atom->color.b=0;
  atom->mass=3;
  atom->vel.x=0;
  atom->vel.y=-.3;
  do {
    atom->pos.x=(drand48()-.5)*0.25;
    atom->pos.y=(drand48()-.5)*0.25+0.4;
   } while (hypot(atom->pos.x,atom->pos.y-0.4) > 0.05);
  }
  atom->pos.x *= boxsize;
  atom->pos.y *= boxsize;

}


main()
{

  int i;

  printf ("S %f\n",box_size);

  item * zonelist[NG][NG];
  item * curr;
  srand48(13091);
  printf("FPS 30\n");
  double theta,phi,maxv;
//  double x[N],y[N],vx[N],vy[N],mass[N],R[N];
  int j,k,l,m,n,s,vbin,ebin,zx,zy;
  atom_t atoms[N];
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
    printf("!Trying to place ball %d\n",i);
    if (i < N*0.99) n=1; else n=2;
      init(n,&atoms[i],box_size);
    for (j=0;j<i;j++)
    {
      if (hypot(atoms[i].pos.x-atoms[j].pos.x,atoms[i].pos.y-atoms[j].pos.y) < atoms[i].radius+atoms[j].radius)
      {
         printf("!Position %f,%f rejected because of conflict with ball %d at %f,%f\n",j);
         init(n,&atoms[i],box_size);
         j=-1;
      }
    }
  }
  check_zonelist(zonelist,atoms);
  
  for (t=-1;1;t+=dt)
  {
    s++;
    for (i=0;i<N;i++)
    {
      atoms[i].pos.x += atoms[i].vel.x*dt;
      atoms[i].pos.y += (atoms[i].vel.y+g*0.5*dt)*dt;
      atoms[i].vel.y += g*dt;
      if (atoms[i].pos.x < -box_size && atoms[i].vel.x < 0) atoms[i].vel.x = -atoms[i].vel.x;
      if (atoms[i].pos.y < -box_size && atoms[i].vel.y < 0) atoms[i].vel.y = -atoms[i].vel.y;
      if (atoms[i].pos.x > box_size && atoms[i].vel.x > 0) atoms[i].vel.x = -atoms[i].vel.x;
      if (atoms[i].pos.y > box_size && atoms[i].vel.y > 0) atoms[i].vel.y = -atoms[i].vel.y;
    }
  
// Hard-sphere collision checking
  
    for (i=0;i<N;i++)
    {
// We only need to check for collisions if they are in the same zone or adjacent
      zx=getzone(atoms[i].pos.x);
      zy=getzone(atoms[i].pos.y);
      for (k=zx-1;k<=zx+1;k++)       for (l=zy-1;l<=zy+1;l++)
      {
        if (k<0 || k>=NG || l<0 || l>=NG) continue;
        curr=zonelist[k][l];
        while (curr)
        {
          j=curr->val; if (i<=j) {curr=curr->next; continue;}
          //printf("!Checking ball %d to see if it hits ball %d... separation is %f, required %f\n",j,i,(atoms[i].pos.x-atoms[j].pos.x)*(atoms[i].pos.x-atoms[j].pos.x) + (atoms[i].pos.y-atoms[j].pos.y)*(atoms[i].pos.y-atoms[j].pos.y),(atoms[i].radius+atoms[j].radius)*(atoms[i].radius+atoms[j].radius));
        if ((atoms[i].pos.x-atoms[j].pos.x)*(atoms[i].pos.x-atoms[j].pos.x) + (atoms[i].pos.y-atoms[j].pos.y)*(atoms[i].pos.y-atoms[j].pos.y) < (atoms[i].radius+atoms[j].radius)*(atoms[i].radius+atoms[j].radius))
        {
          if ( (atoms[i].pos.x-atoms[j].pos.x) * (atoms[i].vel.x-atoms[j].vel.x) + (atoms[i].pos.y-atoms[j].pos.y)*(atoms[i].vel.y-atoms[j].vel.y) < 0)
          {
           
//          printf("!COLLISION between balls %d and %d\n",i,j);
          printf("l %f %f %f %f\n",atoms[i].pos.x,atoms[i].pos.y,atoms[j].pos.x,atoms[j].pos.y);
          vxcom = (atoms[i].vel.x*atoms[i].mass+atoms[j].vel.x*atoms[j].mass)/(atoms[i].mass+atoms[j].mass);
          vycom = (atoms[i].vel.y*atoms[i].mass+atoms[j].vel.y*atoms[j].mass)/(atoms[i].mass+atoms[j].mass);
          atoms[i].vel.x -= vxcom; atoms[i].vel.y -= vycom;  
          atoms[j].vel.x -= vxcom; atoms[j].vel.y -= vycom;
          phi = atan2(-atoms[i].vel.y*atoms[i].mass,-atoms[i].vel.x*atoms[i].mass);
          theta = atan2(atoms[i].pos.y-atoms[j].pos.y,atoms[i].pos.x-atoms[j].pos.x) ;
          angle = phi - 2*(phi-theta); 
          mag1=sqrt((atoms[i].mass*(atoms[i].vel.x*atoms[i].vel.x + atoms[i].vel.y*atoms[i].vel.y) + atoms[j].mass*(atoms[j].vel.x*atoms[j].vel.x + atoms[j].vel.y*atoms[j].vel.y)) / (atoms[i].mass+atoms[i].mass*atoms[i].mass/atoms[j].mass)); 
          mag2=sqrt((atoms[j].mass*(atoms[j].vel.x*atoms[j].vel.x + atoms[j].vel.y*atoms[j].vel.y) + atoms[i].mass*(atoms[i].vel.x*atoms[i].vel.x + atoms[i].vel.y*atoms[i].vel.y)) / (atoms[j].mass+atoms[j].mass*atoms[j].mass/atoms[i].mass)); 

// hypot(atoms[i].vel.x*atoms[i].mass,atoms[i].vel.y*atoms[i].mass); 
          atoms[i].vel.x = cos(angle)*mag1 + vxcom;
          atoms[i].vel.y = sin(angle)*mag1 + vycom; 
          atoms[j].vel.x = -cos(angle)*mag2+ vxcom;
          atoms[j].vel.y = -sin(angle)*mag2+ vycom;
          }
        }
        curr=curr->next;
      }
    }
  }
  
  if (s%SKIP == 0) {
  drawballs(atoms,N);
  for (i=0;i<N;i++)
  {
    if (hypot(atoms[i].vel.x,atoms[i].vel.y) > maxv) maxv=hypot(atoms[i].vel.x,atoms[i].vel.y);
  }
  }
  if (dt > atoms[0].radius/maxv*0.1) {dt=atoms[0].radius/maxv*0.1; printf("!Timestep changed to %f to preserve your mom.\n",dt);}
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
  check_zonelist(zonelist,atoms);
  printf("F\n");
//  fprintf(stderr,"drawing a frame: time %f\n",t);
}
}

