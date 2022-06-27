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

////////////////////////////////////////////////DEFINICIÓN DE CONSTANTES//////////////////////////////////////////////////////

#define LIMITE_CADENA 100
#define POSICION_U 0
#define POSICION_V 1
#define VALOR_REAL 2
#define VALOR_IM 3
#define RUIDO 4

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





////////////////////////////////DEFINICIÓN DE VARIABLES GLOBALES//////////////////////////////////////////////////////////////
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

pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex3 = PTHREAD_MUTEX_INITIALIZER;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




///////////////////DECLARACIÓN DE FUNCIONES////////////////////////
int verificarFormato(char* argv[],int argc,char** nombreArchivoEntrada,char** nombreArchivoSalida,int* cantidadDiscos,int* cantidadHebras, int* chunk,int* anchoDisco, int* flagB);
disco* crearArregloDiscos(int cantidadDiscos);
void liberarArregloDiscos(int cantidadDiscos, disco* arregloDiscos);
float calcularDistancia(char cadena[]);
int asignarDisco(int anchoDisco, int cantidadDiscos ,float distancia);
float calcularPotencia(float* valorReal,float* valorImaginario, int cantidadElementos);
float sumatoria(float* arrayElementos, int cantidadElementos);
float calcularPromedio(float* arrayElementos, int cantidadElementos);
void cadenaAFlotantes(char cadena[], float* posU, float* posV, float* valorR, float* valorIm, float* Ruido);
void* hebra(void* arg);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////