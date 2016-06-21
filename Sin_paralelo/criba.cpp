// Programa elaborado por Carlos Eduardo Mesen y Andres Sanchez

/*----------------------------------------------------------------------
 * Escriba un programa que calcule los primeros 5000 números primos. 
 * Se debe medir el tiempo de ejecución del programa.
 * Version 1 : Utilizando la Criba de Eratóstenes encuentre los números,
 * sin utilizar ningún tipo de paralelismo
 * Para correrlo : g++ -o out criba.cpp
 * 				   ./out
 * 				   Numero limite de numeros: 5000

 * -------------------------------------------------------------------*/
 
 
 #include <iostream>
 #include <ctime>

 
 
 using namespace std;
 
 // Se inicia el main 
 
 int main (){
	 
	 
	 
	 int start_s=clock(); // Empieza el reloj
	 
	 
	 int n,i,j;
	 
	 cout << "Numero limite de numeros: ";
	 cin >> n;
	 
	 bool prime[n];
	 

	
	 
	 
	 for (i=0; i<n ; i++){
		 
		 prime[i] = true;
		 
		 }
		 
	
		
	for (i=2; i<n; i++){
		for (j=2; i*j < n ; j++){
				 
			 prime[i*j] = false;
			 
		 }
			 
	 }
	
	
	 
	 for (i=2; i<n ; i++){
		 
		 if (prime[i]) cout << i << ' ' ;
		 
		 } 
	 
	 
	 cout << endl;

	int stop_s=clock(); //Termina el reloj 
	cout << "tiempo en milisegundos: " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 << endl;
	 
	 
	 }
