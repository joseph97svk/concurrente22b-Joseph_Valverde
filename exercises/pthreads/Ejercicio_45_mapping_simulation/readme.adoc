Ejercicio 45 [mapping_simulation]
Escriba un programa que recibe de la entrada estándar un conjunto de duraciones de trabajo, y como argumento de línea de comandos la cantidad de trabajadores. El programa debe crear tantos hilos como trabajadores se indicaron en los argumentos (o la cantidad de CPUs disponibles si se omite), y repartir el trabajo de la entrada estándar siguiendo los tres tipos principales de mapeo vistos en clase: estático por bloque, estático cíclico, y dinámico.

Los hilos de ejecución reciben el trabajo asignado por el mapeo y simulan que lo realizan mediante una espera (sleep). Suponga que las duraciones están dadas en milisegundos. Cada hilo debe llevar la estadística de la cantidad de trabajo asignado.

Tras realizar las tres simulaciones (una por cada tipo de mapeo), el programa reporta estadísticas en la salida estándar que le ayudarían a un desarrollador a escoger el tipo mapeo más adecuado a un conjunto de datos de interés. La salida del programa puede tener formato de tabla como el sugerido a continuación, el cual sigue la estructura del ejemplo de la sección Section 4.3.

$ echo 1 2 1 3 2 1 1 2 1 3 1 2 2 3 2 2 1 1 2 2 2 2 | ./mapping_simulation 4
39         0    1    2    3 Duration Speedup Efficiency
Block     10   10   10    9    10.00    3.90      0.975
Cyclic     9   12    7   11    12.00    3.25      0.813
Dynamic   10   10   10    9    10.00    3.90      0.975
En la línea de encabezado de la salida 39 es la duración serial (suma de todas las duraciones en la entrada estándar), y 0 a 3 son los números de thread. Los números bajo ellos son las duraciones asignadas (trabajo) de cada hilo. La columna duración indica el tiempo tomado por cada tipo de mapeo (tiempo de pared del mapeo y no el mayor de los hilos). Con esta duración de tiempo de pared se calcula el incremento de velocidad y la eficiencia, respecto a la hipotética versión serial.

Puede aprovechar el siguiente código inicial para implementar su simulación. Primero idee una solución que complete el pseudocódigo provisto. Se requiere implementar los tres tipos de mapeos cuyos procedimientos se encuentran vacíos. Puede agregar código en la subrutina main que puede utilizar en la implementación de los mapeos. Si lo desea puede implementar más de una versión del mapeo dinámico, como: repartir con una variable contadora, usar un buffer thread-safe (cola), y un mapeo dinámico aleatorio.