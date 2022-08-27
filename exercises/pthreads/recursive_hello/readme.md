Copie su ejemplos/pthreads/hello a ejercicios/pthreads/recursive_hello. Puede renombrar ejercicios/pthreads/recursive_hello/src/hello.c a recursive_hello.c o si lo prefiere, main.c.

Modifique a greet() para recibir un número de tipo size_t. Si este número es 0, greet() imprime una despedida y el número recibido. Luego termina su ejecución.

Si el número que greet() recibe es mayor que cero, imprime un saludo y el número recibido. Luego crea un hilo para que invoque a greet() pero con un número menor que el recibido por parámetro.

Modifique el procedimiento main() para crear una variable local de tipo size_t inicializada en 2. Envíe este número como parámetro de la subrutina greet() que correrá en el hilo secundario.

Recuerde copiar este enunciado en una sección de su documeneto de análisis ejercicios/pthreads/recursive_hello/readme.md y darle el formato apropiado. En otra sección haga una predicción de la salida de su programa antes de corerlo. Compílelo y córralo con el Makefile. Copie la salida de su programa y péguela en un bloque de código debajo de su predicción. ¿Acertó la predicción?



Dibuje un rastreo de memoria del programa que relizó en el Ejercicio 4 [recursive_hello]. Puede hacerlo en su cuaderno o en un programa de ilustración. Use direcciones ficticias de memoria.

En caso de dibujar en el cuaderno, tome una fotografía. Edítela para que ocupe menos de 100kB. En caso de dibujar en un programa de ilustración, exporte la imagen a formato vectorial (SVG).

Agregue la imagen a la carpeta recursive_hello/trace/ e incrústela dentro de su documento recursive_hello/readme.md.

![Memory Tracing](../recursive_hello/trace/recursive_hello_mem_trace.png)