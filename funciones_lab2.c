#include <stdio.h> //Funciones de entrada y salida como printf
#include <stdlib.h> //Asignación de memoria, atoi, etc.
#include <string.h>
#include <unistd.h> //para getopt 
#include <pthread.h> //para hebras







//Se declara la estructura disco, donde se guardará la informacion de las visibilidades pertenecientes a ese disco
typedef struct disco{

    int cantidadVisibilidades;//cantidad de visibilidades que tiene guardardada la estructura en ese momento
    int id_disco;//id del disco, esto depende de la cantidad de discos que se requieran, valor que es ingresado por teclado
    float* ejeU;//arreglo donde irán los ejes U de las observaciones pertenecientes al disco
    float* ejeV;//arreglo donde irán los ejes V de las observaciones pertenecientes al disco
    float* valorReal;//arreglo donde los valores reales de las observaciones pertenecientes al disco
    float* valorImaginario;//arreglo donde irán los valores imaginarios de las observaciones pertenecientes al disco
    float* ruido;//arreglo donde irán los ruidos de las observaciones pertenecientes al disco

    //cabe mencionar que el dato completo de una observacion específica coincide con:
    //ejeU[i],ejeV[i],valorReal[i],valorImaginario[i],ruido[i]

}disco;






//Funcion que verifica el formato correcto de la entrada ingresada por teclado al ejecutar el programa
//Entrada: Vector de argumentos(argv), cantidad de argumentos de argv, punteros para guardar el string con los parametros de entrada, puntero al entero que indica se activó la bandera b
//Salida: 1 en caso de que el formato sea correcto, 0 si es incorrecto
int verificarFormato(char* argv[],int argc,char** nombreArchivoEntrada,char** nombreArchivoSalida,int* cantidadDiscos,int* cantidadHebras, int* chunk,int* anchoDisco, int* flagB){


	//banderas para verificar el formato de entrada de los parametros
	int flagI = 0;
	int flagO = 0;
	int flagN = 0;
	int flagD = 0;
	int flagH = 0;
	int flagC = 0;

	//Variables para almacenar lo recibido por teclado
	char* numeroDiscos;
	char* numeroHebras;
	char* chunk_char;
	char* ancho;
	int c;

	//ciclo para obtener los parametros ingresados por teclado
	while((c = getopt(argc, argv, "i:o:d:n:h:c:b")) != -1){
		switch(c){
        	case 'i':
                *nombreArchivoEntrada = optarg;
                //se levanta la bandera de i
                flagI = 1;
                break;
            case 'o':
                *nombreArchivoSalida = optarg;
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
                *flagB = 1;
                break;
            default:
                printf("Error en el formato de entrada\nEjemplo:./lab2 -i archivoeEntrada.csv -o ArchivoSalida.txt -d ancho -n cantidadDiscos -h numeroHebras -c chunk -b \n");
                return 0;
        }//fin switch
    }//fin while

    //En caso de que alguna bandera no esté levantada quiere decir que el formato de entrada es incorrecto, por lo que se indica por pantalla y se termina el programa
    if(!(flagD && flagI && flagN && flagO && flagH && flagC)){

    	printf("Error en el formato de entrada\nEjemplo:./lab2 -i archivoeEntrada.csv -o ArchivoSalida.txt -d ancho -n cantidadDiscos -h numeroHebras -c chunk -b \n");
    	return(0);


    }//fin if flag d,i,n,o,h,c

    //se transforman los valos obtenidos del ancho,la cantidad de discos, de hebras y el valor de chunk a entero con la funcion atoi
    *anchoDisco = atoi(ancho);
    *cantidadDiscos = atoi(numeroDiscos);
    *chunk = atoi(chunk_char);
    *cantidadHebras = atoi(numeroHebras);

    //si alguno de estos valores transformados no es positivo
    if((*anchoDisco <= 0) || (*cantidadDiscos <= 0) || (*chunk <= 0) || (*cantidadHebras <= 0)){
    	//se indica por pantalla del error y se retorna la respuesta indicando que hay un error
    	printf("Error al indicar el ancho del disco, la cantidad de discos, el valor de chunk o la cantidad de hebras.\nProcure utilizar valores positivos\n");
    	return(0);
    }//fin if parametros < 0 

    //Si el archivo de entrada no existe, se indica el error por pantalla y se sale de la funcion
    FILE* archivo = fopen(*nombreArchivoEntrada, "r");
    if(archivo == NULL){
        printf("Error: El archivo de entrada no existe\n");
        return(0);
    }

    fclose(archivo);


    //si no hay problemas, se retorna 1
    return(1);

}//fin verificarFormato()







































//Funcion que crea el espacio en memoria para el arreglo de estructuras discos e inicializa el valor de "cantidadVisibilidades" en cero
//Entrada: Cantidad de estructuras a crear
//Salida: puntero al arreglo de discos
disco* crearArregloDiscos(int cantidadDiscos){

    int i;//iterador

    //Se crea el espacio en memoria
    disco* arregloDiscos = (disco*)malloc(sizeof(disco)*cantidadDiscos);

    for(i = 0;i<cantidadDiscos;i++){

        //se inicializan los valores
        arregloDiscos[i].cantidadVisibilidades = 0;
        arregloDiscos[i].id_disco = i;

    }//fin for

    //se retorna el arreglo
    return(arregloDiscos);
}//fin crearArregloDiscos

