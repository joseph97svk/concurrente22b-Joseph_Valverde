

La solución a Ejemplo 7 utiliza semáforos no nombrados o también llamados semáforos de memoria. Son regiones de la memoria alojadas con sem_init() y liberadas con sem_destroy(). En macOS, estos semáforos están desaprobados (deprecated).

Modifique su solución para usar semáforos nombrados que tienen la ventaja de ser más universales entre sistemas operativos. Son archivos, abiertos con sem_open(), cerrados con sem_close() y eliminados del sistema de archivos con sem_unlink(). Si no se eliminan, quedan en el sistema de archivos y próximas ejecuciones del programa podrían continuar usándolos o dependiendo del modo de apertura, fallarán.

¿Puede su programa con semáforos nombrados crear más hilos que con semáforos de memoria?
