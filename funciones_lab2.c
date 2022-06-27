#include <stdio.h> //Funciones de entrada y salida como printf
#include <stdlib.h> //Asignación de memoria, atoi, etc.
#include <string.h>
#include <unistd.h> //para getopt 
#include <math.h> //para funciones como sqrt()
#include <pthread.h> //para hebras
#include "cabeceras.h" 







//////////////////////////////////////////DEFINICIÓN DE FUNCIONES///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




//Funcion perteneciente a la hebras hijas que se van a ejecutar en el programa
void* hebra(void* arg){

    //Variables donde se guardarán los valores de las visibilidades evaluadas
    int whileEjecutando = 1;
    float posU;
    float posV;
    float valorR;
    float valorIm;
    float Ruido;

    int i; //iterador

    //arreglo estatico donde se guardará momentaneamente la linea leída del archivo
    char cadenaLeida[LIMITE_CADENA]; 
    char copiaCadenaLeida[LIMITE_CADENA];

    //entero que indicará el disco al que pertenece la visibilidad evaluada
    int discoElegido;

    //Arreglo de flotantes donde irán los datos de la visibilidad evaluada
    float* arregloFlotantes;

    //Variable para guardar la distancia de la visibilidad al origen
    float distancia;

    //Se crea una matriz de strings para guardar las lineas leídas del archivo de entrada
    char matrizStrings[chunk][LIMITE_CADENA];


    //Se proceden a leer las "chunk" lineas del archivo de entrada

    //Mientras no se llegue al final del archivo
    while(whileEjecutando){

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
            if(feof(archivoEntrada) != 0){

                whileEjecutando = 0;
                break;
            }


            //Se lee la cadena y se guarda en la matriz de strings
            fgets(cadenaLeida,LIMITE_CADENA,archivoEntrada);
            strcpy(matrizStrings[i], cadenaLeida);

        }//fin for chunk

        //Se libera el mutex
        pthread_mutex_unlock(&mutex);

        ////////////////////////////////EXIT SECCION CRITICA////////////////////////////////////////////////////////////////////////



        //Para cada linea de string leída
        for(i=0;i<chunk;i++){
            //Si lo que hay en el arreglo no es un caracter vacío
            if(strcmp(matrizStrings[i],"") != 0){

            
                //Para manipular la cadena leída, se pretende realizar split con la funcion strtok
                //pero esta funcion modifica el arreglo, por lo que para evitar problemas se hará una copia del arreglo
                strcpy(copiaCadenaLeida,matrizStrings[i]);

                /////////////////////////////////////////Enter mini sección critica//////////////////////////////////////////////////////////////

                //Por razones desconocidas, el programa en algunas ocasiones no se termina debido a que provoca violación de segmento.
                //Luego de intentar indagar el problema con diferentes estrategias, no se pudo encontrar su origen,
                //Sin embargo, se sabe que este problema (de concurrencia lo mas probable) se genera en alguna de estas 3 funciones
                //o mientras las hebras ejecutan estas funciones. Por lo que para evitar que esto suceda se decidió asignar una pequeña sección critica
                //que involucra el uso de estas 3 funciones, y así evitar el segmentation fault y que solo una hebra a la vez haga uso de estas funciones.
                //Para esto, se asigna un mutex que en este caso es llamado mutex3.
                pthread_mutex_lock(&mutex3);

                //Se calcula la distancia de la visibilidad al origen
                distancia = calcularDistancia(copiaCadenaLeida);

                //Se obtiene el disco al que pertenece la visibilidad de acuerdo a su distancia
                discoElegido = asignarDisco(anchoDisco,cantidadDiscos,distancia);

                
                //Se fragmenta la cadena con la visibilidad leída y se guardan sus datos en las variables
                cadenaAFlotantes(matrizStrings[i],&posU,&posV,&valorR,&valorIm,&Ruido);

                //una vez usada las funciones, se libera el mutex.
                pthread_mutex_unlock(&mutex3);
                ///////////////////////////////////////Exit mini sección critica/////////////////////////////////////////////////////////////////////





                /////////////////////////////////// ENTER SECCIÓN CRÍTICA ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                //Se toma el mutex ya que aqui se escribirá y modificará el arreglo de estructuras disco, compartido por todas las hebras
                pthread_mutex_lock(&mutex2);

                //Se proceden a escribir los valores de las variables en el disco al cual pertenecen

                //Primero, se aumenta el largo de los arreglos con la ayuda de la funcion realloc
                arregloDiscos[discoElegido].ejeU = (float*)realloc(arregloDiscos[discoElegido].ejeU, sizeof(float)*((arregloDiscos[discoElegido].cantidadVisibilidades)+1));
                arregloDiscos[discoElegido].ejeV = (float*)realloc(arregloDiscos[discoElegido].ejeV, sizeof(float)*((arregloDiscos[discoElegido].cantidadVisibilidades)+1));
                arregloDiscos[discoElegido].valorReal = (float*)realloc(arregloDiscos[discoElegido].valorReal, sizeof(float)*((arregloDiscos[discoElegido].cantidadVisibilidades)+1));
                arregloDiscos[discoElegido].valorImaginario = (float*)realloc(arregloDiscos[discoElegido].valorImaginario, sizeof(float)*((arregloDiscos[discoElegido].cantidadVisibilidades)+1));
                arregloDiscos[discoElegido].ruido = (float*)realloc(arregloDiscos[discoElegido].ruido, sizeof(float)*((arregloDiscos[discoElegido].cantidadVisibilidades)+1));

                int visibilidadesActuales = arregloDiscos[discoElegido].cantidadVisibilidades;

                arregloDiscos[discoElegido].ejeU[visibilidadesActuales] = posU;
                arregloDiscos[discoElegido].ejeV[visibilidadesActuales] = posV;
                arregloDiscos[discoElegido].valorReal[visibilidadesActuales] = valorR;
                arregloDiscos[discoElegido].valorImaginario[visibilidadesActuales] = valorIm;
                arregloDiscos[discoElegido].ruido[visibilidadesActuales] = Ruido;
                arregloDiscos[discoElegido].cantidadVisibilidades = arregloDiscos[discoElegido].cantidadVisibilidades + 1;

                //Se libera el mutex
                pthread_mutex_unlock(&mutex2);
                ////////////////////////////////// EXIT SECCIÓN CRÍTICA ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            }//fin if matrizStrings[i] != ""


        }//fin for cada lidea de string leída

        
    }//fin while feof

}//fin hebra































































































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

        //se inicializan los valores y se pide el espacio en memoria para los arreglos
        arregloDiscos[i].cantidadVisibilidades = 0;
        arregloDiscos[i].id_disco = i;
        arregloDiscos[i].ejeU = (float*)malloc(sizeof(float));
        arregloDiscos[i].ejeV = (float*)malloc(sizeof(float));
        arregloDiscos[i].valorReal = (float*)malloc(sizeof(float));
        arregloDiscos[i].valorImaginario = (float*)malloc(sizeof(float));
        arregloDiscos[i].ruido = (float*)malloc(sizeof(float));

    }//fin for

    //se retorna el arreglo
    return(arregloDiscos);
}//fin crearArregloDiscos


























//Funcion que libera la memoria utilizada por el arreglo de discos
//Entrada: cantidad de discos del arreglo
void liberarArregloDiscos(int cantidadDiscos, disco* arregloDiscos){

    int i;
    for(i=0;i<cantidadDiscos;i++){

        //Se libera la memoria utilizada por los arreglos que contiene la estructura
        free(arregloDiscos[i].ejeU);
        free(arregloDiscos[i].ejeV);
        free(arregloDiscos[i].valorReal);
        free(arregloDiscos[i].valorImaginario);
        free(arregloDiscos[i].ruido);


    }

    //finalmente, se libera el arreglo
    free(arregloDiscos);



}//fin liberarArregloDiscos





























//Función que recibe la cadena de caracteres perteneciente a una linea del archivo de entrad
//Y a traves de esta cadena, obtiene la distancia de la visibilidad (coordenadas (u,v)) al origen
//Entrada: Cadena de caracteres
//Salida: Distancia de la coordenada u,v al origen
float calcularDistancia(char cadena[]){

    //variables para guardar los valores de las coordenadas
    float coordenadaU;
    float coordenadaV;

    //se realiza un primer split para obtener la coordenada U en forma de char*
    char* coordenadaUChar = strtok(cadena, ",");

    //se obtiene ahora la coordenada V en forma de char*
    char* cadenaRestante = strtok(NULL, " ");
    char* coordenadaVChar = strtok(cadenaRestante, ",");

    //se obtiene las coordenas u y v como flotantes con la funcion strtok
    coordenadaU = strtof(coordenadaUChar,NULL);
    coordenadaV = strtof(coordenadaVChar,NULL);

    //se procede a calcular la distancia
    float argumentoRaiz = ((coordenadaU * coordenadaU) + (coordenadaV * coordenadaV));

    //Se calcula la raiz cuadrada
    float resultado = sqrtf(argumentoRaiz);

    //Finalmente, se retornan el resultado obtenido
    return(resultado);


}//fin calcularDistancia



























//Funcion que toma la distancia de la visibilidad al origen y evalúa a que disco pertenece la visibilidad de acuerdo al ancho del mismo
//Entrada: ancho de disco, cantidad de discos que tendrá el programa, distancia de la visibilidad al origen
//Salida: entero que indica a que posicion del arreglo de la estructura disco pertenece la visibilidad
int asignarDisco(int anchoDisco, int cantidadDiscos ,float distancia){

    //Variable para ir comparando con la distancia
    int rangoDisco = anchoDisco;

    //Disco al que pertenece la visibilidad
    int discoElegido = 0;

    //mientras se evalua la visibilidad
    while(discoElegido<cantidadDiscos){

        //Si la distancia es menor que la cota superior del disco, entonces pertenece a ese disco, y se retorna
        if(distancia < rangoDisco){

            return(discoElegido);
        }

        else{

            //sino se sigue evaluando para el siguiente disco
            discoElegido = discoElegido + 1;
            rangoDisco = rangoDisco + anchoDisco;
        }//fin else

    }//fin while

    //Se retorna el resultado
    return((discoElegido-1));


}//






























//Funcion que calcula la potencia de un array de flotantes.
//Esta función será utilizada para calcular la potencia de las visibilidades obtenidas por el proceso hijo

//Entrada: Array de numeros flotantes
//Salida: potencia del array de elementos
float calcularPotencia(float* valorReal,float* valorImaginario, int cantidadElementos){

    //iterador
    int i = 0;

    //variable donde se guardará el resultado final
    float resultadoFinal = 0.0;

    float resultadoParcial;

    //mientras queden valores por sumar
    while(i<cantidadElementos){

        //Se calcula el termino i de la sumatoria
        resultadoParcial = sqrtf((valorReal[i] * valorReal[i]) + (valorImaginario[i] * valorImaginario[i]));

        //se suma el valor al resultado
        resultadoFinal = resultadoFinal + resultadoParcial;
        i = i+1;

    }//fin while

    //se retorna el resultado
    return(resultadoFinal);

}//Fin funcion calcularPotencia



































//Funcion que calcula la sumatoria de los elementos de un array de flotantes.
//Esta función será utilizada para calcular el ruido total de las visibilidades obtenidas por el proceso hijo

//Entrada: Array de numeros flotantes
//Salida: sumatoria del total de numeros que componen el array
float sumatoria(float* arrayElementos, int cantidadElementos){

    //iterador
    int i = 0;

    //variable donde se guardará el resultado final
    float resultado = 0.0;

    //mientras queden valores por sumar
    while(i<cantidadElementos){


        //se suma el valor al resultado
        resultado = resultado + arrayElementos[i];
        i = i+1;

    }//fin while


    //se retorna el resultado
    return(resultado);

}//Fin funcion sumatoria
































//Funcion que calcula el promedio simple de los elementos de un array de flotantes.
//Esta función será utilizada para calcular la media imaginaria y real de las visibilidades obtenidas por el proceso hijo.

//Entrada: Array de numeros flotantes
//Salida: promedio simple del total de numeros que componen el array
float calcularPromedio(float* arrayElementos, int cantidadElementos){

    //iterador
    int i = 0;

    //variable donde se guardará el resultado final
    float resultado = 0.0;

    //mientras queden valores por sumar
    while(i<cantidadElementos){

        //se suma el valor al resultado
        resultado = resultado + arrayElementos[i];
        i = i+1;

    }//fin while

    //Se calcula el promedio simple
    resultado = (resultado/(cantidadElementos+0.0));

    //se retorna el resultado
    return(resultado);

}//Fin funcion calcular promedio




































//Funcion que toma una cadena que indica los valores de la visibilidad y los fragmenta para obtener los valores en flotante de
//la posicion u, posicion v, valor real, valor imaginario y el ruido. 
//Estos valores serán guardado por medio de paso por referencia con las direcciones de memoria de los valores que se reciben como entrada.


//Entrada: Cadena de caracteres la cual será evaluada, punteros a las variables donde guardar los resultados
void cadenaAFlotantes(char cadena[], float* posU, float* posV, float* valorR, float* valorIm, float* Ruido){

    //printf("Soy la hebra %d y antes de empezar con cadenaAFlotantes2 la cadena recibida es %s\n",gettid(),cadena);

    //se guarda la cantidad de parametros que tiene la visibilidad, para este caso son 5
    int cantidadParametros = 5;

    //valores para guardar los valores flotantes

    float posicionU;
    float posicionV;
    float valorReal;
    float valorImaginario;
    float ruido;

    //Variable para guardar el valor flotante obtenido de la visibilidad pero en version char
    char* charLeido;

    //Variable para guardar el valor flotante obtenido de la visibilidad 
    char flotanteLeido;

    //Variable para guardar lo que queda de cadena de caracteres luego de aplicar la funcion strtok
    char* cadenaRestante;

    //Este proceso se realizará 5 veces con su respectiva variable
  

    //Se realiza split de la cadena para obtener un valor, para esto se utiliza la funcion strtok
    charLeido = strtok(cadena, ",");
    //Se guarda lo que queda de cadena
    cadenaRestante = strtok(NULL," ");
    //Se transforma el valor a flotante y se guarda en su variable respectiva
    *posU = strtof(charLeido,NULL);

    //Se realiza split de la cadena para obtener un valor, para esto se utiliza la funcion strtok
    charLeido = strtok(cadenaRestante, ",");
    //Se guarda lo que queda de cadena
    cadenaRestante = strtok(NULL," ");
    //Se transforma el valor a flotante y se guarda en su variable respectiva
    *posV = strtof(charLeido,NULL);


    //Se realiza split de la cadena para obtener un valor, para esto se utiliza la funcion strtok
    charLeido = strtok(cadenaRestante, ",");
    //Se guarda lo que queda de cadena
    cadenaRestante = strtok(NULL," ");
    //Se transforma el valor a flotante y se guarda en su variable respectiva
    *valorR = strtof(charLeido,NULL);


    //Se realiza split de la cadena para obtener un valor, para esto se utiliza la funcion strtok
    charLeido = strtok(cadenaRestante, ",");
    //Se guarda lo que queda de cadena
    cadenaRestante = strtok(NULL," ");
    //Se transforma el valor a flotante y se guarda en su variable respectiva
    *valorIm = strtof(charLeido,NULL);


    //Se realiza split de la cadena para obtener un valor, para esto se utiliza la funcion strtok
    charLeido = strtok(cadenaRestante, ",");
    //Se guarda lo que queda de cadena
    cadenaRestante = strtok(NULL," ");
    //Se transforma el valor a flotante y se guarda en su variable respectiva
    *Ruido = strtof(charLeido,NULL);
    
}//Fin cadenaAFlotantes



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////











































































































































