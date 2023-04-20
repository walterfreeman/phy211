#include <stdio.h>
#include <math.h>
#define N 50

  double a = 1./(N);

double grad (double X[N+1][N+1][N+1],int i,int j,int k)
{
  double g;
  g = (  X[i-1][j][k]
      + X[i+1][j][k]
      + X[i][j-1][k]
      + X[i][j+1][k]
      + X[i][j][k-1]
      + X[i][j][k+1]
      - X[i][j][k]*6) / (a*a);
  //printf("Gradient at %d,%d,%d is %f.\n",i,j,k,g);
  return g;
} 

main()
{
  printf ("S %d\n",N*1.2);
  double V[N+1][N+1][N+1];
  double V_half[N+1][N+1][N+1];
  double X[N+1][N+1][N+1];
  double X_half[N+1][N+1][N+1];
  int i,j,k;
  double x,y,z;
  double dt=0.01,t;
  for (i=0;i<=N;i++)
  for (j=0;j<=N;j++)
  for (k=0;k<=N;k++)
  {
    V[i][j][k] = V_half[i][j][k] = X[i][j][k] = X_half[i][j][k] = 0;
  }

  // only update inside the cone

  for (t=0;t<1000;t+=dt)
  {   
  for (i=0;i<=N;i++)   
  {
    for (j=0;j<=N;j++)
    {
      for (k=0;k<=N;k++)
      {
        if (i!=0 && j!=0 && k!=0 && i!=N && j!=N && k!=N)
        {
          X_half[i][j][k] = X[i][j][k] + V[i][j][k]*dt/2;
          V_half[i][j][k] = V[i][j][k] + grad(X,i,j,k)*dt/2;
        }
}}}

for (i=0;i<=N;i++)
  {
    for (j=0;j<=N;j++)
    {
      for (k=0;k<=N;k++)
      {
        if (i!=0 && j!=0 && k!=0 && i!=N && j!=N && k!=N)
        {
          X[i][j][k] = X[i][j][k] + V_half[i][j][k]*dt;
          V[i][j][k] = V[i][j][k] + grad(X_half,i,j,k)*dt;
        }

  if (i == N/2)
  {
    printf("c %d %d %f\n",j-N/2,k-N/2,X[i][j][k]+0.3);
  }
}}}
 fprintf(stderr,"End of frame: time is %f.\n",t);

printf("F\n");
}
}
