Una adorable abuelita juega todos los domingos lotería. Como es peligroso que ella salga de casa, envía a sus dos nietos a comprar un par de pedacitos a vendedores diferentes para incrementar la suerte. Ella siempre juega a "gallo tapado", es decir, sin saber el número que le venden. Sin embargo, cuando sus nietos llegan a la casa, por su cansado estado de la vista ella les tiene que preguntar el número que los vendedores le dieron a cada uno.

Modele la situación anterior, con la abuela como el hilo principal, y los dos nietos como dos hilos secundarios. Los nietos generan un número pseudoaleatorio para representar la compra de la fracción de lotería. La espera de la abuela del regreso a casa de los dos nietos es representado por su join. Los nietos comunican a la abuela los números obtenidos a través del valor de retorno de la rutina secundaria. Indague en la documentación de Pthreads cómo este valor es recuperado por pthread_join(). Implemente dos variantes en que los nietos comunican este valor a la abuela:

1. Retornan la dirección de memoria de la variable que tiene el número comprado.

2. Retornan el número comprado como una falsa dirección de memoria.

Utilice las herramientas de análisis dinámico de código para determinar cuál de las dos variantes anteriores produce el resultado correcto.

Resultados al utilizar opcion 1:

Grandma! I bought 21!
Grandma! I bought 87!
The grandchildren fetched the numbers: 0 and 0 //el puntero lleva a direcciones dealocadas y por ende se pierden los valores
