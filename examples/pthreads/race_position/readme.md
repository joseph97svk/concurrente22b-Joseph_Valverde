Modifique su solución a Ejemplo 5 para implementar una carrera de hilos de ejecución. Cada vez que un hilo llega a la meta, lo avisa en la salida estándar, como ocurre en la siguiente corrida hipotética:

Thread 0/4: I arrived at position 1
Thread 2/4: I arrived at position 2
Thread 1/4: I arrived at position 3
Thread 3/4: I arrived at position 4

Note que las posiciones tienen que aparecer en orden, mientras que es el número de cada hilo es indeterminístico. Utilice algún mecanismo de control de concurrencia para que el reclamo de la posición sea el correcto.