Consideraciones del trabajo realizado:

1.- No se pudo realizar el makefile, por lo que para compilar los archivos y ejecutar la hebra principal se escribe lo siguiente.


COMPILACION
gcc lab2.c -lm -o lab2


EJECUCION(ejemplo)
./lab2 -i test10000.csv -o salida1.txt -d 100 -n 4 -h 3 -c 2 -b

./lab2 -i test100.csv -o salida1.txt -d 200 -n 2 -h 3 -c 2 -b



Es importante que la compilacion del archivo .c lleve el "-lm" ya que en el programa se utilizan funciones de la librería math.h y el compilador solo la reconoce colocando ese "-lm" a la hora de compilar.
