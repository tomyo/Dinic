#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "bfs.h"
#include "defs.h"

#define INF -1 /* Funciona porque los pesos son unsigned */
#define min(x,y) x < y ? x : y

void destroy_step(gpointer bfs_step);
void memory_check(gpointer m);

/**
 * \brief
 * Paso de bfs: contiene un padre y el flujo hasta entonces.
 *
 */
typedef struct {
    Node *father;
    Weight max_flow;
} bfs_step;

void destroy_step(gpointer bfs_step) {
    assert(bfs_step != NULL);
    free(bfs_step);
}

void memory_check(gpointer m){
    if(m == NULL) {
        fprintf(stderr, "Memoria insuficiente\n");
        exit(1);
    }
}

bfs_result bfs(Network *net, Node s, Node t) {
    bfs_result result;
    Node curr_node = 0, *pointer_to_t = NULL;
    Weight final_flow = 0;
    bool found_t = false;
    GHashTable *visited = NULL;  /* Visited nodes */
    GQueue *queue = NULL;        /* BFS queue */
    GSList *result_path = NULL;   /* Shortest path */
    GSList *forward_edges = NULL; /* value de network hashtable */
    bfs_step *state = NULL, *step = NULL;

    visited = g_hash_table_new_full(g_int_hash, g_int_equal, NULL, destroy_step);
    memory_check((gpointer) visited);

    queue = g_queue_new();
    memory_check((gpointer) queue);

    g_queue_push_tail(queue, &s);

    state = calloc(1, sizeof(*state));
    memory_check((gpointer) state);

    state->father = NULL;
    state->max_flow = INF;

    g_hash_table_insert(visited, (gpointer) &s, (gpointer) state);

    while(not g_queue_is_empty(queue) and not found_t) {
        Node *first = NULL;
        Weight flow_to_first = 0;
        bfs_step *priot_step = NULL;

        first = (Node *) g_queue_pop_head(queue);
        priot_step = (bfs_step *) g_hash_table_lookup(visited, (gpointer) first);
        assert(priot_step != NULL);

        flow_to_first = priot_step->max_flow;
        forward_edges = network_get_edges(net, *first);

        while(forward_edges != NULL and not found_t) {
            Edge *curr_edge = NULL;
            Weight curr_weigth = 0;
            Node *neighbour = NULL;

            curr_edge = (Edge *) g_slist_nth_data(forward_edges, 0);
            neighbour = edge_get_second(curr_edge);

            /* Si no hemos visitado este nodo, entonces no hay registro
             * en la hashtable y un lookup devuelve NULL */
            if(g_hash_table_lookup(visited, neighbour) == NULL) {
                g_queue_push_tail(queue, neighbour);
                state = calloc(1, sizeof(*state));
                memory_check((gpointer) state);

                state->father = first;
                curr_weigth = edge_get_weight(curr_edge);
                state->max_flow = min(flow_to_first, curr_weigth);
                g_hash_table_insert(visited, (gpointer) neighbour, state);

                if(*neighbour == t) {
                    pointer_to_t = neighbour;
                    final_flow = state->max_flow;
                    found_t = true;
                }
            }

            forward_edges = g_slist_next(forward_edges);
        }

    }

    /* Que paso con la corrida de BFS ? */
    if(not found_t) {
        result.path = NULL;
        result.path = 0;
    } else {
        /* Ya encontramos el camino, lo ponemos en una lista y lo devolvemos. */
        curr_node = t;
        /* Usamos pointer_to_t y no &t, pues t es una variable local que se
         * paso por valor, por lo que fuera de esta funcion esa direccion no
         * tiene sentido alguno
         */
        result_path = g_slist_prepend(result_path, pointer_to_t);
        while (curr_node != s) {
            Node *predecessor;
            step = (bfs_step *) g_hash_table_lookup(visited,
                                                    (gpointer) &curr_node);
            assert(step != NULL);
            predecessor = step->father;
            assert(predecessor != NULL);
            curr_node = *predecessor;
            result_path = g_slist_prepend(result_path, predecessor);
        }

        result.path = result_path;
        result.flow = final_flow;
    }

    /* liberamos memomria usada */
    g_hash_table_destroy (visited);
    g_queue_free (queue);

    return result  ;
}
