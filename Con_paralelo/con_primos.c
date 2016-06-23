// Programa elaborado por Carlos Eduardo Mesen y Andres Sanchez

/*----------------------------------------------------------------------
 * Escriba un programa que calcule los primeros 5000 números primos. 
 * Se debe medir el tiempo de ejecución del programa.
 * Version 1 : Utilizando la Criba de Eratóstenes encuentre los números,
 * sin utilizar ningún tipo de paralelismo.
 * 
 * NOTA: Para poder comparar los tiempo de ejecución se utiliza la 
 * función MPI_Wtime() y ambos códigos se compilan usando mpicc.
 *  
 * Para correrlo : mpicc con_primos.c  -o out
 * 				   mpirun -np 20 out

 * 	 IMPRIME  "Existen 5000 numeros primos menores que 48613
El tiempo total de ejecucion es : 3.488064 milisegundos"

 * -------------------------------------------------------------------*/





#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Dado que el algoritmo de la criba de Eratostenes va poniendo 
//cual numero es o no primo en un rango establecido se averigua 
//el ultimo valor del rango para que sean calculados los primeros 
//5000 numeros primos 

const int n=48613;   // El primo #5000 => 48611

//Fuente: http://www.primber.com/first-5000-prime-number.html48611


 //*********************************************************************
 //	*   				MAIN
 //*********************************************************************
 

int main (int argc, char *argv[]) {


//----------------------------------------------------------------------
// 	INICIALIZACION DE VARIABLES
//----------------------------------------------------------------------

    long cuenta_temporal=0;
    long cuenta_total=0;     //variables que mantienen la cuenta de primos 
    long inicio;
    long final;      //rango de numeros primos por proc
    long i;

    int pid;          //id de proc
    int nump;          //numero de proc
    int tam, tam_proc0;   // cantidad de numeros a revisar por proc
	double tiempo;         // variable que lleva la cuenta del tiempo de ejecuacion

//----------------------------------------------------------------------
// 	AMBIENTE MPI
//----------------------------------------------------------------------


//---------------------INICIO MPI --------------------------------------
    
    MPI_Init (NULL, NULL); // Inicializa ambiente de paralelizacion MPI

    MPI_Comm_rank (MPI_COMM_WORLD, &pid); // Cual proc se esta llamando
        
    MPI_Comm_size (MPI_COMM_WORLD, &nump); // Tamano comdato
    
    MPI_Barrier(MPI_COMM_WORLD); // Garantiza codigo procesado ~ al mismo tiempo
    
//-----------------------INICIA TEMPORIZADOR----------------------------

    tiempo = -MPI_Wtime();

    
    inicio= 2 + pid*(n-1)/nump;  //rango de los numeros calculary tam para c/proc
    final = 1 + (pid+1)*(n-1)/nump;
    tam = final - inicio+ 1;

    //calcula tam para proc 0
    tam_proc0 = (n-1)/nump;

    int j;
    // para cada numero en rango   
    for (j=inicio; j<=final; j++){
     
		if (primo(j)==0){ // Si el numero es primo incrementa contador 
			cuenta_temporal++;
		}
     }

     //Sincroniza los contadores
         MPI_Reduce (&cuenta_temporal, &cuenta_total, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

     //Actualiza el temporizador 
     tiempo += MPI_Wtime();
//-----------------------FINALIZA TEMPORIZADOR--------------------------

	// FOR DEBUG : IMPRIME EL NUMERO DE PROCESO UTLIZADO
    // printf(" Finaliza proceso %d \n", pid); 



     // Utiliza el proceso 1 para imprimir la cuenta y el tiempo que duro 
     if (pid == 0) {
            printf ("Existen %ld numeros primos menores que %d\n", cuenta_total, n);
        printf ("El tiempo total de ejecucion es :  %f milisegundos\n", tiempo*1000);
     }

     MPI_Finalize ();
//--------------------------FIN MPI-------------------------------------
     return 0;
   }


 //*********************************************************************
 //	   				FUNCION : CALCULO NUMERO PRIMO
 // Retorna un cero si el numero es primo y un uno si no es primo 
 //
 //*********************************************************************


int primo (int num){

int val=0;

if(num==1)
        val = 1;

   else if(num%2 == 0 && num!=2)
        val = 1;

    if(val != 1) {
    int j;
    for(j=3; j<num; j+=2) {
                if(num%j == 0 ){
                        val = 1;
                        break;
                }
                if(j*j>num) break;
            }
    }

if(val == 0)
    return 0;
else 
    return 1;
}
