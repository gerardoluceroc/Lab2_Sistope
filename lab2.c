#include <stdio.h> //Funciones de entrada y salida como printf
#include <stdlib.h> //Asignación de memoria, atoi, etc.
#include <string.h>
#include <unistd.h> //para getopt 
#include "funciones_lab2.c"


/* Laboratorio 2 Sistemas Operativos

Nombre: Gerardo Lucero Córdova
Rut: 19.919.462-3
Sección: B2

*/


int main(int argc, char* argv[]){

	//Variables para almacenar lo recibido por teclado
	char* nombreArchivoEntrada = (char*)malloc(sizeof(char));
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
	printf("AAfAAAAAAAAAAAAAAAAAAAAAA\n");

	printf("nombreArchivoEntrada = %s\n",nombreArchivoEntrada);
	printf("nombreArchivoSalida = %s\n",nombreArchivoSalida);
	printf("Valor de cantidadDiscos = %d\n",cantidadDiscos);
	printf("Valor de cantidadHebras = %d\n",cantidadHebras);
	printf("Valor de chunk = %d\n",chunk);
	printf("Valor de anchoDisco = %d\n",anchoDisco);
	printf("Valor de flagB = %d\n",flagB);


/*

	//banderas para verificar el formato de entrada de los parametros
	int flagI = 0;
	int flagO = 0;
	int flagN = 0;
	int flagD = 0;
	int flagH = 0;
	int flagC = 0;
    int flagB = 0; //booleano que indica si se activa la bandera -b

//Variables para almacenar lo recibido por teclado
	char* nombreArchivoEntrada;
	char* nombreArchivoSalida;
	char* numeroDiscos;
	char* numeroHebras;
	char* chunk_char;
	char* ancho;
	int c;

	//ciclo para obtener los parametros ingresados por teclado
	while((c = getopt(argc, argv, "i:o:d:n:h:c:b")) != -1){
		switch(c){
        	case 'i':
                nombreArchivoEntrada = optarg;
                //se levanta la bandera de i
                flagI = 1;
                break;
            case 'o':
                nombreArchivoSalida = optarg;
                //se levanta la bandera de o
                flagO = 1;
                break;
            case 'n':
                numeroDiscos = optarg;
                //se levanta la bandera de n
                flagN = 1;
                break;    
            case 'd':
                ancho = optarg;
                //se levanta la bandera de d
                flagD = 1;
                 break;
            case 'h':
                numeroHebras = optarg;
                //se levanta la bandera de d
                flagH = 1;
                 break;
            case 'c':
                chunk_char = optarg;
                //se levanta la bandera de d
                flagC = 1;
                 break;     
            case 'b':
                flagB = 1;
                break;
            default:
                printf("Error en el formato de entrada\nEjemplo:./lab2 -i archivoeEntrada.csv -o ArchivoSalida.txt -d ancho -n cantidadDiscos -h numeroHebras -c chunk -b \n");
                return 1;
        }//fin switch
    }//fin while

    //En caso de que alguna bandera no esté levantada quiere decir que el formato de entrada es incorrecto, por lo que se indica por pantalla y se termina el programa
    if(!(flagD && flagI && flagN && flagO && flagH && flagC)){

    	printf("Error en el formato de entrada\nEjemplo:./lab2 -i archivoeEntrada.csv -o ArchivoSalida.txt -d ancho -n cantidadDiscos -h numeroHebras -c chunk -b \n");
    	return(1);


    }//fin if flag d,i,n,o,h,c


    printf("TUDU BEN\n");

*/











	return(0);


}//Fin main
