typedef struct vector {
  double x;
  double y;
  double z;
} vector;

double drnd48(void);
vector rvec (double mag);
void vipadd(vector& A, vector& B);
void vipmadd(vector& A, vector B, double C);
vector vmadd(vector& A, vector& B, double C);
void vipm(vector& A, double B);
void printvec(vector v);
void printveccr(vector v);
double vmag(vector& A);
void vclear(vector& A);
vector vm(vector& A, double B);
