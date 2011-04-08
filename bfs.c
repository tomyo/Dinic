#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "bfs.h"
#include "defs.h"

#define INF -1 /* Funciona porque los pesos son unsigned */
#define min(x,y) x < y ? x : y

void destroy_step(gpointer bfs_step);
void destroy_step(gpointer bfs_step) {
    assert(bfs_step != NULL);
    free(bfs_step);
}

typedef struct bfs_t {
    Node *father;
    Weight max_flow;
} bfs_step;

bfs_result bfs(Network *self, Node s, Node t) {
    bfs_result result;
    Node curr_node;
    Weight flow;
    bool found_t = false;
    GHashTable *visited = NULL;  /* Visited nodes */
    GQueue *queue = NULL;        /* BFS queue */
    GList *result_path = NULL;   /* Shortest path */
    GList *forward_edges = NULL; /* value de network hashtable */
    bfs_step *state = NULL, *step = NULL;

    visited = g_hash_table_new_full(g_int_hash, g_int_equal,
                                    NULL, destroy_step);
    if(visited == NULL) {
            fprintf(stderr, "Memoria insuficiente\n");
            exit(1);
    }
    
    queue = g_queue_new();
    if(queue == NULL) {
            fprintf(stderr, "Memoria insuficiente\n");
            exit(1);
    }
    g_queue_push_tail(queue, &s);
    
    state = calloc(1, sizeof(*state));
    if(state == NULL) {
            fprintf(stderr, "Memoria insuficiente\n");
            exit(1);
    }
    
    state->father = NULL;
    state->max_flow = INF;
    g_hash_table_insert(visited, (gpointer) &s, (gpointer) state);

    while(not g_queue_is_empty(queue) and not found_t) {
        Weight flow_to_first = 0; 
        Node *first = NULL;
        bfs_step *priot_step = NULL;
        
        first = (Node *) g_queue_pop_head(queue);
        priot_step = (bfs_step *) g_hash_table_lookup(visited, (gpointer) first);
        assert(priot_step != NULL);
        
        flow_to_first = priot_step->max_flow;
        forward_edges = network_get_edges(self, *first);
        
        while(forward_edges != NULL and not found_t) {
            Edge *curr_edge = NULL;
            Weight curr_weigth;
            Node *neightbour = NULL;
            
            curr_edge = (Edge *) g_list_nth_data(forward_edges, 0);
            neightbour = edge_get_second(curr_edge);
                
            /* Hemos visitado este nodo? */
            if(g_hash_table_lookup(visited, neightbour) == NULL) { 
                g_queue_push_tail(queue, neightbour);
                state = calloc(1, sizeof(*state));
                if(state == NULL) {
                    fprintf(stderr, "Memoria insuficiente\n");
                    exit(1);
                }
                
                state->father = first;
                curr_weigth = edge_get_weight(curr_edge);
                state->max_flow = min(flow_to_first, curr_weigth);
                g_hash_table_insert(visited, (gpointer) neightbour, state);
                if(*neightbour == t) {
                    found_t = true;
                }
            }
            
            forward_edges = g_list_next(forward_edges);

        }

    }
    
    /* Ya encontramos camino, lo ponemos en una lista y lo devolvemos. */
    curr_node = t;
    printf("%d", t);
    result_path = g_list_prepend(result_path, &t);
    while (curr_node != s) {
        Node *dady;
        step = (bfs_step *) g_hash_table_lookup(visited, (gpointer) &curr_node);
        assert(step != NULL);
        dady = step->father;
        result_path = g_list_prepend(result_path, dady);
        curr_node = *dady;
    }
    step = (bfs_step *)g_hash_table_lookup(visited, (gpointer) &t);
    flow = step->max_flow;
    result.path = result_path;
    result.flow = flow;
    
    /* liberamos memomria usada */
    g_hash_table_destroy (visited);
    g_queue_free (queue);
    
    return result  ;    
}
