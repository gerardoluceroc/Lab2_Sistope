#include <stdio.h> //Funciones de entrada y salida como printf
#include <stdlib.h> //Asignación de memoria, atoi, etc.
#include <string.h>
#include <unistd.h> //para getopt 
#include <pthread.h> //para hebras
#include <string.h> 
#include <sys/types.h>
#include "funciones_lab2.c"


/* Laboratorio 2 Sistemas Operativos

Nombre: Gerardo Lucero Córdova
Rut: 19.919.462-3
Sección: B2

*/





int main(int argc, char* argv[]){

	//iterador
	int i;

	//Variables para almacenar lo recibido por teclado (junto con las variables globales)
	char* nombreArchivoSalida;
	int flagB = 0; //numero que indica si la bandera -b fue levantada

	//Se llama a la funcion que verifica si la entrada por teclado es correcta
	int entradaCorrecta = verificarFormato(argv,argc,&nombreArchivoEntrada,&nombreArchivoSalida,&cantidadDiscos, &cantidadHebras, &chunk, &anchoDisco, &flagB);

	//Si la entrada no es correcta
	if(!entradaCorrecta){
		//se sale del programa
		return(1);
	}

	//Se crea el arreglo con los discos correspondientes y su informacion
	arregloDiscos = crearArregloDiscos(cantidadDiscos);

	//Se abre el archivo de entrada
	archivoEntrada = fopen(nombreArchivoEntrada, "r");

	//se crea un arreglo con los id de las hebras a crear
	pthread_t arregloHebras[cantidadHebras];


	//Se crean las hebras
	for(i = 0;i<cantidadHebras;i++){

		pthread_create(&arregloHebras[i], NULL, hebra,NULL);

	}//fin for create hebras

	//se esperan a las hebras creadas
	for(i=0;i<cantidadHebras;i++){

		pthread_join(arregloHebras[i],NULL);

	}//fin for join hebras




	//Variables donde se guardarán las propiedades de cada disco para escribirlas en el archivo de salida
	float potencia;
	float ruidoTotal;
	float mediaReal;
	float mediaImeginaria;

	//Se crea o abre el archivo de salida en modo escritura
	FILE* archivoSalida = fopen(nombreArchivoSalida, "w");

	//Se escribe en el archivo de salida
	fprintf(archivoSalida,"%s\n%d discos, ancho %d\n\n",nombreArchivoEntrada,cantidadDiscos,anchoDisco);

	//por cada disco
	for(i=0;i<cantidadDiscos;i++){

		//Se obtienen sus propiedades de acuerdo a las visibilidades que contiene
		mediaReal = calcularPromedio(arregloDiscos[i].valorReal, arregloDiscos[i].cantidadVisibilidades);
		mediaImeginaria = calcularPromedio(arregloDiscos[i].valorImaginario, arregloDiscos[i].cantidadVisibilidades);
		ruidoTotal = sumatoria(arregloDiscos[i].ruido, arregloDiscos[i].cantidadVisibilidades);
		potencia = calcularPotencia(arregloDiscos[i].valorReal, arregloDiscos[i].valorImaginario, arregloDiscos[i].cantidadVisibilidades);

		//Se escriben los resultados en el archivo de salida
		fprintf(archivoSalida,"Disco %d:\nMedia real: %f\nMedia imaginaria: %f\nPotencia: %f\nRuido total: %f\n",arregloDiscos[i].id_disco+1,mediaReal,mediaImeginaria,potencia,ruidoTotal);

	}//fin for disco






	//si la bandera b está levantada
	if(flagB){

		//Se escriben los resultados por pantalla
		printf("%s\n%d discos, ancho %d\n\n",nombreArchivoEntrada,cantidadDiscos,anchoDisco);

		//por cada disco
		for(i=0;i<cantidadDiscos;i++){

			//Se obtienen sus propiedades de acuerdo a las visibilidades que contiene
			mediaReal = calcularPromedio(arregloDiscos[i].valorReal, arregloDiscos[i].cantidadVisibilidades);
			mediaImeginaria = calcularPromedio(arregloDiscos[i].valorImaginario, arregloDiscos[i].cantidadVisibilidades);
			ruidoTotal = sumatoria(arregloDiscos[i].ruido, arregloDiscos[i].cantidadVisibilidades);
			potencia = calcularPotencia(arregloDiscos[i].valorReal, arregloDiscos[i].valorImaginario, arregloDiscos[i].cantidadVisibilidades);

			//Se escriben los resultados por pantalla
			printf("Disco %d:\nMedia real: %f\nMedia imaginaria: %f\nPotencia: %f\nRuido total: %f\n",arregloDiscos[i].id_disco+1,mediaReal,mediaImeginaria,potencia,ruidoTotal);

		}//fin for disco


	}//fin if flagB




 
 	liberarArregloDiscos(cantidadDiscos,arregloDiscos);
 	//free(arregloHebras);//BORRAR ya que ahora uso arreglo estatico

	fclose(archivoEntrada);
	fclose(archivoSalida);
	return(0);


}//Fin main
