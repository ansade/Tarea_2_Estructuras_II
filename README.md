# Tarea_2_Estructuras_II
Cache protocolo MESI y cálculo de numeros primos usando  OpenMPI


Para correr los archivos de cálculo de numeros primos:

CASO1: Sin paralelizar ejecutar :

./Sin_paralelo/mpicc sin_primos.c  -o out
./out

CASO2: Con paralelización ejecutar :

./Con_paralelo/mpicc con_primos.c  -o out
./mpirun -np 25 out

