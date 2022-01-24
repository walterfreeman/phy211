#include <stdio.h>
#include <unistd.h>
double x;
double y;



void lx(double d)
{
  printf("c %f %f .02\n",x,y);
  printf("l %f %f %f %f\n",x,y,x+d,y);
  printf("l %f %f %f %f\n",x+d*.55,y,x+d*.45,y+d*.05);
  printf("l %f %f %f %f\n",x+d*.55,y,x+d*.45,y-d*.05);
  printf("l %f %f %f %f\n",x+d*.45,y+d*.05,x+d*.45,y-d*.05);
  x+=d;
}

void ly(double d)
{
  printf("c %f %f .02\n",x,y);
  printf("l %f %f %f %f\n",x,y,x,y+d);
  printf("l %f %f %f %f\n",x,y+d*.55,x+d*.05,y+d*.45);
  printf("l %f %f %f %f\n",x,y+d*.55,x-d*.05,y+d*.45);
  printf("l %f %f %f %f\n",x+d*.05,y+d*.45,x-d*.05,y+d*.45);
  y+=d;
}

void lz(double d)
{
  printf("c %f %f .02\n",x,y);
  printf("l %f %f %f %f\n",x,y,x+d*.7,y+d*.7);
  printf("l %f %f %f %f\n",x+d*.4,y+d*.4,x+d*.315,y+d*.385);
  printf("l %f %f %f %f\n",x+d*.4,y+d*.4,x+d*.385,y+d*.315);
  printf("l %f %f %f %f\n",x+d*.315,y+d*.385,x+d*.385,y+d*.315);
  x+=d*.7;
  y+=d*.7;
}

void lt(double d)
{
  printf("c %f %f .02\n",x,y);
  printf("l %f %f %f %f\n",x,y,x-d*.1,y+d*.1);
  printf("l %f %f %f %f\n",x-d*.2,y+d*.2,x-d*.3,y+d*.3);
  printf("l %f %f %f %f\n",x-d*.4,y+d*.4,x-d*.5,y+d*.5);
  printf("l %f %f %f %f\n",x-d*.6,y+d*.6,x-d*.7,y+d*.7);
  printf("l %f %f %f %f\n",x-d*.385,y+d*.385,x-d*.315,y+d*.385);
  printf("l %f %f %f %f\n",x-d*.385,y+d*.385,x-d*.385,y+d*.315);
  printf("l %f %f %f %f\n",x-d*.315,y+d*.385,x-d*.385,y+d*.315);
  x-=d*.7;
  y+=d*.7;
}

void main()
{
  int i=9; 
  printf("S 2\n");

  while (1==1)
  {
   
  x=-.5; y=-.5;
if (i==0){  lx (1);}
if (i==1){  ly(1); lx(1); ly(-1);}  // staple
if (i==2){  ly(1); lx(1); ly(-1); lx(-1);}  // plaquette
if (i==3){  ly(1); lz(1); lx(1); ly(-1); lz(-1); lx(-1);} // pgram
if (i==4){  ly(1); ly(1); lx(1); ly(-1); ly(-1); lx(-1);} // rect 
if (i==5){  ly(1); ly(1); lx(1); ly(-1); ly(-1);} // Lepage
if (i==6){  x=-1.5; lx(1); lx(1); lx(1);} // Naik
if (i==7){  ly(1); lz(1); lx(1); ly(-1); lz(-1);} // 5link"
if (i==8){  ly(1); lz(1); lt(0.6); lx(1); ly(-1); lz(-1); lt(-0.6);} // 5link"
if (i==9){  ly(1); lz(1); lx(1);  lz(-1); ly(-1); lx(-1);} // chair"
  lx(0);
  printf("F\n");
}}
