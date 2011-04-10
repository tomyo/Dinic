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
 * \mainpage My personal main page
 * \section una seccion
 * texto texto texto
 * texto loco
 *
 * \section otra seccion
 * texto re re re loco
 * texto
 */

/**
 * @file main.c
 *
 * \brief Lee los datos de la entrada, carga el network, corre BFS y muestra
 * el resultado
 *
 * Se ingresan por la entrada estandar las aristas
 * de la forma nodo_origen nodo_destino capacidad, hasta que se ingrese algo
 * que no cumpla con este formato, eso va a definir un Network, luego
 * correremos bfs sobre ese Network para encontrar un camino de distancia
 * minima de s (nodo 0) a t (nodo 1).
 * El resultado sera:
 * 1) "Existe el camino: " s nodo_1 ... nodo_n t
 *    "Con flujo:" flujo
 * 2) "No existe el camino :'("
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
