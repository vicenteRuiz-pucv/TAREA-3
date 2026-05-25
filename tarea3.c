#include <stdio.h>
#include <stdlib.h>
#include "tdas/list.h"
#include "tdas/heap.h"
#include "tdas/extra.h"
#include <string.h>
#include <time.h>
#include "tdas/stack.h"
#include "tdas/queue.h"

// Definición de la estructura para el estado del puzzle
typedef struct {
    int maze[N][N]; // Matriz NxN que representa el tablero
    int x;    // Posición x del agente
    int y;    // Posición x del agente
    int steps; // Pasos realizados hasta la posición actual
    List* actions; //Secuencia de movimientos para llegar al estado
    int visitado;
} State;

int distancia_L1(State* state) {
    return abs(state->x - (N-1)) + abs(state->y - (N-1));
}

// Función para imprimir el estado actual
void imprimirEstado(const State *estado) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (estado->x==i && estado->y==j) printf(" A ");
            else if (i == 0 && j == 0) printf(" I "); 
            else if (i == N-1 && j == N-1) printf(" M ");
            else if (estado->maze[i][j] == 0)
                printf(" . "); // Imprime un espacio en blanco para el espacio vacío
            else if(estado->maze[i][j] == 2)
                printf(" * ");
            else
                printf("[X]");
        }
        printf("\n");
    }
}

State crearEstadoInicial(int maze[N][N], int dificultad){
    State estado;
     // Copiar el laberinto generado al estado
    generate_maze(estado.maze,  dificultad);
    estado.x = 0;
    estado.y = 0;
    estado.steps = 0;
    estado.actions = list_create();
    return estado;
}

int is_final_state(State * state)
{
    if((state->x == (N - 1)) && (state->y == (N-1))) return 1;
    return 0;
}

//EL MOVIMIENTO SE DEFINE COMO ENTEROS/ ARRIBA=  1 / ABAJO = 2 / DERECHA = 3 / IZQUIERDA = 4
//que es lo que gestionara como se va almacenando todo? -> uno de los metodos de busqueda!!
State* transition(State * actual, int accion)
{
    //SEGUN LA ACCION(1,2,3 O 4) debemos actualizar la pos actual, sumar un paso. ademas de copiar el state y luego retornarlo..
    //se deben haceer verificaciones de si es un movimiento valido? o simplemente es la funcion que indica un movimiento aun que no sea      //valido?
    State * nuevo = (State *)malloc(sizeof(State));
    *nuevo= *actual;
    if(accion == 1) nuevo->x--;
    else if(accion == 2) nuevo->x++;
    else if(accion == 3) nuevo->y--;
    else if(accion == 4) nuevo->y++;
    nuevo->visitado= 0;
    nuevo->steps++;
    nuevo->actions = list_create();
    //cada vez que hay un movimiento transition se debe añadir al historial..
    void * temp = list_first(actual->actions);
    while(temp != NULL)
    {
        list_pushBack(nuevo->actions,temp);
        temp = list_next(actual->actions);
    }
    //cuidado al sacar de la lista, hay que volver a castear a int!
    list_pushBack(nuevo->actions,(void*)accion);
    
    return nuevo;
}

List * obtener_adyacentes(State* actual)
{
    //debemos retornar una lista de movivmiento!
    List* adyacentes= list_create();
    //ciclo para probar cada movimiento!
    for(int i = 1; i <= 4; i++)
    {
        int candidatoX = actual->x;
        int candidatoY = actual->y;
        if(i == 1) candidatoX--;
        else if(i == 2) candidatoX++;
        else if(i == 3) candidatoY--;
        else if(i == 4) candidatoY++;

        //ahora debemos filtrar si es una posicion valida!
        if((candidatoX < 0) || (candidatoY < 0) || (candidatoX >= N) || (candidatoY >= N)) continue;
        if(actual->maze[candidatoX][candidatoY] != 0) continue;

        //si pasamos el filtro el movimiento es valido y se agrega a la lista de estados adyacentes!
        State* estadoValido = transition(actual,i);
        list_pushBack(adyacentes,estadoValido);
    }
    return adyacentes;
}

//ALGORITMOS DE BUSQUEDA!
//Busqueda en Profundidad
void dfs(State* estado_inicial)
{
    //UTILIZAMOS UN STACK!
    Stack * pila = stack_create(NULL);
    State* copia= (State *) malloc(sizeof(State));
    *copia=*estado_inicial;
    
    stack_push(pila,copia);
    while(stack_top(pila) != NULL)
    {
         State* estado = (State *) stack_pop(pila);
         if(estado->visitado == 1) 
         {    
             free(estado);
             continue;
         }
         estado->visitado = 1;
         
         if(is_final_state(estado))
         {
             imprimirEstado(estado);
             break;
         }
         estado->maze[estado->x][estado->y] = 2;    
        
        //else continue;
        List* lista = obtener_adyacentes(estado);
        State* temp = (State*) list_first(lista);
        while(temp)
            {
                stack_push(pila,temp);
                temp= (State*) list_next(lista);
            }
        free(lista);
    }
}

//Busqueda en anchura con cola!
void bfs(State* estado_inicial)
{
    //UTILIZAMOS UN QUEUE!
    Queue * cola = queue_create(NULL);
    State* copia= (State *) malloc(sizeof(State));
    *copia=*estado_inicial;
    
    queue_insert(cola,copia);
    while(queue_front(cola) != NULL)
    {
         State* estado = (State *) queue_remove(cola);
         if(estado->visitado == 1)
         {
             free(estado);
             continue;
         }  
         estado->visitado = 1;

         if(is_final_state(estado))
         {
             imprimirEstado(estado);
             break;
         }
         estado->maze[estado->x][estado->y] = 2;    

        //else continue;
        List* lista = obtener_adyacentes(estado);
        State* temp = (State*) list_first(lista);
        while(temp)
            {
                queue_insert(cola,temp);
                temp= (State*) list_next(lista);
            }
        free(lista);
    }
}
//busqueda A*
void best_first(State* estado_inicial)
{
    //UTILIZAMOS UN HEAP!
    Heap * heap = heap_create();
    State* copia= (State *) malloc(sizeof(State));
    *copia=*estado_inicial;

    int prioridad= copia->steps + distancia_L1(copia);
    heap_push(heap,copia,-prioridad);
    while(heap_top(heap) != NULL)
    {
         State* estado = (State *) heap_top(heap);
         heap_pop(heap);
         if(estado->visitado == 1) 
         {    
             free(estado);
             continue;
         }
         estado->visitado = 1;

         if(is_final_state(estado))
         {
             imprimirEstado(estado);
             break;
         }
         estado->maze[estado->x][estado->y] = 2;    

        //else continue;
        List* lista = obtener_adyacentes(estado);
        State* temp = (State*) list_first(lista);
        while(temp)
            {
                int prioridadF= temp->steps + distancia_L1(temp);
                heap_push(heap,temp,-prioridadF);
                
                temp= (State*) list_next(lista);
            }
        free(lista);
    }
}

int main() {
    // Inicializar la semilla de aleatoriedad
    srand(time(NULL));

    int maze[N][N];
    int dificultad;

    // Solicitar la dificultad al usuario con validación
    do {
        printf("Ingrese la dificultad del laberinto (porcentaje de obstáculos, 0 a 100): ");
        scanf("%d", &dificultad);
        if (dificultad < 0 || dificultad > 100) {
            printf("Error: Por favor ingrese un valor válido entre 0 y 100.\n");
        }
    } while (dificultad < 0 || dificultad > 100);

    // Estado inicial del puzzle
    // Laberinto generado con la dificultad ingresada por el usuario
    State estado_inicial = crearEstadoInicial(maze, dificultad);

    // Imprime el estado inicial
    printf("\nEstado inicial del puzzle:\n");
    imprimirEstado(&estado_inicial);

    printf("Distancia L1: %d\n", distancia_L1(&estado_inicial));
    
    //EJEMPLO-------------------------------------------------------------------------------------------------------------------
    // Ejemplo de heap (cola con prioridad)
    
    //printf("\n***** EJEMPLO USO DE HEAP ******\nCreamos un Heap e insertamos 3 elementos con distinta prioridad\n");
    //Heap* heap = heap_create();
    //char* data = strdup("Cinco");
    //printf("Insertamos el elemento %s con prioridad -5\n", data);
    //heap_push(heap, data, -5 /*prioridad*/);
    //data = strdup("Seis");
    //printf("Insertamos el elemento %s con prioridad -6\n", data);
    //heap_push(heap, data, -6 /*prioridad*/);
    //data = strdup("Siete");
    //printf("Insertamos el elemento %s con prioridad -7\n", data);
    //heap_push(heap, data, -7 /*prioridad*/);

    //printf("\nLos elementos salen del Heap ordenados de mayor a menor prioridad\n");
    //while (heap_top(heap) != NULL){
    //    printf("Top: %s\n", (char*) heap_top(heap));      
    //    heap_pop(heap);
    //}
    //printf("No hay más elementos en el Heap\n");
    
    //FIN DEL EJEMPLO-------------------------------------------------------------------------------------------------------------
    //EJEMPLO EN MAIN PARA GRAFO IMPLICITO!
    printf("\n");
    printf("\n");
    /*
    printf("Ejemplo de Grafo Implicito\n");

    List * adyacentes = obtener_adyacentes(&estado_inicial);
    State * temp = (State *) list_first(adyacentes);
    int contador = 1;
    if(temp == NULL)
    {
        printf("No hay movimientos Disponibles\n");
    }
    else
    {
        while(temp != NULL) 
            {
                printf("Movimiento Disponible #%i\n",contador);
                printf("Posicion: (X: %d | Y: %d)\n",temp->x,temp->y);
                imprimirEstado(temp);
                temp= (State *)list_next(adyacentes);
                contador++;
            }
    }
    printf("--------------------------------------------------------------\n");
    */
    //OPCIONES DE LAS FUNCIONALIDADES
    char opcion;
    do {
        printf("\n***** MENU ******\n");
        puts("========================================");
        puts("     Escoge método de búsqueda");
        puts("========================================");

        puts("1) Búsqueda en Profundidad");
        puts("2) Buscar en Anchura");
        puts("3) Buscar Mejor Primero");
        puts("4) Salir");

        printf("Ingrese su opción: ");
        scanf(" %c", &opcion);

        switch (opcion) {
        case '1':
          dfs(&estado_inicial);
          break;
        case '2':
          bfs(&estado_inicial);
          break;
        case '3':
          best_first(&estado_inicial);
          break;
        }

        // Evitamos pausar y limpiar pantalla si el usuario eligió salir
        if (opcion != '4') {
            presioneTeclaParaContinuar();
            limpiarPantalla();
        }

  } while (opcion != '4');

  return 0;
}
