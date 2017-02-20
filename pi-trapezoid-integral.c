#include <stdio.h>

static long num_pasos=10; //Numero de pasos
double paso;

int main(){
    double x, pi, sum=0.0; //constantes
    paso= 1.0/num_pasos; //Paso a evaluar
    for ( int i=0; i<num_pasos; i++){
        x=(i+0.5)*paso;//Define el punto a evaluar en la funcion
        printf("x=%lf\n",x);        
        sum+=4/(1+x*x);
        printf("sum=%lf\n",sum);
    }
    pi = paso*sum;
    printf("pi = (%lf)\n",pi); //imprime resultado
}
