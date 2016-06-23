// Programa elaborado por Carlos Eduardo Mesen y Andres Sanchez

/*----------------------------------------------------------------------
 * Escriba un programa que calcule los primeros 5000 números primos. 
 * Se debe medir el tiempo de ejecución del programa.
 * Version 1 : Utilizando la Criba de Eratóstenes encuentre los números,
 * sin utilizar ningún tipo de paralelismo.
 * 
 *  NOTA: Para poder comparar los tiempo de ejecución se utiliza la 
 * función MPI_Wtime() y ambos códigos se compilan usando mpicc.
 * 
 * Para correrlo : mpicc sin_primos.c  -o out
 * 				   ./out
 * 
 * IMPRIME  "Existen 5000 numeros primos menores que 48613
*		El tiempo total de ejecucion es :  5.388975 milisegundos"

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

    long cuenta=0;
    double tiempo;  //variable que lleva el tiempo de ejecucion 
    

//----------------------------------------------------------------------
// 	AMBIENTE MPI
//----------------------------------------------------------------------


//---------------------INICIO MPI --------------------------------------
    
    MPI_Init (NULL, NULL); // Inicializa ambiente de paralelizacion MPI

 
    
//-----------------------INICIA TEMPORIZADOR----------------------------

    tiempo = -MPI_Wtime();


    int j;
    // para cada numero en rango   
    for (j=0; j<=n; j++){
     
		if (primo(j)==0){ // Si el numero es primo incrementa contador 
			cuenta++;
		}
     }

     //Actualiza el temporizador 
     tiempo += MPI_Wtime();
     printf ("Existen %ld numeros primos menores que %d\n", cuenta, n);
     printf ("El tiempo total de ejecucion es :  %f milisegundos\n", tiempo*1000);
//-----------------------FINALIZA TEMPORIZADOR--------------------------
     //terminate the execution environment
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
