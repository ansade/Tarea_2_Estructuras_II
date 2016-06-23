/* 
 
   Compilar en linux con g++: g++ TP2_MESI.cpp -o MESI
   Archivo Necesario: aligned.trace
   Ejecutar en el bash: ./MESSI
*/

#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <bitset>
#include <limits>
#include <vector>
#include <sstream>
#include <ctime>

using namespace std;

//Constantes:

//Datos para crear la cache L1 y L2

//Iguales para ambos niveles de cache.
#define  Columnas        3  //Bit de Válido, Estado y Tag
#define  TamOffset       4

//Cache L1
#define FilasL1       500
#define TamIndiceL1     9	
#define TamTagL1       19

//Cache L2
#define FilasL2     4000
#define TamIndiceL2   12
#define TamTagL2      16

//Estados:
#define M 3
#define E 2
#define S 1
#define I 0
   
//Crea las estructuras de datos para las caches.

//Caches L1:

unsigned long CacheL11[FilasL1][Columnas]; 
unsigned long CacheL12[FilasL1][Columnas];

//Cache L2:

unsigned long CacheL2[FilasL2][Columnas]; 


//Variables Globales
	string sBits_byte_offset, sBits_index, sBits_tag;

// Strings que contienen la direccion hexadecimal del dato leido del archivo (dirHex), 
//los bits en binario de la direccion leida del archivo y la linea leida del archivo (sLine)
	string dirHex, sBits_direccion, sLine; 

string HexStringABinString (string dirHex);

unsigned long misses = 0, hits = 0;

int Fila;

//Funciones Auxiliares

void BinStringAMultipleString (string sBits_direccion, int tam_index, int tam_tag, int tam_byte_offset);

// Función para leer y escribir las caches
void RedWriteCache(unsigned long Cache[][Columnas], int Fila, int LE, unsigned long decimal_tag, unsigned long decimal_index, unsigned long decimal_byte_offset);

void ClearCache(unsigned long Cache[][Columnas], int Fila);


int main(int argc, char *argv[]){

//Variables
	
	int LongCad = 9;
	int LongPalabra = 32;
  int LE=0;
	char Direccion[LongCad];
	char DirTemp[LongCad];
  char LecturaEscritura[LongCad];
	string TipoCache;
  int FlagCPU=0;
	double HitRatio = 0.0, MissRatio = 0.0;

  // Valor decimal del tag,index y byte offset del la linea leida y cantidad de misses y hits en el cache
  unsigned long decimal_tagL1, decimal_indexL1, decimal_byte_offsetL1, decimal_tagL2, decimal_indexL2, decimal_byte_offsetL2,Cont;


//Objeto de tipo ofstream que permite leer el archivo 
  ifstream archivo;
				
//Vacia las caches seteandolas a cero.


Fila = FilasL1;
ClearCache(CacheL11,FilasL1);

ClearCache(CacheL12,FilasL1);

Fila = FilasL2;
ClearCache(CacheL2,FilasL2);


//Apertura del archivo para su lectura
	archivo.open("aligned.trace",ifstream::in);

//Verifica que el archivo se haya abierto correctamente
	if (archivo.fail()){
		cout << "\nError al abrir el archivo\n" << endl;
		return -3;
	}
	else{
		cout << "\nArchivo abierto con éxito\n" << endl;

    int indice0 = 0;
		int indice1 = 0;
//Inicio del ciclo de lectura	
//Obtiene la línea de dirección
		while(archivo.getline(Direccion,LongCad,' ')){
			Cont++;				
   		sBits_byte_offset = ""; // Inicializacion de los strings.
	 		sBits_index = "";
  		sBits_tag = ""; 
 			LE = 0; 
			
			//Obtiene la línea de lectura/escritura
			archivo.getline(LecturaEscritura,LongCad,'\n');

			cout << "Dirección: " << Direccion << endl;
			cout << "Acción: " << LecturaEscritura << endl;

			//Convierte la dirección en hexadecimal a binario
				
			sBits_direccion = HexStringABinString (Direccion);
						
			cout << "Direccion binaria: " << sBits_direccion << endl;

			//Separa la dirección en Tag, Index y Offset para L1
			BinStringAMultipleString (sBits_direccion, TamIndiceL1, TamTagL1, TamOffset);

			cout << "Tag: " << sBits_tag << endl;
			cout << "Index: " << sBits_index << endl;
			cout << "Offset: " << sBits_byte_offset << endl;

//Conversion de bits a decimal index, BO y tag
// Convierte el string que contiene  los bits del tag a un numero decimal.
			decimal_tagL1 = bitset<numeric_limits<unsigned long>::digits>(sBits_tag).to_ulong(); 
	
// Convierte el string que contiene  los bits del índice a un numero decimal.
			decimal_indexL1 = bitset<numeric_limits<unsigned long>::digits>(sBits_index).to_ulong();
	
// Convierte el string que contiene  los bits del offset a un numero decimal.
			decimal_byte_offsetL1 = bitset<numeric_limits<unsigned long>::digits>(sBits_byte_offset).to_ulong();
		
			cout << "Decimal Tag L1: " << decimal_tagL1 << endl;
			cout << "Decimal Index L1: " << decimal_indexL1 << endl;
			cout << "Decimal Offset L1: " << decimal_byte_offsetL1 << endl;

//Separa la dirección en Tag, Index y Offset para L2
			BinStringAMultipleString (sBits_direccion, TamIndiceL2, TamTagL2, TamOffset);

			cout << "Tag: " << sBits_tag << endl;
			cout << "Index: " << sBits_index << endl;
			cout << "Offset: " << sBits_byte_offset << endl;

//Conversion de bits a decimal index, BO y tag
// Convierte el string que contiene  los bits del tag a un numero decimal.
			decimal_tagL2 = bitset<numeric_limits<unsigned long>::digits>(sBits_tag).to_ulong(); 
	
// Convierte el string que contiene  los bits del índice a un numero decimal.
			decimal_indexL2 = bitset<numeric_limits<unsigned long>::digits>(sBits_index).to_ulong();
	
// Convierte el string que contiene  los bits del offset a un numero decimal.
			decimal_byte_offsetL2 = bitset<numeric_limits<unsigned long>::digits>(sBits_byte_offset).to_ulong();
		
			cout << "Decimal Tag L1: " << decimal_tagL2 << endl;
			cout << "Decimal Index L1: " << decimal_indexL2 << endl;
			cout << "Decimal Offset L1: " << decimal_byte_offsetL2 << endl;

			//Define si se hace una escritura o lectura comparando esa bandera
			for(int i=0;i<LongCad-1;i++){			
				if(LecturaEscritura[i]=='S')
						LE=1;				
			}
		}

//CPU0
//Devuelve si hay un hit/miss en la cache L11
RedWriteCache(CacheL11, Fila, LE, decimal_tagL1, decimal_indexL1, decimal_byte_offsetL1);

	if(LE != 0){
//Escritura de la cache

		if(misses == 1){
//Miss en la cache L11, revisa la cache L12
			RedWriteCache(CacheL12, Fila, LE, decimal_tagL1, decimal_indexL1, decimal_byte_offsetL1);
			if(misses == 1){
				//Miss en la cache L12, revisa la cache L2
				RedWriteCache(CacheL2, Fila, LE, decimal_tagL2, decimal_indexL2, decimal_byte_offsetL2);
				if(misses == 1)
					//Miss en la cache L2, Invalida el bus
					CacheL11[decimal_indexL1][1] = I;
				else 
					CacheL11[decimal_indexL1][1] = M;
			}
	    else
				CacheL11[decimal_indexL1][1] = E;
		}	
	
		else{
//Hit en la cache L11, revisa la cache L12
			RedWriteCache(CacheL12, Fila, LE, decimal_tagL1, decimal_indexL1, decimal_byte_offsetL1);

			if(misses == 1){
				//Miss en la cache L12, revisa la cache L2
				RedWriteCache(CacheL2, Fila, LE, decimal_tagL2, decimal_indexL2, decimal_byte_offsetL2);
				if(misses == 1)
					//Miss en la cache L2, dato exclusivo cache L1
					CacheL11[decimal_indexL1][1] = E;
				else 
					//Dato compartido por ambas caches
					CacheL11[decimal_indexL1][1] = M;
			}
	    else
				//Hit en la cache L12, dato compartido por ambas caches
				CacheL11[decimal_indexL1][1] = S; 
		}	

//Lectura de la cache
	if(misses == 1){
//Miss en la cache L11
		RedWriteCache(CacheL12, Fila, LE, decimal_tagL1, decimal_indexL1, decimal_byte_offsetL1);
		if(misses == 1){
			//Miss en L12
			RedWriteCache(CacheL2, Fila, LE, decimal_tagL2, decimal_indexL2, decimal_byte_offsetL2);
			if(misses == 1)
				//Miss en L2
				CacheL11[decimal_indexL1][1] = I;
			else
				CacheL11[decimal_indexL1][1] = M;
		}
		else
		 	CacheL11[decimal_indexL1][1] = S;
	}
	else	
	//Hit en la cache
	CacheL11[decimal_indexL1][1] = E;		
}



//CPU2
RedWriteCache(CacheL12, Fila, LE, decimal_tagL1, decimal_indexL1, decimal_byte_offsetL1);

//Devuelve si hay un hit/miss en la cache L12
RedWriteCache(CacheL12, Fila, LE, decimal_tagL1, decimal_indexL1, decimal_byte_offsetL1);

	if(LE != 0){
//Escritura de la cache

		if(misses == 1){
//Miss en la cache L12, revisa la cache L11
			RedWriteCache(CacheL11, Fila, LE, decimal_tagL1, decimal_indexL1, decimal_byte_offsetL1);
			if(misses == 1){
				//Miss en la cache L12, revisa la cache L2
				RedWriteCache(CacheL2, Fila, LE, decimal_tagL2, decimal_indexL2, decimal_byte_offsetL2);
				if(misses == 1)
					//Miss en la cache L2, Invalida el bus
					CacheL12[decimal_indexL1][1] = I;
				else 
					CacheL12[decimal_indexL1][1] = M;
			}
	    else
				CacheL12[decimal_indexL1][1] = E;
		}	
	
		else{
//Hit en la cache L12, revisa la cache L11
			RedWriteCache(CacheL11, Fila, LE, decimal_tagL1, decimal_indexL1, decimal_byte_offsetL1);

			if(misses == 1){
				//Miss en la cache L12, revisa la cache L2
				RedWriteCache(CacheL2, Fila, LE, decimal_tagL2, decimal_indexL2, decimal_byte_offsetL2);
				if(misses == 1)
					//Miss en la cache L2, dato exclusivo cache L12
					CacheL12[decimal_indexL1][1] = E;
				else 
					//Dato compartido por ambas caches
					CacheL12[decimal_indexL1][1] = M;
			}
	    else
				//Hit en la cache L11, dato compartido por ambas caches
				CacheL12[decimal_indexL1][1] = S; 
		}	

//Lectura de la cache
	if(misses == 1){
//Miss en la cache L12
		RedWriteCache(CacheL11, Fila, LE, decimal_tagL1, decimal_indexL1, decimal_byte_offsetL1);
		if(misses == 1){
			//Miss en L12
			RedWriteCache(CacheL2, Fila, LE, decimal_tagL2, decimal_indexL2, decimal_byte_offsetL2);
			if(misses == 1)
				//Miss en L2
				CacheL12[decimal_indexL1][1] = I;
			else
				CacheL12[decimal_indexL1][1] = M;
		}
		else
		 	CacheL12[decimal_indexL1][1] = S;
	}
	else	
	//Hit en la cache
	CacheL12[decimal_indexL1][1] = E;		
}
			
	}
	//Cierre del archivo

	archivo.close();

  cout << "Cierra archivo y finaliza correctamente" << endl;
  return 0;
}



//Funciones de conversion

//Funcion que convierte un string en hexadecimal a un string en binario 
string HexStringABinString (string dirHex) {
	string sReturn = "";
	for (int i = 0; i < dirHex.length (); ++i){
		switch (dirHex [i]){
			case '0': sReturn.append ("0000"); break;
			case '1': sReturn.append ("0001"); break;
			case '2': sReturn.append ("0010"); break;
			case '3': sReturn.append ("0011"); break;
			case '4': sReturn.append ("0100"); break;
			case '5': sReturn.append ("0101"); break;
			case '6': sReturn.append ("0110"); break;
			case '7': sReturn.append ("0111"); break;
			case '8': sReturn.append ("1000"); break;
			case '9': sReturn.append ("1001"); break;
			case 'a': sReturn.append ("1010"); break;
			case 'b': sReturn.append ("1011"); break;
			case 'c': sReturn.append ("1100"); break;
			case 'd': sReturn.append ("1101"); break;
			case 'e': sReturn.append ("1110"); break;
			case 'f': sReturn.append ("1111"); break;
			}
		}
	return sReturn;
}

// Funcion que  convierte el string de direccion binaria a multiples strings binarios, correspondientes a los strings del index, tag y byte offset.
void BinStringAMultipleString (string sBits_direccion, int tam_index, int tam_tag, int tam_byte_offset) {
	for (int j = 0; j < sBits_direccion.length (); ++j)
		{
		if ( tam_tag != 0 ) {  // Coviert primero el tag.	
			switch (sBits_direccion [j]){
				case '0': sBits_tag.append ("0"); break;
				case '1': sBits_tag.append ("1"); break;
			}
			tam_tag--;
		}
		else if ( tam_index != 0 ){ // Luego del tag, convierte el index.
			switch (sBits_direccion [j]){
				case '0': sBits_index.append ("0"); break;
				case '1': sBits_index.append ("1"); break;
			}
				tam_index--;
		}
			else if ( tam_byte_offset != 0){ // Termina de convertir con el byte offset.
				switch (sBits_direccion [j]){
					case '0': sBits_byte_offset.append ("0"); break;
					case '1': sBits_byte_offset.append ("1"); break;
				}
			tam_byte_offset--;
		}
	}
}


// Función para leer y escribir las caches
void RedWriteCache(unsigned long Cache[][Columnas], int Fila, int LE, unsigned long decimal_tag, unsigned long decimal_index, unsigned long decimal_byte_offset){

	misses = 0;
  hits = 0;

	if(LE != 0){
//Escritura de la cache
		cout << "'Escritura a la cache'\n"<< endl; 
    if(Cache[decimal_index][0] == (E | M)){
			if(Cache[decimal_index][0] == 0){
				//cout << "No esta en la cache" << endl;
				Cache[decimal_index][2] = decimal_tag;
				Cache[decimal_index][0] = 1;	
				misses++;						
			}
			else{
				//cout << "esta en la cache" << endl;
				if(Cache[decimal_index][1] == decimal_tag)
					hits++;
				else{
					//cout << "el bloque esta ocupado y lo debe cambiar" << endl;
	    		Cache[decimal_index][2] = decimal_tag;
					Cache[decimal_index][0] = 1;
					misses++;
				}
			}
		}
	}

//Lectura de La cache
	else{
		cout << "'Lectura a la cache'\n"<< endl;
		//Si el dato es inválido no lo puede leer 
		if(Cache[decimal_index][1] != 0){
			//Si el bit de válido esta en 1, el dato en cache
			if(Cache[decimal_index][0] == 0)
				misses++;
			else if(Cache[decimal_index][2] == decimal_tag)
						hits++;
						else
							misses++;	
		}
	}
}

void ClearCache(unsigned long Cache[][Columnas], int Fila){
	for(int i=0;i<Fila;i++){
 		for(int j=0;j<Columnas;j++)
			Cache[i][j] = 0;
		}
}

