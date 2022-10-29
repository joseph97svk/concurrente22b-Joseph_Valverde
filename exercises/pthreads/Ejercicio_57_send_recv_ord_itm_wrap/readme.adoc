Ejercicio 57 [send_recv_ord_itm_wrap]
Las funciones de paso de mensajes de MPI son orientadas a arreglos. Todas ellas reciben la dirección de memoria (puntero) donde inicia un arreglo en la memoria. Si la operación es un send, se envía la cantidad de elementos a partir de esa dirección al proceso destino. Si la operación es un receive, se escribe a partir de esta dirección los elementos que se recibieron por la red hasta un máximo indicado por una cantidad de elementos. Dado que MPI está diseñado pensando en arreglos, para enviar o recibir un único elemento hay que enviar su dirección de memoria e indicar que sólo 1 elemento conforma el arreglo.

Hasta el momento su implementación de la clase wrapper de MPI sólo permite enviar valores escalares. En este ejercicio, modifique la clase Mpi en la carpeta mpi_wrapper/ para proveer variaciones de los métodos genéricos send() y receive() que puedan enviar y recibir arreglos de valores. Estas variaciones están indicadas en el siguiente pseudocódigo, donde Type es un tipo de datos genérico.

/// Send a scalar value to another process
send(const Type& value, toProcess, tag = 0)
/// Send an array of count values to another process
send(const Type* values, count, toProcess, tag = 0)
/// Send an array of values to another process
send(const std::vector<Type>& values, toProcess, tag = 0)
/// Send a text to another process
send(const std::string& text, toProcess, tag = 0)

/// Wait until it receives a scalar value from other process
receive(Type& value, fromProcess = any_process, tag = any_tag)
/// Wait until it receives at most capacity values from another process
receive(Type* values, capacity, fromProcess = any_process, tag = any_tag)
/// Wait until it receives at most capacity values from another process
receive(std::vector<Type>& values, capacity, fromProcess = any_process, tag = any_tag)
/// Wait until it receives a text of at most length chars from another process
receive(std::string& text, capacity, fromProcess = any_process, tag = any_tag)
En la carpeta send_recv_ord_itm_wrap/ provea un archivo main.cpp que use los métodos send() y receive() de textos para lograr la funcionalidad del ejemplo de imprimir en orden a través de un intermediario.