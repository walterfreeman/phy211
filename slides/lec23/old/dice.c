#include <stdio.h>
#include <stdlib.h>

int die(int n)
{
  int m;
  if (n>0) m=rand()%n+1; else m=rand()%4-1;
  if (m==n) {m += die(n);}
  return m;
}

void main()
{
  int dietypes[6];
  dietypes[0]=4;
  dietypes[1]=6;
  dietypes[2]=8;
  dietypes[3]=10;
  dietypes[4]=12;
  dietypes[5]=0;
  int n=8;
  int accum=0;
  int trials=1000000;
  int result[50];
  double avg=0;
  int i,j;
  int r,w;

  for (j=0;j<6;j++)
  {
  printf("#cm %d\n",j);
  avg=0; accum=0;
  for (i=0;i<50;i++) result[i]=0;
  for (i=0;i<trials;i++)
  {
    r=die(dietypes[j]);
    w=die(6);
    if (w>r) r=w;
    avg += r;
    if (r>=50) r=49;
    result[r]++;
  }
  
  for (i=0;i<20;i++) {
  accum += result[i];
  printf("%d %f\n",i,dietypes[j],1-(float)accum/(float)trials);}
//  printf("%d d%d : %f\n",i,dietypes[j],1-(float)accum/(float)trials);}
//  printf("Average is %f.\n",avg/trials);
  }
}
