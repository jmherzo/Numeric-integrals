/****************************************************************/
/* Module : romberg.c 						*/
/* Section: 5.3							*/
/* Cheney-Kincaid, Numerical Mathematics and Computing, 5th ed, */
/* Brooks/Cole Publ. Co.                                        */
/* Copyright (c) 2003.  All rights reserved.                    */
/* For educational use with the Cheney-Kincaid textbook.        */
/* Absolutely no warranty implied or expressed.                 */   
/* 								*/
/* Description: Compute an integral using romberg array, tested */
/*		using 3 distinct functions			*/
/*								*/
/****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double F (double x);
double G (double x);
double P (double x);
void romberg(double f(double), double a, double b, int n, double **R);

double F (double x)
{
  return (1.0/ (1.0 + x));
}

double G (double x)
{ 
   return (exp(x));
}

double P (double x)
{
   return (sqrt(x));
}

 void romberg(double f(double), double a, double b, int n, double **R)
{
  int i, j, k;
  double h, sum;


  h = b - a;
  R[0][0] = 0.5 * h * (f(a) + f(b));   
  printf(" R[0][0] = %f\n", R[0][0]);

  for (i = 1; i <= n; i++)
  { 
     h *= 0.5;
     sum = 0;
     for (k = 1; k <= pow(2,i)-1; k+=2)
     {
       sum += f(a + k * h);
     } 
     R[i][0] = 0.5 * R[i-1][0] + sum * h;  
     printf(" R[%d][0] = %f\n", i, R[i][0]);
     for (j = 1; j <= i; j++)
     {
       R[i][j] = R[i][j-1] + (R[i][j-1] - R[i-1][j-1]) / (pow(4,j)-1); 
       printf(" R[%d][%d] = %f\n", i,j, R[i][j]);
     }
   }
}

void main()
{
  int n = 10;
  int i;
  double **R;
  double F(double), G(double), P(double);

  R = calloc((n+1), sizeof(double *));
  for (i = 0; i <= n; i++)
    R[i] = calloc((n+1), sizeof(double));
  printf("The first function is F(x) = 1/(1 + x)\n");
  romberg(F, 0.0, 2.0,3, R);
  printf("The second function is G(x) = exp(x)\n");
  romberg(G,-1.0, 1.0, 4, R);
  printf("The third function is P(x) = sqrt(x)\n");
  romberg(P,0.0, 1.0, 7, R);
}
