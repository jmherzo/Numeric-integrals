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
#include <omp.h>

int n = 100000;
double a = 0.0;
double b = 5.0;

#define NUM_THREADS 9

double funcDeX (double x);

double funcDeX (double x)
{
	return (4.0/ (1.0 + x*x));
}


int main()
{
	int i, nthreads, sum[NUM_THREADS];
	double h, x1[n], x2[n], x, xi0, xi1, xi2, res, t1, t2, tiempo;
	
	omp_set_num_threads(NUM_THREADS);
	const double startTime = omp_get_wtime();
	t1 = omp_get_wtime();


//PASO 1
	h = (b - a)/n;


//PASO 2	
	xi0 = funcDeX(a) + funcDeX(b);
	xi1 = 0.0;
	xi2 = 0.0;

	#pragma omp parallel
	{
		int i, nthrds, id;

		id = omp_get_thread_num();
		nthrds = omp_get_num_threads();
		if (id==0) nthreads=nthrds;

//Paso 				
		for(i = id + 1; i < n; i = i + nthrds)
		{
			x = a + (double)i*h;
			if(i%2 == 0){
				x2[i] = funcDeX(x);
				x1[i] = 0;
			}
			else{
				x1[i] = funcDeX(x);
				x2[i] = 0;
			}
			sum[id]+=1;
		}
	}
	
	for(i = 1; i < n; i++)
	{
		if(i%2 == 0)
			xi2 += x2[i];
		else
			xi1 += x1[i];
	}	

	for (i=0; i<NUM_THREADS; i++)
		printf("EL thread %d hizo: %d operaciones\n",i,sum[i]);

	res = (h/3.0)*(xi0 + 2.0*xi2 + 4.0*xi1);

	t2=omp_get_wtime();
	const double endTime = omp_get_wtime();
	tiempo=t2-t1;

	printf("Se usaron %d threads \nEn un tiempo de %lf segundos\nRespuesta: %f\n", NUM_THREADS, tiempo, res);
}
