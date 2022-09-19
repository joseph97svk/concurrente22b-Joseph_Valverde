Ejercicio 21 [prod_cons_unbound_produce2]
En los diseños de Ejemplo 10 se proveyó una versión produce1() que usa variables contadoras y exclusión mutua para deneter a los productores. No se proveyeron versiones produce2() y produce3().

Cree la versión produce2() que utilice control de concurrencia en lugar de variables contadoras con exclusión mutua para detener a los productores. Use la función try_wait(semaphore) que retorna false si el semáforo está en 0 o negativo en lugar de bloquear al hilo. Si el semáforo está en positivo, funciona igual que wait(semaphore).

Su código puede mantener las variables contadoras para agregar los datos en la cola, pero no para detener los productores. También si lo quiere, puede suponer que existe una función get_count(queue) que retorna la cantidad de elementos almacenada en la cola.

Implemente la variante produce2() en Pthreads junto con consume1(). Compruebe que resuelve el problema correctamente. Recuerde verificar la calidad de código (sanitizing, linting).


Detalles sobre solucion:

1. Solucion de produce2() en prod_cons_unbound2.pseudo.

