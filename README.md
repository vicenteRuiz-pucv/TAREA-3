## Código de Ejemplo (tarea3)
Para ejecutar primero debemos compilar (en la carpeta raíz)
````
gcc tdas/*.c tarea3.c -Wno-unused-result -o tarea3
````

Y luego ejecutar:
````
./tarea3
````

## LO QUE FUNCIONA Y LO QUE NO

En este codigo todas las funcionalidades cumplen correctamente!

El unico caso o mejora la cual se podria implementar es el warning que salta en la funcion Transition de la conversion de tipos al guardar los avances y 
las fugas de memoria o la falta de uso de los free para liberar la memoria.

##EJEMPLO DE FUNCIONAMIENTO (la matriz no se visualiza apropiadamente pero al ingresar y ejecutar en el replit si, editando el readme tambien se ve bien !!)

~/workspace$ ./tarea3
Ingrese la dificultad del laberinto (porcentaje de obstáculos, 0 a 100): 90

Estado inicial del puzzle:
 A  .  .  .  .  .  .  . [X][X]
 .  .  . [X] .  .  .  . [X] . 
 .  . [X] . [X][X] . [X][X][X]
 .  . [X][X] .  .  . [X][X] . 
[X][X][X][X] . [X][X][X][X][X]
[X] . [X][X] .  . [X][X][X][X]
[X][X][X][X] .  . [X][X][X][X]
 . [X][X][X][X] . [X][X][X][X]
[X][X][X][X][X] .  .  .  .  . 
[X][X][X][X][X] .  .  . [X] M 
Distancia L1: 18



***** MENU ******
========================================
  Escoge método de búsqueda
========================================
1) Búsqueda en Profundidad
2) Buscar en Anchura
3) Buscar Mejor Primero
4) Salir
Ingrese su opción: 1
 I  *  *  *  *  *  *  * [X][X]
 .  .  . [X] .  .  *  * [X] . 
 .  . [X] . [X][X] * [X][X][X]
 .  . [X][X] *  *  * [X][X] . 
[X][X][X][X] * [X][X][X][X][X]
[X] . [X][X] *  * [X][X][X][X]
[X][X][X][X] .  * [X][X][X][X]
 . [X][X][X][X] * [X][X][X][X]
[X][X][X][X][X] *  *  *  *  * 
[X][X][X][X][X] .  .  . [X] A 
Presione una tecla para continuar...
