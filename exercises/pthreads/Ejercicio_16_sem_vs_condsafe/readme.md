Ejercicio 16 [sem_vs_condsafe]
Ejecute al menos tres veces los códigos de Ejemplo 7 (imprimir en orden con semáforos) y Ejemplo 8 (imprimir en orden con seguridad condicional) con tantos hilos como su sistema operativo permite. Reporte en su readme.md las tres duraciones de cada solución. Tome la menor duración de las tres corridas de cada versión. ¿Cuál solución realiza su trabajo en menor tiempo? Discuta en máximo un párrafo cuál podría ser la causa de este comportamiento.


Ejemplo 7:
1. 0.567565483s
2. 0.636905649s
3. 0.607331275s

Min: 0.567565483s

Ejemplo 8:
1. 0.665203832s
2. 0.608685435s
3. 0.656799167s

Min: 0.608685435s


Resultado:
Ejemplo 7: 0.567565483s
Ejemplo 8: 0.608685435s


El ejercicio 7, cuya solucion es depende en el uso semaforos, resultó ser de las dos, el más rápido. En si la diferencia en los tiempos no deberia estar asociada a los diferentes tipos de concurrencia, sino en las implementaciones de las soluciones. Para los semaforos, se crean semaforos por cada hilo para manejar la concurrencia y posteriormente cada hilo imprime su mensaje. Para la solucion condicionalmente segura se busca memoria para la misma cantidad de espacio en un arreglo, pero para strings en lugar de semaforos, despues por cada uno se busca memoria para cada string, despues se hace una copia de texto hacia estos strings y finalmente estos son leidos del arreglo para su lectura. Es esta cantidad extra de operaciones que implica mayor tiempo de ejecucion, consistemente, de la solucion condicionalmente segura sobre la solucion con semaforos. 