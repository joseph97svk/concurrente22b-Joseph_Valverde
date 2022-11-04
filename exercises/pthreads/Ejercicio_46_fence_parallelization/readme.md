# Marcar la cerca

Un constructor de divisiones tiene un problema frecuente. Se quiere delimitar un terrero con la tapia o cerca rectangular que tenga el perímetro más grande posible. Sin embargo, es común que en el terreno hayan árboles, pantanos, estructuras, o cualquier otro obstáculo que impida levantar una división.

Antes de construir se elabora un croquis del terreno, como una cuadrícula donde cada celda representa una unidad de estudio dependiendo de la escala, por ejemplo, un metro cuadrado del terreno. En el croquis se marcan las celdas donde es posible construir con un punto (`.`), y las celdas donde hay un obstáculo con una equis minúscula (`x`).

**Ejemplo de entrada 1**:

	4 4
	....
	..x.
	..x.
	x...

**Ejemplo de salida 1**:

	10 (1,2)-(4,4)

Dado que algunos croquis son muy grandes y complejos, se quiere un programa de computadora que eficientemente encuentre la cerca con el perímetro más grande que se pueda construir. El programa recibe en la entrada estándar un croquis de un terreno antecedido por sus dimensiones en filas y columnas. Finalmente, el programa imprime en la salida estándar el perímetro máximo y las coordenadas de la esquina superior izquierda y de la esquina inferior derecha donde debe ubicarse la cerca.

**Ejemplo de entrada 2**:

	3 4
	.x..
	..x.
	x...

**Ejemplo de salida 2**:

	impossible

Para efectos de este evaluación, asuma que existe una solución única para cada croquis. En caso de que sea imposible construir una cerca, reporte el mensaje `impossible`. Su programa puede utilizar fuerza bruta para encontrar el rectángulo que maximice el perímetro. Dado que la cantidad de pruebas puede ser considerable, distribúyalas entre la cantidad de unidades de procesamiento que dispone el sistema donde corre su programa usando concurrencia compartida. Usted debe distribuir las pruebas lo más equitativamente posible entre los hilos de ejecución.

**Ejemplo de entrada 3**:

	6 10
	........x.
	....x...x.
	x.....x...
	....x.....
	...x..x...
	.x........

**Ejemplo de salida 3**:

	20 (1,3)-(6,8)

Rastree la memoria de su programa completo (dibujo) con el caso de prueba 4 [15%]. Rastree el procesamiento sólo de los hilos secundarios (tabla) en una máquina con dos núcleos [15%]. Agregue números de línea al código que realizó y recuerde usarlos en el rastreo de procesamiento.

**Ejemplo de entrada 4**:

	3 3
	...
	x..
	...

**Ejemplo de salida 4**:

	6 (1,2)-(3,3)

Su programa debe resolver el problema planteado completo [25%]. Su solución debe ser concurrentemente correcta y eficiente [25%]. No debe no debe provocar desbordes de pila, fugas de memoria, accesos inválidos, ni condiciones de carrera [20%]. Recuerde seguir buenas prácticas de programación y modularizar su código en subtareas. La única tecnología de concurrencia permitida para esta evaluación es Pthreads.
