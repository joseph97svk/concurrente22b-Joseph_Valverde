Modifique el programa de Ejemplo 35 para repartir iteraciones entre hilos usando los tipos de mapeo (scheduling) provistos por OpenMP:

static
static,N
dynamic
dynamic,N
guided
guided,N
Recuerde que el programa debe permitir al usuario indicar la cantidad de hilos y de iteraciones como argumentos opcionales en la línea de comandos. Agregue un tercer parámetro que equivale al número N de la lista de arriba, conocido como el "tamaño de bloque" (chunk en terminología de OpenMP). Si se omite, use el tipo de mapeo sin este parámetro.

Cree un arreglo dinámico de enteros, uno para cada iteración. Haga que los hilos marquen en el arreglo las iteraciones que les fueron asignadas, con su número identificador. Por ejemplo si se reparten 7 iteraciones entre 3 hilos, y si al hilo 2 le correspondieron las iteraciones 5 y 6 establecería

arr[5] := 2
arr[6] := 2
Finalmente el programa debe imprimir en la salida estándar una tabla, con el tipo de mapeo en las filas, las iteraciones en las columnas, y los threads en las celdas. Por ejemplo, para repartir 10 iteraciones entre 3 threads, una salida sin N podría ser:

$ ./schedule 3 10
          0 1 2 3 4 5 6 7 8 9
static    0 0 0 0 1 1 1 2 2 2
dynamic   0 1 0 2 1 0 2 1 0 2
guided    0 0 0 0 1 1 2 2 0 1
La siguiente es una salida hipotética con N=2. Haga más corridas y trate de inferir el algoritmo de mapeo de cada planificación provista por OpenMP.

$ ./schedule 3 10 2
          0 1 2 3 4 5 6 7 8 9
static,2  0 0 1 1 2 2 0 0 1 1
dynamic,2 0 0 1 1 0 0 2 2 1 1
guided,2  0 0 0 0 1 1 2 2 0 0