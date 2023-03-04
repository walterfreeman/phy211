#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "vector.h"
#include <omp.h>

int ilist_size=1000000; // how much memory to allocate for the list of possible interactions
int N=2000; // default number of particles (probably changed later)
double L=1; // box width
double r0=0.015; // interaction radius
int NG; // number of zones in each direction. Set later
double pad=2; // At what multiple of r0 do we assume the force is small and neglect the interactions?
double timer[10]; // Array shared by starttimer() and stoptimer() for timing things
double steprate;
double forcerate;

// the following is a new thing y'all haven't learned yet. Ask me about in person; this is used to make
// "linked lists", which are a way of keeping track of which particles are where.
struct list_el {
  int val;
  struct list_el *next;
};
typedef struct list_el item;


double myclock(void)
{
  return omp_get_wtime();
}

bool istime3(int msec)
{
  
  static double lasttime=0;
  if (myclock() - lasttime > msec / 1000.0)
  {
    lasttime=myclock();
    return true;
  }
  else return false;
}

// Just a dopey timing function that tells you if n milliseconds have passed since the last time you called it
// "bool" = "boolean", true or false (1 or 0, basically)
bool istime2(int msec)
{
  static double lasttime=0;
  if (myclock() - lasttime > msec / 1000.0)
  {
    lasttime=myclock();
    return true;
  }
  else return false;
}

bool istime(int msec)
{
  static double lasttime=0;
  if (msec == 0)
  {
    lasttime=myclock();
    return false;
  }
  if (myclock() - lasttime > msec / 1000.0)
  {
    return true;
  }
  else return false;
}

// given a particle's position, which zone is it in?
ivec getzone (vector v)
{
  ivec result;
  result.x=(int)((v.x+L/2)*NG/L);
  result.y=(int)((v.y+L/2)*NG/L);
  result.z=(int)((v.z+L/2)*NG/L);
  if (result.x>=NG) result.x=NG-1;
  if (result.x<0) result.x=0;
  if (result.y>=NG) result.y=NG-1;
  if (result.y<0) result.y=0;
  if (result.z>=NG) result.z=NG-1;
  if (result.z<0) result.z=0;
//  printf("!Particle in zone (%d, %d, %d)\n",result.x, result.y, result.z);
  return result;
}

// add/remove an item from linked lists -- will explain this in person
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
  item *temp;
  if (curr == NULL) {return;}
  //  printf("!Deleting item %d... list starts with %d (%d)\n",v,*list,(*list)->val);
  if (curr->val == v) {*list=curr->next; free(curr);}
  else
    while (curr -> next != NULL)
    {
      if (curr->next->val == v)
      {
        temp=curr->next;
        curr->next=curr->next->next;
	free(temp); 
	break;
      }
      curr=curr->next;
    }
}



// cycle through all the particles and put them in the right zones
void check_zonelist(item **zonelist,ivec zone[], vector pos[])
{
  ivec cz; // the correct zone for the particle to be in
  for (int i=0;i<N;i++)
  {
//    printf("zonelist check: particle %d of %d\n",i,N);
    cz = getzone(pos[i]);
    if (cz.x != zone[i].x || cz.y != zone[i].y || cz.z != zone[i].z) // if we're not in the right zone, fix it
    {
      if (zone[i].x >= 0 && zone[i].x < NG && zone[i].y >= 0 && zone[i].y < NG && zone[i].z >= 0 && zone[i].x < NG) del(&zonelist[zone[i].x*NG*NG + zone[i].y * NG + zone[i].z],i);
      add(&zonelist[cz.x*NG*NG + cz.y*NG + cz.z],i); 
    }
    zone[i]=cz;
//    printf("Done checking zone of particle %d of %d (it's in (%d, %d, %d)\n",i,N,cz.x,cz.y,cz.z);
  }
}

// a function for V(r), used only to compute total energy
double V(double r)
{
  if (r > r0*pad) return V(r0*pad);
  r/=r0;
  static double r6;
  r6=r*r*r*r*r*r;
  return (4*r0*(1/(r6*r6)-1/r6));
}

double V2(double r)
{
  if (r*r > r0*r0*pad*pad) return V2(r0*r0*pad*pad);
  r/=(r0*r0);
  static double r6;
  r6=r*r*r;
  return (4*r0*(1/(r6*r6)-1/r6));
}


// derpy timing functions: this one starts a timer with index t...
void starttimer(int t)
{
  timer[t]=myclock();
}

// and this one stops it, returning how long it's been in microseconds since starttimer was called
double stoptimer(int t)
{
  return myclock()-timer[t];
}

// NOTE: Actually returns F/r to avoid having to do a square root in velocity_update
//       when dealing with the unit vectors. This is why it's r^14-r^8.
//       Square roots are 'spensive.
double force(double r2)
{
  static double r8,r14,r6;
  r6=r2*r2*r2; // doing this is substantially faster than using pow()
  r8=r6*r2;
  r14=r6*r8;
  return -24.*(2./r14 - 1./r8)  / r0; // the factor of 24 is in some canonical form of this. gods know why.
}

double drnd48(void) // just a way to get a centered random number
{
  return drand48()-0.5;
}

void draw_box(double L)
{
  // baaaaarf.
  printf("l3 -%e -%e -%e -%e -%e  %e\n",L,L,L,L,L,L);
  printf("l3 -%e -%e  %e -%e  %e  %e\n",L,L,L,L,L,L);
  printf("l3 -%e  %e  %e -%e  %e -%e\n",L,L,L,L,L,L);
  printf("l3 -%e  %e -%e -%e -%e -%e\n",L,L,L,L,L,L);
  printf("l3  %e -%e -%e  %e -%e  %e\n",L,L,L,L,L,L);
  printf("l3  %e -%e  %e  %e  %e  %e\n",L,L,L,L,L,L);
  printf("l3  %e  %e  %e  %e  %e -%e\n",L,L,L,L,L,L);
  printf("l3  %e  %e -%e  %e -%e -%e\n",L,L,L,L,L,L);
  printf("l3  %e -%e -%e -%e -%e -%e\n",L,L,L,L,L,L);
  printf("l3  %e  %e -%e -%e  %e -%e\n",L,L,L,L,L,L);
  printf("l3  %e -%e  %e -%e -%e  %e\n",L,L,L,L,L,L);
  printf("l3  %e  %e  %e -%e  %e  %e\n",L,L,L,L,L,L);
}



// This is the function I said I'd give you a copy of. What this does:
// * assumes that zonelist is an array of linked lists, one for each zone, that holds the particles in each zone
// * ... and that zone is an array of size N that holds which zone each particle is in
// * For each particle i in the simulation:
//   * For its own zone and all adjacent zones:
//     * For each particle j in those zones (iterated over with the while loop -- I'll explain this in person)
//       * If the distance from i to j is more than r0*pad, do nothing
//       * ... otherwise, add that pair of particles to ilist1 and ilist2, and their squared distance to r2list
// When we're done, ilist1 and ilist2 contain paired lists of all the interactions worth thinking about
// ... and r2list contains how far away those particles are, so you don't have to work it out again
// This returns the number of interactions that we found, so we know how far to take the for loops later

int build_interaction_list(const vector pos[],item *zonelist[],const ivec zone[],int ilist1[],int ilist2[], double r2list[], vector seplist[])
{
  // need to make some internally-used interaction list arrays for the threads

  static int * ilist1p=(int *)malloc(sizeof(int) * ilist_size);
  static int * ilist2p=(int *)malloc(sizeof(int) * ilist_size);
  static double * r2p=(double *)malloc(sizeof(double) * ilist_size);
  static vector * seplistp=(vector *)malloc(sizeof(vector) * ilist_size);
  const int iliststride=ilist_size / omp_get_max_threads();


  int num_found[omp_get_max_threads()]={0}; // keep track of how many interactions we've found already -- 
  // separate per thread!

#pragma omp barrier

#pragma omp parallel
  {
    int num_found_local=0;  
    ivec myzone; //ivec = vector of integers (only used here)
    item *curr; 
    int i, j, k, l, m;
    int tid=omp_get_thread_num();
    int nth=omp_get_num_threads();
    int stride=N/nth+1;
//    printf("!I am thread %d, scanning from %d to %d\n",tid,stride*tid, stride*(tid+1)-1);

    for (i=stride*tid; i<stride*(tid+1); i++)
    {
      double r2;
      if (i >= N) break;

      myzone=getzone(pos[i]);
      int index;
      for (k=myzone.x-1; k<=myzone.x+1; k++)
	for (l=myzone.y-1; l<=myzone.y+1; l++)
	  for (m=myzone.z-1; m<=myzone.z+1; m++)
	  {
	    // if we've driven off the edge of the simulation region, ignore and move to
	    // the next zone
	    if (k<0 || k>=NG || l<0 || l>=NG || m<0 || m>=NG) 
	      continue;
	    curr=zonelist[k*NG*NG + l*NG + m]; // this awkwardness avoids the issues with passing multidimensional arrays
	    // while there are particles left in this zone...
	    while (curr)
	    {
	      j=curr->val; 
	      // skip if pair misordered, same particle, or further apart than permitted
	      // this last condition may seem silly, but it allows us to consistently truncate
	      // the force so that the physics don't depend on the exact layout of the zone boundaries.
	      // (it also makes things faster.)
	      //r2=(pos[i]-pos[j])*(pos[i]-pos[j])/(r0*r0); // squared distance between, in units of r0
              if (i<=j) {curr=curr->next; continue;}
              r2 = ((pos[i].x - pos[j].x) * (pos[i].x - pos[j].x) +
                    (pos[i].y - pos[j].y) * (pos[i].y - pos[j].y) +
                    (pos[i].z - pos[j].z) * (pos[i].z - pos[j].z))/(r0*r0);
                 
	      if (r2 > pad*pad) {curr=curr->next; continue;}
              index = num_found_local+iliststride*tid;
//	      if (i<=j || r2 > pad*pad) {curr=curr->next; continue;}
	      r2p[index] = r2; // it is very marginally faster to store a third array, containing the squared
              seplistp[index] = (pos[i] - pos[j]);
	      // distances between particles, so we don't have to compute it again.
	      ilist1p[index] = i;
	      ilist2p[index] = j;
	      num_found_local++;
	      curr=curr->next;
	    }
	  }
    }
    num_found[tid]=num_found_local;
  }
  #pragma omp barrier
  // merge lists
  int numtotal=0;
  for (int i=0; i<omp_get_max_threads(); i++)
  {
//    		  printf("!Found %d interactions in list %d\n",num_found[i],i);
    for (int j=0; j<num_found[i]; j++)
    {
      ilist1[numtotal]=ilist1p[j + iliststride*i];
      ilist2[numtotal]=ilist2p[j + iliststride*i];
      r2list[numtotal]=r2p[j + iliststride*i];
      seplist[numtotal]=seplistp[j + iliststride*i];
      numtotal++;
    }
  }
//    printf("!Found %d total.\n",numtotal);
//  printf("!Entire interaction list cost: %f us\n",stoptimer(9)*1e3);
  for (int i=0; i<numtotal; i++)
  {
//    printf("!Merged: Interaction %d between %d and %d, distance %e\n",i,ilist1[i],ilist2[i], r2list[i]);
  }
  return numtotal; 
}

void position_step(vector *pos, vector *vel, double dt)
{
  for (int i=0; i<N; i++)
    pos[i] += vel[i] * dt;
}

void velocity_step(vector *pos, vector *vel, double *m,
    int *ilist1, int *ilist2, double *r2list, int nint, double dt, int *nints)
{
  double PE=0;
  vector sep;
  double r2;
  for (int i=0; i<N; i++) nints[i]=0;
  for (int i=0; i<nint; i++) // traverse the interaction list in ilist1 and ilist2
  {
    vector F;
    int j,k;
    j=ilist1[i]; k=ilist2[i];
    nints[j]++; nints[k]++;
    sep=pos[j]-pos[k];
    //     r2 = sep*sep/r0*r0;
    //     if (r2 > pad*pad) continue;
    //     F=(force((sep*sep)/(r0*r0))*dt)*sep; // parentheses not absolutely necessary but may save a few flops
    F=(force((r2list[i]))*dt)*sep; // is it faster to look up the radius in the list, or compute it?
    // maybe for bigger problems that don't fit into cache the above is better
    vel[j] -= F/m[j];
    vel[k] += F/m[k];
  }
}


double kinetic(vector v[], double m[])
{
  double T=0;
  for (int i=0;i<N;i++)
    T+=0.5*m[i]*v[i]*v[i]; // note that v*v does a dot product
  return T;
}

double potential(vector pos[])
{
  double U=0;
  for (int i=0;i<N;i++)
    for (int j=i+1;j<N;j++)
    {
      U+=V(norm(pos[i]-pos[j]));
    }
  return U;
}









// dummy space




int main(int argc, char **argv)
{
  printf("font large\n");
  double time_intlist=0; 
  double time_velstep=0;
  double lastframetime=0;
  double time_posstep=0;
  double time_check=0;
  double time_checklist=0;
  double time_anim=0;
  double lastchecksteps=0;
  double time_energy=0; // some variables for timing
  double dt=1e-3, vinit=0;
  double P=0, T=0, U, J=0; // J is impulse -- haven't finished that bit yet
  int i=0,j=0,k=0,l=0;
  int ninttotal=0;
  int nint=0;
  int drawn=0;
  int *ilist1=NULL,*ilist2=NULL;
  double *r2list=NULL;
  vector *seplist=NULL;
  double KE=0, PE=0,Tnow=0;
  ilist1=(int *)malloc(sizeof(int) * ilist_size);
  ilist2=(int *)malloc(sizeof(int) * ilist_size);
  r2list=(double *)malloc(sizeof(double) * ilist_size);
  seplist=(vector *)malloc(sizeof(vector) * ilist_size);
  printf("!Allocated...\n");
  double thermo_interval=1;
  double next_thermo=thermo_interval;
  double Taccum=0;
  printf("!!%d parameters\n",argc);
  if (argc < 11) // if they didn't give us command line parameters, die and print out the order
  {
    printf("!!Usage: <this> <N> <dt> <vinit> <pad> <L> <draw_msec> <neighbor threshold> <threads> <t_max> <temp target> <temp change rate>\n");
    exit(1);
  }
  // read command line parameters
  N=atoi(argv[1]);
  dt=atof(argv[2]);
  vinit=atof(argv[3]);
  pad=atof(argv[4]);
  L=atof(argv[5]);
  double drawms=atof(argv[6]);
  int drawthreshold=atoi(argv[7]);
  int threads=atoi(argv[8]);
  double tmax=atof(argv[9]);
  omp_set_num_threads(threads);
  double Ttarget=atof(argv[10]);
  double smult=atof(argv[11]);
  double rate=1;
  NG=L/(r0*pad);
  printf("!!Read parameters. Drawing every %f ms\n",drawms);
  ivec zone[N];
  int lastframe=0;
  vector pos[N],v[N];
  double drawbuf[N*7];
  double m[N];
  int interactioncount[N];
  double smooth_interaction_count[N]={0};
  

  char logfilename[100];
  snprintf(logfilename,100,"thermodynamics_N%d_dt%s_pad%s_L%s_vinit%s.dat",N,argv[2],argv[4],argv[5],argv[3]);
  printf("!!Opening file %s for thermodynamics data\n",logfilename);
 
  FILE *logfile=fopen(logfilename,"w");

  fprintf(logfile,"# Starting up: N=%d dt=%e pad=%e L=%e\n",N,dt,pad,L);
  fprintf(logfile,"time\tPV\tNkT\tPV/NkT\tEnergy\n");
  FILE *energyfile=fopen("energy.txt","w");
  printf("!!NG = %d, NG^3 = %d\n",NG,NG*NG*NG);
//  item *zonelist[NG*NG*NG];
  item **zonelist;
  zonelist=(item **)malloc(NG*NG*NG*sizeof(item *));
  for (i=0;i<NG*NG*NG;i++) 
  {
    zonelist[i]=NULL;
  }
  printf("!!Allocated zone list array\n");
  // set up initial conditions
  
  double interval = r0 * pow(2,1./6.); // how far away to put the particles at the start
  for (i=0;i<N;i++)
  {
    j++;
    if (j>pow(N,1./3.)) {j=0;k++;}
    if (k>pow(N,1./3.)) {k=0;l++;}

    zone[i].x=-1; zone[i].y=-1; zone[i].z=-1; // set these to -1, we'll fix later
    pos[i].x=k*interval-interval * pow(N,1./3.)/2;
    pos[i].y=l*interval-interval * pow(N,1./3.)/2;
    pos[i].z=j*interval-interval * pow(N,1./3.)/2;
    v[i].x=drnd48()*vinit;
    v[i].y=drnd48()*vinit;
    v[i].z=drnd48()*vinit;
    m[i]=1;
  }
  printf("!!Initialization done: N = %d\n",N);
  
  check_zonelist(zonelist,zone,pos); // put particles in the right zones to start

  printf("!!start main loop\n");

  int steps=0;
  for (double t=0; t<tmax; t+=dt)
  {
#pragma omp barrier
       starttimer(9);
   if (istime2(4000) && 0)
     {
       KE=kinetic(v,m);
       PE=potential(pos);
       fprintf(energyfile,"%e %e\n",t,KE+PE);
       fflush(energyfile);
     }
       time_energy+=stoptimer(9);
#pragma omp barrier

if (istime(drawms)) // anim time
//   if (steps % 100 == 0)
   {
     int drawct=0;
     starttimer(4);
     // it's actually super expensive to compute the total energy since we do it
     // pairwise to ensure absolute sanity, so do it only every 100 anim updates
          printf("C 0.5 1 0.5\n");
      // draw particles      
      for (i=0;i<N;i++)
      {
	  drawbuf[i*7+0] = pos[i].x;
	  drawbuf[i*7+1] = pos[i].y;
	  drawbuf[i*7+2] = pos[i].z;
	  drawbuf[i*7+3] = r0/4;
	  if (smooth_interaction_count[i] < drawthreshold)
	  {
		  drawbuf[i*7+4] = 0.5;
		  drawbuf[i*7+5] = 1;
		  drawbuf[i*7+6] = 0.5;
	  }
	  else
	  {
		  drawbuf[i*7+4] = 1;
		  drawbuf[i*7+5] = 0.2;
		  drawbuf[i*7+6] = 0.2;
	  }


//        if (i==0) printf("C 1 1 1\n"); 
//        if (i==1) printf("C 0.5 1 0.5\n");
    //    if (i<1) printf("ct3 %d %.3e %.3e %.3e %.3e\n",i,pos[i].x,pos[i].y,pos[i].z,r0/4);
        }
      printf("manycoloredballs %d\n",N);

       fwrite(drawbuf, N*7, sizeof(double), stdout);



      printf("C 0.7 0.2 0.2\n");
      draw_box(L/2);
      printf("C 0.5 0.5 1\n");
//      printf("T -0.9 0.85\nt=%.3f/%.2f, E=%.5e = %.5e + %.5e\n",t,next_thermo,KE+PE,KE,PE);
      //POS
//      printf("T -0.9 0.68\nCurrent temp = %.3e\tTarget = %.3e\trate = %.4e\n",Tnow,Ttarget,rate);
      printf("T -0.9 0.75\nPV = %.4e \t NkT = %.4e \t ratio = %f\n",P*L*L*L,N*T,P*L*L*L/(N*T));
      if (rate > 0 ) printf("T -0.9 0.68\nCurrent temp = %.3e\tTarget = %.3e\trate = %.4e\n",Tnow,Ttarget,rate);

      printf("T -0.9 0.85\nSimulation rate: %.1f steps per second, %.2f million forces/sec, %d particles\n",steprate,forcerate/1e6,N);
      lastframe=steps;
      
      printf("F\n");
//      printf("!!%d interactions\n",nint);
      if (istime3(drawms*2) && 0)
      {
      for (j=0; j<nint; j++)
      {
        if (fabs(seplist[j].x)<r0*10)
        {
        printf("!c3 %.4e %.4e %.4e %.4e\n",-seplist[j].x,-seplist[j].y,-seplist[j].z,r0/100);
        printf("!c3 %.4e %.4e %.4e %.4e\n",seplist[j].x,seplist[j].y,seplist[j].z,r0/100);
        }
      }
      printf("!F\n");
      }
      lastframetime=stoptimer(4);
      fflush(stdout);
      time_anim += stoptimer(4);
      istime(0);
   }
   
   // start force calculations

     // check zonelist
     // build list of interactions. pass in pointers
     // to interaction lists so it can realloc them
     // if need be
    position_step(pos,v,dt/2);

     starttimer(1);
     starttimer(2);
     check_zonelist(zonelist,zone,pos);
     time_checklist += stoptimer(2);
     nint=build_interaction_list(pos,zonelist,zone,ilist1,ilist2,r2list,seplist);
     time_intlist += stoptimer(1);
//     printf("!%d interactions\n",nint);
     

     starttimer(3);
     velocity_step(pos,v,m,ilist1,ilist2,r2list,nint,dt,interactioncount);
     position_step(pos,v,dt/2);

     double eps = 0.1786178958448;
     double lam = -0.2123418310626;
     double chi = -0.06626458266982;

/*   
Commented out -- fourth-order Omelyan algorithm doesn't seem to have stability advantages  
position_step(pos,v,dt*eps);
velocity_step(pos,v,m,ilist1,ilist2,r2list,nint,dt*(1-2*lam)/2);
position_step(pos,v,dt*chi);
velocity_step(pos,v,m,ilist1,ilist2,r2list,nint,dt*lam);
position_step(pos,v,dt*(1-2*(chi+eps)));
velocity_step(pos,v,m,ilist1,ilist2,r2list,nint,dt*lam);
position_step(pos,v,dt*chi);
velocity_step(pos,v,m,ilist1,ilist2,r2list,nint,dt*(1-2*lam)/2);
position_step(pos,v,dt*eps);
*/     
     
     time_velstep += stoptimer(3);
     steps++;
     ninttotal+=nint;
   
     for (int i=0; i<N; i++)
     {
       if (smooth_interaction_count[i]==0) smooth_interaction_count[i]=interactioncount[i];
         else smooth_interaction_count[i] = smooth_interaction_count[i]*0.99+interactioncount[i]*0.01;
     } 
     // all this code is just here to determine how much time different parts of the calculation are taking
     if (myclock() - 1.0 > time_check)
     {
       double curtime=myclock();
       steprate = (float)(steps-lastchecksteps)/(curtime-time_check);
       forcerate = (float)ninttotal/(curtime-time_check);
       printf("!!%.2f force (%.2f ns/int), %.2f anim, %.2f ilist, %.2f checklist, %.2f energy (us/step): %.1f ints/atom, %.1f SPS, %.1f MIPS (%d threads)\n",
            1e3*(float)time_velstep / (steps-lastchecksteps),
            1e6*(float)time_velstep/ (ninttotal),
            1e3*(float)time_anim / (steps-lastchecksteps),
            1e3*(float)time_intlist / (steps-lastchecksteps),
            1e3*(float)time_checklist / (steps-lastchecksteps),
            1e3*(float)time_energy / (steps-lastchecksteps),
            (float)ninttotal/N/(steps-lastchecksteps),
            (float)(steps-lastchecksteps)/(curtime-time_check),
            (float)ninttotal/(curtime-time_check)/1e6,
	    omp_get_max_threads()); 
      printf("!!test: time_anim = %e, time_intlist = %e, time_velstep = %e, drawms = %e\n",time_anim,time_intlist,time_velstep, drawms);
       if (time_anim < 0.1*(time_intlist + time_velstep + time_energy) && drawms < 1000)
       {
         //drawms *= 0.98;
         printf("!!now drawing every %.3f ms\n",drawms);
       }
       if (time_anim > 0.2*(time_intlist + time_velstep + time_energy))
       {
         //drawms /= 0.98;
         //printf("!!now drawing every %.3f ms\n",drawms);
       }
       time_velstep=time_intlist=time_posstep=time_anim=time_energy=time_checklist=0;
       lastchecksteps=steps;
       time_check=myclock();
       ninttotal=0;
 }
     // do wall collisions and accumulate impulse 
     for (i=0; i<N; i++)
     {   
       if ( (pos[i].y > L/2 && v[i].y > 0) || ((pos[i].y < -L/2 && v[i].y < 0) ) ) 
       {
         v[i].y=-v[i].y;
         J=J+2*fabs(v[i].y)*m[i];
       }
       if ( (pos[i].z > L/2 && v[i].z > 0) || ((pos[i].z < -L/2 && v[i].z < 0) ) ) 
       {
         v[i].z=-v[i].z;
         J=J+2*fabs(v[i].z)*m[i];
       }
       if ( (pos[i].x > L/2 && v[i].x > 0) || ((pos[i].x < -L/2 && v[i].x < 0) ) ) 
       {
         v[i].x=-v[i].x;
         J=J+2*fabs(v[i].x)*m[i];
       }
     }

     Tnow=0;
     for (i=0; i<N; i++)
     {
       Tnow += 0.5 * m[i] * v[i]*v[i] * dt;
     }
     Taccum+=Tnow;
     Tnow/=N;

     // determine cooling/heating rate

     rate = (Tnow-Ttarget)*smult;

     if (Tnow > Ttarget)
     {
       for (i=0; i<N; i++)
       {
         v[i] = v[i] * (1 - rate*dt);
       }
     }
     if (t > next_thermo)
     {
       P=J/(thermo_interval)/(6*L*L);
       T=Taccum * 2.0 / 3.0 / N / thermo_interval;
       fprintf(logfile,"%e %e %e %e %e\n",t,P*L*L*L,N*T,P*L*L*L/(N*T),KE+PE);
       J=Taccum=0;
       next_thermo = t + thermo_interval;
     }
  }
  fprintf(logfile,"# Shutting down\n");
  printf("Q\n");
}
