#include <stdio.h> //Funciones de entrada y salida como printf
#include <stdlib.h> //Asignación de memoria, atoi, etc.
#include <string.h>
#include <unistd.h> //para getopt 
#include <pthread.h> //para hebras
#include "funciones_lab2.c"


/* Laboratorio 2 Sistemas Operativos

Nombre: Gerardo Lucero Córdova
Rut: 19.919.462-3
Sección: B2

*/



void* hola(void* arg){

	

	printf("Hola que tal a todos, recibí un 777\n");
	//pthread_exit(NULL);



}


//Se define el archivo de entrada global que será compartido por las hebras
FILE* archivoEntrada;
//arreglo con los discos correspondientes y su informacion
disco* arregloDiscos;


int main(int argc, char* argv[]){

	//iterador
	int i;

	//Variables para almacenar lo recibido por teclado
	char* nombreArchivoEntrada;
	char* nombreArchivoSalida;
	int cantidadDiscos;
	int cantidadHebras;
	int chunk;
	int anchoDisco;
	int flagB = 0; //numero que indica si la bandera -b fue levantada

	//Se llama a la funcion que verifica si la entrada por teclado es correcta
	int entradaCorrecta = verificarFormato(argv,argc,&nombreArchivoEntrada,&nombreArchivoSalida,&cantidadDiscos, &cantidadHebras, &chunk, &anchoDisco, &flagB);

	//Si la entrada no es correcta
	if(!entradaCorrecta){
		//se sale del programa
		return(1);
	}

	//BORRAR
	printf("nombreArchivoEntrada = %s\n",nombreArchivoEntrada);
	printf("nombreArchivoSalida = %s\n",nombreArchivoSalida);
	printf("Valor de cantidadDiscos = %d\n",cantidadDiscos);
	printf("Valor de cantidadHebras = %d\n",cantidadHebras);
	printf("Valor de chunk = %d\n",chunk);
	printf("Valor de anchoDisco = %d\n",anchoDisco);
	printf("Valor de flagB = %d\n",flagB);

	//Se crea el arreglo con los discos correspondientes y su informacion
	arregloDiscos = crearArregloDiscos(cantidadDiscos);

	//BORRAR
	for(int i=0;i<cantidadDiscos;i++){
		printf("soy el disco con id %d\n",arregloDiscos[i].id_disco);
	}


	//Se abre el archivo de entrada
	archivoEntrada = fopen(nombreArchivoEntrada, "r");

	//se crea un arreglo con los id de las hebras a crear
	pthread_t arregloHebras[cantidadHebras];

	int number = 3;//BORRAR

	//Se crean las hebras
	for(i = 0;i<cantidadHebras;i++){

		pthread_create(&arregloHebras[i], NULL, hola,NULL);

	}//fin for create hebras

	//se esperan a las hebras creadas
	for(i=0;i<cantidadHebras;i++){

		pthread_join(arregloHebras[i],NULL);

	}//fin for join hebras



	//pthread_t tid;
	//int number = 3;
	//pthread_create(&tid, NULL, hola,&number);
	//pthread_join(tid,NULL);








 
 	free(arregloDiscos);
 	//free(arregloHebras);//BORRAR ya que ahora uso arreglo estatico

	fclose(archivoEntrada);
	return(0);


}//Fin main
