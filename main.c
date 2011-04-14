#include <stdio.h>
#include <stdbool.h>
#include <glib-2.0/glib.h>
#include "bfs.h"
#include "parser.h"
#include "defs.h"

#define s 0
#define t 1
#define SUCCESS_RESULT "Existe el camino:"
#define FAIL_RESULT    "No existe camino :'("

/**
 * \mainpage bfsNET
 *
 * \section Introduction
 * El proyecto implementa BFS para encontrar algún
 * camino de longitud mínima (en caso de que exista) entre dos vértices de un network.
 *
 * El programa lee por la linea de comandos una serie de aristas,
 * según el formato definido por el parser.
 *
 * Existen dos posibles resultados:
 *
 * \b 1) Una lista con los vértices de dicho camino, junto con el
 * flujo máximo del mismo.
 *
 * \b 2) Un mensaje de error, en caso de que el camino no exista.
 *
 * \section Implementacion
 * Hemos optado por utilizar algunos TADs de la librería GLib (GHash, GList y GQueue)
 * por encontrarnos ya familiarizados con la misma y su correcto funcionamiento. Además,<BR>
 * esta decisión acelero en gran medida el desarollo del proyecto, utilizando
 * herramientas ya probadas y ahorrando tiempo en depuracion.
 * Por lo tanto es esencial para poder compilar, tener instalada la librería.
 *
 * Para representar el network utilizamos una tabla Hash (GHash), que mappea cada nodo
 * a una lista (GList) de las aristas conectadas al mismo.
 *
 * La Elección de dicha tabla se debe a que el orden de acceso a los vecinos es
 * de orden constante, y al ser esta, una de las operaciones mas utilizadas en
 * el algoritmo bfs, se logra obtener un buen rendimiento.
 *
 * En el algoritmo de BFS, utilizamos:
 *
 * \b 1) Una Hash Table (GHash) para mappear un nodo con su padre y el flujo máximo
 * que se envía hasta ese vértice, que al final utilizamos para recrear el camino y el flujo.
 *
 * \b 2) Una Cola (GQueue) para recorrer el network.
 *
 * \section other_aspects Otros Aspectos
 * Se han incluido, con el fin de asegurar un desarollo mas confiable, una
 * bateria de tests y un generador de networks (make_network.py). Pudiendo de
 * esta manera identificar errores prematuramente (ahorrando tiempo en depuracion)
 * y corroborando que cumple con la semantica deseada.
 *
 * La batería de tests utiliza el Framwork check y si se tiene correctamente
 * instalada se puede correr el comando $ make test esperando un resultado de %%100
 * el cual significa que todos los tests han sido  aprobados.
 * Dichos tests fueron escritos sin pensar en la implentación
 * interna sino sabiendo el comportamiento que deberia tener cada uno de los
 * módulos a probar. El código que se ejecuta en cada test puede ser visto en los
 * archivos tests/test_<nombre_módulo>.c
 */

/**
 * @file main.c
 *
 * \brief Lee los datos de la entrada, carga el network, corre BFS y muestra
 * el resultado
 *
 * Se ingresan por la entrada estandar las aristas con el formato
 * "Nodo_origen Nodo_destino Capacidad", hasta que se ingrese algo
 * que no lo cumpla, eso va a definir un network.
 *
 * Luego se corre bfs (sobre el network creado) para encontrar un camino de distancia
 * minima entre el nodo 's' (nodo 0) y el nodo 't' (nodo 1).
 * El resultado sera:
 *
 * <b>1)</b> "Existe el camino: " s nodo_1 ... nodo_n t
 *    "Con flujo:" flujo.
 *
 * <b>2)</b> "No existe el camino :'(".
 */
int main(void) {
    Edge *edge = NULL;
    Network *network = NULL;
    bool reach_EOF = false;
    bfs_result result;
    GSList *result_path = NULL, *iter = NULL;
    Weight result_flow = 0;
    Node *node = NULL;

    /* Creamos network vacio */
    network = network_create();

    /* Parsear entrada estandar */
    while(not reach_EOF) {
        edge = parse_edge(stdin);
        if(edge == NULL) {
            reach_EOF = true;
        } else {
            network_add_edge(network, edge);
        }
    }

    /* Ya tenemos el network, a correrle BFS ahora */
    result = bfs(network, s, t);
    result_path = result.path;
    result_flow = result.flow;

    if(result_path == NULL) {
        /* El camino no existia */
        puts(FAIL_RESULT);
    } else {
        /* Encontre un camino */
        printf(SUCCESS_RESULT);
        /* Mostremos el camino*/
        iter = result_path;
        while(iter != NULL) {
                node = (Node *) g_slist_nth_data(iter, 0);
                iter = g_slist_next(iter);
                printf(" %d", *node);
        }
        puts("");
        printf("Con flujo: %d\n", result_flow);

        /* Liberemos la lista */
        g_slist_free(result_path);
    }

    /* Liberar la memoria alocada por los programas */
    network_destroy(network);

    return 0;
 }
