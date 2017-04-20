#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int bessel_n=1;
int bessel_m=4;
double stretch=1.5;                     // ratio between stretched and unstretched dimensions of membrane
double damp=1;                          // damping coefficient for letting discretization errors settle
double damptime=3;                     // amount of time to let discretization errors in initial conditions settle
double plucktime=0.2;                   // duration mallet applies force
double plucksize=0.3;                   // size of mallet
double pluckstrength=4e-2;               // how hard to thump
double pluckpos=0.0;                    // how far out to thump
double saturation=2;                    // color saturation for drawing pretty pictures
double timestep=8e-2;                   // rk4 integration timestep
int sampint=10;                        // how many timesteps between waveform samples
double sample_amp=400;                  // amplitude of recorded waveform
int NR=40;                             // number of radial nodes
int Nphi=56;                           // number of angular nodes
int draw_int=2;                        // how many RK4 integration steps per graphical frame update
double modulus = 1;                     // elastic modulus
double maxz=1e-4;                       // thing to automate red/blue color drawing, will change after first frame
double z_exagg=5000;                     // factor by which to exaggerate z-displacement in gfx render

typedef struct state_t
{
  double *x, *y, *z, *vx, *vy, *vz;
} state; // can be used for slopes too

void allocate(state &st)
{
  st.x = (double *)malloc(sizeof(double)*(NR+1)*Nphi); 
  st.y = (double *)malloc(sizeof(double)*(NR+1)*Nphi); 
  st.z = (double *)malloc(sizeof(double)*(NR+1)*Nphi); 
  st.vx= (double *)malloc(sizeof(double)*(NR+1)*Nphi); 
  st.vy= (double *)malloc(sizeof(double)*(NR+1)*Nphi); 
  st.vz= (double *)malloc(sizeof(double)*(NR+1)*Nphi); 
}

void evolve (state &st, state &sl, state &st2, double dt)
{
  static int i,j;
  for (i=0; i<NR; i++)
  for (j=0; j<Nphi; j++)
  {
    st2.x[i*Nphi+j] = st.x[i*Nphi+j] + sl.x[i*Nphi+j] * dt;
    st2.y[i*Nphi+j] = st.y[i*Nphi+j] + sl.y[i*Nphi+j] * dt;
    st2.z[i*Nphi+j] = st.z[i*Nphi+j] + sl.z[i*Nphi+j] * dt;
    st2.vx[i*Nphi+j] = st.vx[i*Nphi+j] + sl.vx[i*Nphi+j] * dt;
    st2.vy[i*Nphi+j] = st.vy[i*Nphi+j] + sl.vy[i*Nphi+j] * dt;
    st2.vz[i*Nphi+j] = st.vz[i*Nphi+j] + sl.vz[i*Nphi+j] * dt;
   
  }
}

double get_k(double i)
{
  return (2*M_PI*(double)(i+0.5)/NR)/Nphi / (1.0/NR) * modulus;
}

double get_kr(double i)
{
  return (1.0/NR) / ((2*M_PI*(double)i/NR)/Nphi) * modulus / M_PI;
}

// why the hell this isn't in math.h is beyond me
double hypot3(double x, double y, double z)
{
  return sqrt(x*x+y*y+z*z);
}

// find force on st, store in sl
void euler (state &st, state &sl)
{
  static double r;
  static int i1,i2;
  static double k, req;
  static int i,j; // i is radius, j is angle
  static double Fx=0, Fy=0, Fz=0, F;
  static double dx,dy,dz,m;
//#pragma omp parallel for private shared(sl)
  for (i=1; i<NR; i++)
    for (j=0;j<Nphi;j++)
    {
      // radial guys
      req=1.0/NR;
      // spring constant should be spacing / length 
      k=get_k(i);
      Fx=Fy=Fz=0;
      dx=-st.x[i*Nphi+j]+st.x[(i+1)*Nphi+j];
      dy=-st.y[i*Nphi+j]+st.y[(i+1)*Nphi+j];
      dz=-st.z[i*Nphi+j]+st.z[(i+1)*Nphi+j];
      r=hypot3(dx,dy,dz);
      Fx+=dx*(r-req)*k;     // outward
      Fy+=dy*(r-req)*k;
      Fz+=dz*(r-req)*k;
      
      if (i != 1)
      {
      k=get_k(i-1);
        //k=1.0/NR * modulus; 
        dx=-st.x[i*Nphi+j]+st.x[(i-1)*Nphi+j]; // inward
        dy=-st.y[i*Nphi+j]+st.y[(i-1)*Nphi+j];
        dz=-st.z[i*Nphi+j]+st.z[(i-1)*Nphi+j];
      r=hypot3(dx,dy,dz);
      Fx+=dx*(r-req)*k;    
      Fy+=dy*(r-req)*k;
      Fz+=dz*(r-req)*k;
      }
      else
      {
        req=2.0/NR; // connections crossing center
        k=(2*M_PI*1.2/NR)/Nphi / (2.0/NR) * modulus; 
        i1=i*Nphi+j;
        i2=i*Nphi+(j+Nphi/2)%Nphi;
        dx=st.x[i2]-st.x[i1];
        dy=st.y[i2]-st.y[i1];
        dz=st.z[i2]-st.z[i1];
      r=hypot3(dx,dy,dz);
      Fx+=dx*(r-req)*k;     // outward
      Fy+=dy*(r-req)*k;
      Fz+=dz*(r-req)*k;
      }

      // angular guys
      req=2*M_PI*(i/(double)NR)/Nphi;
      k=get_kr(i);
      i1=i*Nphi+j;
      i2=i*Nphi+(j+1)%Nphi;
      dx=st.x[i2]-st.x[i1];
      dy=st.y[i2]-st.y[i1];
      dz=st.z[i2]-st.z[i1];
      r=hypot3(dx,dy,dz);
      Fx+=dx*(r-req)*k;     // cw
      Fy+=dy*(r-req)*k;
      Fz+=dz*(r-req)*k;

      i1=i*Nphi+j;
      i2=i*Nphi+(j-1+Nphi)%Nphi;
      dx=st.x[i2]-st.x[i1];
      dy=st.y[i2]-st.y[i1];
      dz=st.z[i2]-st.z[i1];
      r=hypot3(dx,dy,dz);
      Fx+=dx*(r-req)*k;     // ccw
      Fy+=dy*(r-req)*k;
      Fz+=dz*(r-req)*k;

      m = (double)i / (double)NR / (double)Nphi; // mass of each point inversely proportional to local density of points to give constant density
      sl.vx[i*Nphi+j] = Fx/m - damp*st.vx[i*Nphi+j];
      sl.vy[i*Nphi+j] = Fy/m - damp*st.vy[i*Nphi+j];
      sl.vz[i*Nphi+j] = Fz/m - damp*st.vz[i*Nphi+j];
      sl.x[i*Nphi+j]=st.vx[i*Nphi+j];
      sl.y[i*Nphi+j]=st.vy[i*Nphi+j];
      sl.z[i*Nphi+j]=st.vz[i*Nphi+j];
    }
}


void init_drum(state &st, double rm)
{
  int i,j;
  double phi;
  for (i=1; i<=NR; i++)
    for (j=0;j<Nphi;j++)
    {
      st.vx[i*Nphi+j]=0;
      st.vy[i*Nphi+j]=0;
      st.vz[i*Nphi+j]=0;
      phi=j*2*M_PI/Nphi;
      st.x[i*Nphi+j] = rm*sin(phi)*(double)i/NR;
      st.y[i*Nphi+j] = rm*cos(phi)*(double)i/NR;
      st.z[i*Nphi+j] = 0;
    }
}

double get_bessel_zero(int m, int n)
{
  double zeroes[30]={2.4048,3.8317,5.1356,6.3802,7.5883,8.7715,5.5201,7.0156,8.4172,9.7610,11.0647,12.3386,8.6537,10.1735,11.6198,13.0152,14.3725,15.7002,11.7915,13.3237,14.7960,16.2235,17.6160,18.9801,14.9309,16.4706,17.9598,19.4094,20.8269,22.2178};
  int index=(n-1)*6+m;
  if (m>5 || n>5)
  {
    fprintf(stderr,"Higher Bessel function order requested than in my hacked-together table of the zeroes of the Bessel functions; you'll have to fix that. Exiting...\n"); exit(-1);
  }
  return zeroes[index];
}

void init_bessel(state &st,int m, int n) // m angular number, n radial number
{
  double zero=get_bessel_zero(m,n);
  int i,j;
  for (i=1; i<=NR; i++)
    for (j=0;j<Nphi;j++)
    {
      double phi=2*M_PI*(double)j/Nphi;
      double r=hypot(st.x[i*Nphi+j],st.y[i*Nphi+j])/stretch;
      st.z[i*Nphi+j] = jn(m,zero*r)*0.0001*cos(m*phi);
    }
  printf("!Initializing with Bessel function: angular number %d, radial number %d, zero %f\n",m,n,zero);
}



void draw(state &st)
{
   int i,j;
   for (i=1; i<=NR; i++)
    for (j=0;j<Nphi;j++)
     {
       // anim drawing commands -- connect the dots
       if (fabs(st.z[i*Nphi+j]) > maxz) maxz=fabs(st.z[i*Nphi+j]);
       printf("C %e %e %e\n",1+st.z[i*Nphi+j]/maxz*saturation,1.0-fabs(st.z[i*Nphi+j]/maxz*saturation),1-st.z[i*Nphi+j]/maxz*saturation);
       printf("c3 %e %e %e 0.003\n",st.x[i*Nphi+j],st.y[i*Nphi+j],st.z[i*Nphi+j]*z_exagg);
       if (i<NR) printf("l3 %e %e %e %e %e %e\n",st.x[i*Nphi+j],st.y[i*Nphi+j],st.z[i*Nphi+j]*z_exagg,st.x[(i+1)*Nphi+j],st.y[(i+1)*Nphi+j],st.z[(i+1)*Nphi+j]*z_exagg);
       printf("l3 %e %e %e %e %e %e\n",st.x[i*Nphi+j],st.y[i*Nphi+j],st.z[i*Nphi+j]*z_exagg,st.x[i*Nphi+(j+1)%Nphi],st.y[i*Nphi+(j+1)%Nphi],st.z[i*Nphi+(j+1)%Nphi]*z_exagg);
       if (i==1) printf("l3 %e %e %e %e %e %e\n",st.x[i*Nphi+j],st.y[i*Nphi+j],st.z[i*Nphi+j]*z_exagg,st.x[i*Nphi+(j+Nphi/2)%Nphi],st.y[i*Nphi+(j+Nphi/2)%Nphi],st.z[i*Nphi+(j+Nphi/2)%Nphi]*z_exagg);
     }
   printf("F\n");
}

void pl(state &st, double pluck)
{
  for (int i=1;i<NR;i++)
  {
    for (int j=0;j<Nphi;j++)
    {
      // some sort of gaussian -- normalization is hacky
      double plstr=exp(-pow(hypot(st.x[i*Nphi+j]-pluckpos*stretch,st.y[i*Nphi+j]),2)/plucksize)*pluck/(plucksize);
      st.vz[i*Nphi+j]+=plstr/(NR*Nphi);
    }
  }
}

int main(int argc, char **argv)
{
  printf("S3 6\nS 1.8\n");
  int pluck_cond=0;
  int bessel_cond=0;
  if (argc < 4) {printf("!Usage: <this> [b <N> <L>] or [t <thump length> <thump position> <thump size> <thump strength>]\n"); exit(0);}

  if (argv[1][0]=='b') {printf("!Bessel function start condition\n"); bessel_cond=1;
  bessel_m=atoi(argv[3]); bessel_n=atoi(argv[2]);
  printf("!Radial mode number: %d\t\tAngular mode number: %d\n",bessel_n,bessel_m);
  }
  else if (argv[1][0]=='t') {printf("!Drum thump start condition\n"); pluck_cond=1;
  
    plucktime=atof(argv[2]);
    pluckpos=atof(argv[3]);
    plucksize=atof(argv[4]);
    pluckstrength=atof(argv[5])*sqrt(plucksize)/5;
    
  }
  else {printf("!Usage: <this> [b <N> <L>] or [t <thump length> <thump position> <thump size> <thump strength>]\n"); exit(0);}

  double pluck=0;
  double ringbuffer[1000];
  int rbp=0;
  for (int i=0;i<1000;i++) ringbuffer[i]=0;
  state st,stt,sl1,sl2,sl3,sl4;
  allocate(st);
  allocate(stt);
  allocate(sl1);
  allocate(sl2);
  allocate(sl3);
  allocate(sl4);
  init_drum(st,stretch);
  init_drum(stt,stretch);
  int frame=0;
  double dt;
  double timestep_mult=1;
  for (double t=0;1;)
  {
    frame++;
    timestep_mult=1;
    if (t>damptime) damp=0;
    if (t>damptime && t<damptime+plucktime && pluck!=-1) pluck=pluckstrength/plucktime; else pluck=0;
    if (pluck>0 && pluck_cond) {pl(st,pluck*timestep);}
    if (pluck > 0 && bessel_cond) {init_bessel(st,bessel_m,bessel_n); pluck=-1;}
    dt=timestep*timestep_mult;
    t+=dt;
    // rk4
    euler(st,sl1);
    evolve(st,sl1,stt,dt/2);
    euler(stt,sl2);
    evolve(st,sl2,stt,dt/2);
    euler(stt,sl3);
    evolve(st,sl3,stt,dt);
    euler(stt,sl4);
    evolve(st,sl1,st,dt/6);
    evolve(st,sl2,st,dt/3);
    evolve(st,sl3,st,dt/3);
    evolve(st,sl4,st,dt/6);
    if (frame % sampint == 0)
    {
      ringbuffer[rbp]=0;
      for (int i=1;i<NR;i++) for (int j=0;j<Nphi;j++) {
        ringbuffer[rbp]+=st.z[i*Nphi+j] / (Nphi*NR)*sample_amp;
      }
      rbp=(rbp+1)%1000;
    }
    if (frame % draw_int == 0)
    {
  //    printf("T 0 0.8\nt=%.4g\nC 0 1 0",t);
      // draw waveform history using ring buffer, gets the job done
    //  for (int i=1;i<999;i++)
    //  {
    //    printf("l %e %e %e %e\n",(500-i)/500.,-0.8+ringbuffer[(rbp-i+1000)%1000],(500-(i+1))/500.,-0.8+ringbuffer[(rbp-(i+1)+1000)%1000]);
    //  }
      draw(st);
    }
  }
} 
