/**
Para aproximar la integral numérica se puede emplear el método de Simpson cuyo algotimo es el
siguiente:
Sea f(x)dx, donde a, b son los límites de integración, y se define un nímero par n de pasos
de integración, entonces:

Paso 1 Se define a h = (b − a)/n

Paso 2 Se inicializan variables:
XI0 = f(a) + f(b)
XI1 = 0
XI2 = 0

Paso 3 Para i = 1, ..., n − 1 hacer los pasos 4 a 5

Paso 4 se define a X = a + ih

Paso 5 S´ı i es par XI2 = XI2 + f(X) de lo contrario XI1 = XI1 + f(X)

Paso 6 Se define a XI = h*3*(XI0 + 2XI2 + 4XI1)

Paso 7 Escribir XI. FIN

Después de su implementación mediante algoritmos de paralelización, determine la escalabilidad del
sistema en base al tiempo de ejecución y el número de “threads”(speedup) y compare sus resultados
con los que se obtienen con el algortimo del trapecio (analizado en clase).

**/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define NUM_THREADS 1

double funcDeX (double x);

double funcDeX (double x)
{
	return (1.0/ (1.0 + x));
}

void main()
{
  	int n = 3;
	int i,j,k;
	double **R,h,sum;
	double t1, t2, total;

	double a=0,b=2;

	R = calloc((n+1), sizeof(double *));
	  
	for (i = 0; i <= n; i++)
 		R[i] = calloc((n+1), sizeof(double));

	omp_set_num_threads(NUM_THREADS);
	const double startTime = omp_get_wtime();
	t1 = omp_get_wtime();

  	h = b - a;
  	R[0][0] = 0.5 * h * (funcDeX(a) + funcDeX(b));   

	int nthreads;
	int mult = 0;
	
	/**#pragma omp parallel ordered
	{
		int i, nthrds, id;
		id = omp_get_thread_num();
		nthrds = omp_get_num_threads();
		if (id==0) nthreads=nthrds;**/

		#pragma omp ordered
		{
		int i, nthrds, id;
		id = omp_get_thread_num();
		nthrds = omp_get_num_threads();
		if (id==0) nthreads=nthrds;

			for(i = id +1; i <= n; i = i + nthrds)
			{
				h *= 0.5;
				sum = 0;
				for (k = 1; k <= pow(2,i)-1; k+=2)
				{
					sum += funcDeX(a + k * h);
				} 
				R[i][0] = 0.5 * R[i-1][0] + sum * h;  

				for (j = 1; j <= i; j++)
				{
					R[i][j] = R[i][j-1] + (R[i][j-1] - R[i-1][j-1]) / (pow(4,j)-1); 
					printf("R[i][j]: %f\n", R[i][j]);

				}
			}
		}
	//}

	t2 = omp_get_wtime(); 
  	total = t2-t1; 
  	printf("Tiempo: %f\n", total);
	printf("Threads: %d\n", NUM_THREADS);

}
