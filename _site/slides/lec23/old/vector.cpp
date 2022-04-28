#include <stdio.h>
#include <math.h>
#include "vector.h"
#include <stdlib.h>

vector rvec (double mag)
{
  vector result;
  result.x=drnd48()*mag;
  result.y=drnd48()*mag;
  result.z=drnd48()*mag;
  return result;
}


void vipadd(vector& A, vector& B) // A=A+B;
{
  A.x += B.x;
  A.y += B.y;
  A.z += B.z;
}

double drnd48(void)
{
  return drand48()-0.5;
}


void vipmadd(vector& A, vector B, double C) // A=A+C*B;
{
  A.x += B.x*C;
  A.y += B.y*C;
  A.z += B.z*C;
}

vector vmadd(vector& A, vector& B, double C) // returns A+B*C
{
  static vector res;
  res.x = A.x + B.x*C;
  res.y = A.y + B.y*C;
  res.z = A.z + B.z*C;
  return res;
} 

void vipm(vector& A, double B) // A=A*B;
{
  A.x *= B;
  A.y *= B;
  A.z *= B;
}


void printvec(vector v)
{
  printf("% .7e, % .7e, % .7e",v.x,v.y,v.z);
}

void printveccr(vector v)
{
  printvec(v);
  printf("\n");
}
vector vm(vector& A, double B)
{
  static vector C;
  C.x = A.x*B;
  C.y = A.y*B;
  C.z = A.z*B;
  return C;
}

double vmag(vector& A)
{
  return sqrt(A.x*A.x + A.y*A.y + A.z*A.z);
}

void vclear(vector& A)
{
  A.x=A.y=A.z=0;
}
