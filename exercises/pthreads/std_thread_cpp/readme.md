C++ también agregó soporte para programación de hilos de ejecución en el 2011 y, a diferencia de C, las funcionalidades concurentes han ido creciendo en versiones recientes. Por ejemplo, el soporte de semáforos y barreras se agregó en el estándar 2020. La funcionalidad concurrente está disponible en encabezado estándar <thread>.

Implemente la misma funcionalidad del ejemplo hello_iw_shr usando hilos estándar de C++, de tal manera que el resultado se convierta en su código base para futuras implementaciones con hilos de ejecución en este lenguaje. Siga estos pasos, y compile y corra su solución tras completar cada uno de ellos:

Su archivo solución debe tener extensión .cpp.

Obtenga la cantidad de CPUs disponibles en el sistema usando funcionalidad de la clase std::thread en lugar de POSIX.

Use un arreglo dinámico para almacenar los objetos que controlan los hilos de ejecución. Dado que estos objetos deben recibir en su constructor la rutina que se va a ejecutar en un hilo aparte, agréguelos al vector con el método emplace_back() en lugar de push_back(). Aunque se puede crear arreglos de punteros a objetos hilo de ejecución en memoria dinámica, mejor evite este diseño.

Mida la duración de ejecución con funcionalidad estándar como std::chrono::high_resolution_clock::now() en lugar de POSIX (clock_gettime).

Use manejo de memoria dinámica de C++ (new) para los datos compartidos de los hilos. Es recomendable usar un puntero inteligente (std::shared_ptr<>) para este fin.

Use un arreglo dinámico para alojar los datos privados de los hilos de ejecución. Aproveche que conoce el tamaño del arreglo de antemano para crearlo de la capacidad justa.

Cambie la entrada y salida de C por la genérica de C/C++. Esto implica reemplazar <stdio.h> por <iostream>, printf() por std::cout <<, stderr por std::cerr, y así por el estilo.

Use std::istringstream en lugar de sscanf().

Cambie el manejo de errores de C (códigos de error) por manejo de excepciones de C++. Lance objetos excepción que puedan transportar mensajes textuales. Para cada error busque la clase que mejor se aproxima en la biblioteca estándar. También puede heredar su propia clase para errores concurrentes, algo como std::concurrency_error si lo necesita (y ésta en su propio archivo .hpp). Atrape y maneje las excepciones en el procedimiento main().

Elimine la inclusión de todos los encabezados dependientes del sistema operativo: <pthread.h>, <unistd.h>. Asegúrese de que las inclusiones queden en orden alfabético.

En su Makefile, elimine el soporte de extensiones del compilador para apegarse sólo a código estándar C++. Asigne la variable XSTD al estándar más reciente que soporta su compilador, por ejemplo: XSTD=-std=c17` o `XSTD=-std=c20. Para esto indague la versión de su compilador (ej.: g++ --version) y aproveche para revisar si soporta semáforos contadores.

Si siguió los pasos anteriores, al final su solución no necesitará usar el operador delete ni delete[] en ninguna parte del código. Recuerde asegurarse de que su solución no realiza mal uso de la memoria, ni la concurrencia, y de apegarse a la convención de estilos.