Suponga que dos equipos se enfrentan en el deporte de lanzamiento de bala (shot put). Cada equipo tiene la misma cantidad de atletas. Los atletas de cada equipo se ordenan y se enfrentan en parejas. Para simplificar, la notación T.N indica el atleta número N del equipo T. Por ejemplo, el atleta 1 del equipo 1 se enfrenta con el atleta 1 del equipo 2 (1.1 vs 2.1), y el atleta N del equipo 1 se enfrenta con el atleta N del equipo 2 (1.N vs 2.N).

Cada atleta dispone de tres lanzamientos y se toma la distancia más larga que consiguió llegar la bala (shot) en esos tres tiros. Si el atleta N de un equipo consigue llegar la bala más largo que su par N del otro equipo, consigue un punto para su equipo. Gana el encuentro el equipo que consigue más puntos. Los equipos siempre tienen una cantidad impar de atletas.

Simule la situación anterior con programación concurrente. El hilo principal realiza el rol del árbitro. El hilo principal obtiene la cantidad de atletas por argumento de línea de comandos, que siempre debe proveerse y ser impar, de lo contrario, finaliza con un mensaje de error. Seguidamente crea dos equipos de la misma cantidad de atletas (hilos secundarios).

Cada atleta lanza la bala tres veces, lo cual se puede simular generando tres números pseudoaleatorios, tomar el mayor de ellos, imprimirlo en la salida estándar, y reportarlo al árbitro como su valor de retorno. Un lanzamiento se mide como una distancia real entre 0.0 y 25.0 metros. Para comunicar este resultado al árbitro, podría implementar una estrategia como las siguientes.

1. Dado que un flotante de doble precisión tiene el mismo tamaño que un puntero en una arquitectura de 64 bits, puede retornar su valor como una dirección de memoria.

2. Usar memoria dinámica para alojar el resultado del mejor lanzamiento y retornar la dirección al árbitro.

3. El árbitro crea una pizarra donde los atletas anotan su mejor resultado.

El árbitro se encargará de comparar los resultados de cada pareja de atletas, contar los puntos, y reportar en la salida estándar el equipo ganador. La siguiente podría ser una interación con la simulación.

$ bin/team_shot_put 2
error: athlete count must be odd

$ bin/team_shot_put 3
2.1: best shot put 18.407m
1.1: best shot put 11.913m
1.3: best shot put 22.105m
2.2: best shot put 7.890m
2.3: best shot put 24.681m
1.2: best shot put 14.009m
result 2:1, team 1 wins