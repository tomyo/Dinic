#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "bfs.h"
#include "defs.h"
#include "hashtable/hashtable.h"
#include "queue.h"

#define INF -1 /* Funciona porque los pesos son unsigned */
#define min(x,y) x < y ? x : y

void destroy_step(void *bfs_step);

/**
 *\brief
 *Paso de bfs: contiene un padre y el flujo hasta entonces.
 *
 */
typedef struct {
    Node *father;
    Flow max_flow;
} bfs_step;

void destroy_step(void *bfs_step) {
    assert(bfs_step != NULL);
    free(bfs_step);
}

bfs_result bfs(Network *net, Node s, Node t) {
    bfs_result result;
    Node curr_node = 0, *pointer_to_t = NULL;
    Flow final_flow = 0;
    bool found_t = false;
    hash_table_t *visited = NULL;  /* Visited nodes */
    Queue *queue = NULL;        /* BFS queue */
    SList *result_path = NULL;   /* Shortest path */
    SList *forward_edges = NULL; /* value de network hashtable */
    bfs_step *state = NULL, *step = NULL;

    visited = ht_new_full(MODE_ALLREF, NULL, destroy_step);
    memory_check(visited);

    queue = queue_new();
    memory_check(queue);

    queue_push_tail(queue, &s);

    state = calloc(1, sizeof(*state));
    memory_check(state);

    state->father = NULL;
    state->max_flow = INF;

    ht_insert(visited, (void *) &s, (void *) state);

    int i = 0;

    while(not queue_is_empty(queue) and not found_t) {
        Node *first = NULL;
        Capacity flow_to_first = 0;
        bfs_step *priot_step = NULL;
        i++;

        first = (Node *) queue_pop_head(queue);

        priot_step = (bfs_step *) ht_lookup(visited, first);
        assert(priot_step != NULL);

        flow_to_first = priot_step->max_flow;
        forward_edges = network_get_edges(net, *first);


        while(forward_edges != NULL and not found_t) {
            Edge *curr_edge = NULL;
            Capacity curr_weigth = 0;
            Node *neighbour = NULL;

            curr_edge = (Edge *) slist_nth_data(forward_edges, 0);
            neighbour = edge_get_second(curr_edge);

            /* Si no hemos visitado este nodo, entonces no hay registro
             *en la hashtable y un lookup devuelve NULL */
            if(ht_lookup(visited, neighbour) == NULL) {

                queue_push_tail(queue, neighbour);

                state = calloc(1, sizeof(*state));
                memory_check(state);

                state->father = first;
                curr_weigth = edge_get_capacity(curr_edge);
                state->max_flow = min(flow_to_first, curr_weigth);
                ht_insert(visited, neighbour, state);

                if(*neighbour == t) {
                    pointer_to_t = neighbour;
                    final_flow = state->max_flow;
                    found_t = true;
                }
            }
            forward_edges = slist_next(forward_edges);
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
         *paso por valor, por lo que fuera de esta funcion esa direccion no
         *tiene sentido alguno
         */
        result_path = slist_prepend(result_path, pointer_to_t);
        while (curr_node != s) {
            Node *predecessor;
            step = (bfs_step *) ht_lookup(visited, &curr_node);
            assert(step != NULL);
            predecessor = step->father;
            assert(predecessor != NULL);
            curr_node = *predecessor;
            result_path = slist_prepend(result_path, predecessor);
        }

        result.path = result_path;
        result.flow = final_flow;
    }

    /* liberamos memomria usada */
    ht_destroy(visited);
    queue_free(queue);

    return result  ;
}
