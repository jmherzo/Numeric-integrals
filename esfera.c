#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define PI 3.14159265
#define NUM_THREADS 30

int n = 100000;
double p2 = 4.0;
double p1 = 0.0;
double phi2 = PI/2;
double phi1 = PI/4;
double theta2 = 2*PI;
double theta1 = 0;

int main()
{
	int i, nthreads, sum[NUM_THREADS];
	double hp, ht, hph, xi0p, xi1p[n], xi2p[n], xp, xip, xi0t, xi1t[n], xi2t[n], xt, xit, xi0ph, xi1ph[n], xi2ph[n], xph, xiph, t1, t2, tiempo, x2p, x1p, x2t, x1t, x2ph, x1ph;
	
	omp_set_num_threads(NUM_THREADS);
	const double startTime = omp_get_wtime();
	t1 = omp_get_wtime();

	hp = (p2 - p1)/(double)n;
	
	xi0p = p1*p1 + p2*p2;
	x1p = 0.0;
	x2p = 0.0;

	ht = (theta2 - theta1)/(double)n;
	
	xi0t = 1.0 + 1.0;
	x1t = 0.0;
	x2t = 0.0;	

	hph = (phi2 - phi1)/(double)n;
	
	xi0ph = sin(phi1) + sin(phi2);
	x1ph = 0.0;
	x2ph = 0.0;

	#pragma omp parallel
	{
		int i, nthrds, id;
		
		id = omp_get_thread_num();
		nthrds = omp_get_num_threads();
		if (id==0) nthreads=nthrds;		
		
		for(i = id + 1; i < n; i = i + nthrds)
		{
			xp = p1 + i*hp;
			xt = theta1 + i*ht;
			xph = phi1 + i*hph;

			if(i%2 == 0)
			{
				xi2p[i] = xp*xp;
				xi2t[i] = 1;
				xi2ph[i] = sin(xph);
			}
			else
			{
				xi1p[i] = xp*xp;
				xi1t[i] = 1;
				xi1ph[i] = sin(xph);
			}
			sum[id]+=1;
		}
		
	}
	
	for(i = 1; i < n; i++)
	{
		if(i%2 == 0)
		{
			x2p += xi2p[i];
			x2t += xi2t[i];
			x2ph += xi2ph[i];
		}
		else
		{
			x1p += xi1p[i];
			x1t += xi1t[i];
			x1ph += xi1ph[i];		
		}
	}

	for (i=0; i<NUM_THREADS; i++)
		printf("EL thread %d hizo: %d operaciones\n",i,sum[i]);

	xip = (hp/3.0)*(xi0p + 2.0*x2p + 4.0*x1p);
	xit = (ht/3.0)*(xi0t + 2.0*x2t + 4.0*x1t);
	xiph = (hph/3.0)*(xi0ph + 2.0*x2ph + 4.0*x1ph);

	t2=omp_get_wtime();
	const double endTime = omp_get_wtime();
	tiempo=t2-t1;
	/*
	printf("Resultado: %lf\n", xip*xit*xiph);
	printf("Tiempo: %lf\n", tiempo);
	printf("NUM_THREADS: %d\n", NUM_THREADS);
	*/
	printf("Se usaron %d threads \nEn un tiempo de %lf segundos\nRespuesta: %f\n", NUM_THREADS, tiempo, xip*xit*xiph);

}
