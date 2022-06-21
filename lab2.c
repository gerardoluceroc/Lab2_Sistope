#include <stdio.h> //Funciones de entrada y salida como printf
#include <stdlib.h> //Asignación de memoria, atoi, etc.
#include <string.h>
#include <unistd.h> //para getopt 
#include <pthread.h> //para hebras
#include <sys/types.h>
#include "funciones_lab2.c"


/* Laboratorio 2 Sistemas Operativos

Nombre: Gerardo Lucero Córdova
Rut: 19.919.462-3
Sección: B2

*/

#define LIMITE_CADENA 300



//Variables globales que serán compartidas por las hebras


//Se define el archivo de entrada global que será compartido por las hebras
FILE* archivoEntrada;
//arreglo con los discos correspondientes y su informacion
disco* arregloDiscos;
//string con el nombre del archivo de entrada
char* nombreArchivoEntrada;
//entero con la cantidad de discos 
int cantidadDiscos;
//entero con el valor de chunk
int chunk;
//Entero con la cantidad de hebras
int cantidadHebras;
//Entero con el ancho de los discos
int anchoDisco;

//Mutex para proteger las secciones criticas
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;




























//Funcion perteneciente a la hebras hijas que se van a ejecutar en el programa
void* hebra(void* arg){

	int i; //iterador

	char cadenaLeida[LIMITE_CADENA]; 

	//Se crea una matriz de strings para guardar las lineas leídas del archivo de entrada
	char matrizStrings[chunk][LIMITE_CADENA];


	//Se proceden a leer las "chunk" lineas del archivo de entrada

	//Mientras no se llegue al final del archivo
	while(feof(archivoEntrada) == 0){

		//Se inicializan los valores de la matriz con caracter vacío para identificar aquellas filas que tengan strings y las que no
		for(i=0;i<chunk;i++){
			strcpy(matrizStrings[i], "");
		}

        ////////////////////ENTER SECCION CRITICA///////////////////////////////////////////////////////////////////////////////////////

		//se leen las lineas
		//Se toma el mutex ya que en este ciclo la hebra lee el archivo de entrada
		pthread_mutex_lock(&mutex);
		for(i=0;i<chunk;i++){
			//si se está al final del archivo, se detiene el ciclo
			if(feof(archivoEntrada) != 0){break;}


			//Se lee la cadena y se guarda en la matriz de strings
			fgets(cadenaLeida,LIMITE_CADENA,archivoEntrada);
			strcpy(matrizStrings[i], cadenaLeida);

		}//fin for chunk
		//Se libera el mutex
		pthread_mutex_unlock(&mutex);
		
  		////////////////////////////////EXIT SECCION CRITICA////////////////////////////////////////////////////////////////////////






	}//fin while feof


}//fin hebra






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


	//Se crean las hebras
	for(i = 0;i<cantidadHebras;i++){

		pthread_create(&arregloHebras[i], NULL, hebra,NULL);

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
