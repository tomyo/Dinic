#include <assert.h>
#include <stdlib.h>

#include "dinic.h"
#include "network.h"
#include "edge.h"
#include "node.h"
#include "queue.h"
#include "stack.h"

extern bool print_aux_paths;

/**
 * @brief Estructura interna de Dinic
 */
typedef struct _dinic_t {
    /** El network original que va a ir cambiando durante el algoritmo */
    Network *network;

    /** Nodo origen */
    Node *s;

    /** Nodo destino */
    Node *t;

    /** Un network que va a contener los lados backwards */
    Network *backwards;

    /** La estructura que devuelve el resultado */
    dinic_result *result;
} dinic_t;

/**
 * @brief Deterina si se puede agregar edge al network auxiliar.
 * @param fwd_net network con los lados forward.
 * @param bwd_net network con los lados backward.
 * @param edge el lado a examinar
 */
static bool can_add_edge(Network *fwd_net, Network *bwd_net, Edge *edge) {
    /* Asumimos que primero se pregunta por los lados backward */
    Node *node = edge_get_first(edge);
    bool result = false;

    if (!slist_is_empty(network_get_edges(fwd_net, node))) {
        /* Podemos mandar flujo? */
        result = ((edge_get_capacity(edge) - edge_get_flow(edge)) > 0);
    }

    if (!result) {
        /* Podemos devolver flujo? */
        if (!slist_is_empty(network_get_edges(bwd_net, node))) {
            result = true;
        }
   }
   return result;
}

/**
 * Funcion de creacion de un nuevo network auxiliar a partir de un
 * network y un par de nodos origen y destino (ambos dentro de s_dinic).
 * @param s_dinic data donde esta el network y los nodos origen y destino.
 * @returns network (network auxiliar)
 */
static Network *aux_network_new(dinic_t *data) {
    Network *main_network = NULL, *backwards = NULL, *result = NULL;
    Queue *bfs_queue = NULL, *next_level = NULL;
    Node *current_node = NULL;
    Node *s = NULL, *t = NULL;
    SList *fwd_edges = NULL, *bwd_edges = NULL, *edges = NULL;
    bool is_t_found = false;

    assert(data != NULL);

    /* Algoritmo Basicos:
     * 1) Revisar bfs_queue:
     *        a) Si esta vacia, terminamos -> Devolver Netwok Auxiliar (result)
     *        b) Si esta t, terminamos -> Devolver Netwok Auxiliar (result)
     *
     * 2) Poner aristas vecinas backward y forward en *result* y en la cola
     *    de nodos del proximo nivel segun corresponda (*Lea Dinic) por cada
     *    nodo de la cola de BFS.
     *
     * 3) Una vez vacia la cola BFS, Reemplazarla por la cola de nodos del
     *    siguiente nivel creada en 2) (nueva cola a iterar) -> GOTO 1).
     */

    /* Referencias a datos de *data* que vamos a necesitar constantemente */
    main_network = data->network;
    s = data->s;
    t = data->t;
    backwards = data->backwards;

    /* Inicializando variables */
    result = network_create(); /* NA (vacio) */
    bfs_queue = queue_new();   /* BFS queue (vacia) */
    next_level = queue_new(); /* Siguiente nivel queue (vacia) */

    queue_push_head(bfs_queue, s); /* Empezamos desde *s* */
    while (!queue_is_empty(bfs_queue) && !is_t_found) {
        SList *current_edges = NULL;
        Edge *current_edge = NULL;
        Node *neighbour = NULL;

        /* 2 */
        while (!queue_is_empty(bfs_queue)) {
            current_node = queue_pop_head(bfs_queue);
            fwd_edges = network_get_edges(main_network, current_node);
            bwd_edges = network_get_edges(backwards, current_node);
            edges = slist_concat(fwd_edges, bwd_edges);
            /* edges tiene todas las posibles aristas del proximo nivel */
            current_edges = edges;

            while (current_edges != NULL) {
                current_edge = slist_head_data(current_edges);
                neighbour = edge_get_second(current_edge);

                /* Ver si corresponde agregar la arista al NA (result) */
                if (!network_has_node(result, neighbour)) {
                    if (can_add_edge(main_network, backwards, current_edge)) {
                        network_add_edge(result, current_edge);
                        queue_push_head(next_level, neighbour);
                    }
                    if (neighbour == t) {
                        is_t_found = true;
                    }
                } else {
                    /* Solo la agregamos si pertenece al nivel siguiente */
                    if (queue_find(next_level, neighbour) &&
                        can_add_edge(main_network, backwards, current_edge)) {
                        network_add_edge(result, current_edge);
                    }
                }
                current_edges = slist_next(current_edges);

            }

            /* Revisamos (y agregamos segun correspondia) todas las
             * arista que contenian los vecinos de current_node */
            slist_free(edges);
        }

        /* 3 */
        queue_free(bfs_queue); /* supuestamente vacia */
        bfs_queue = next_level;
        next_level = queue_new();
    }
    queue_free(bfs_queue);

    return result;
}

/**
 * Funcion que busca caminos entre nodo origen y destino hasta
 * saturar todos los caminos (flujo bloqueante)
 * @note Los caminos y los flujos son almacenados en s_dinic->result.
 * @param s_dinic data donde esta el network y los nodos origen-destino.
 * @param network network auxiliar donde operar.
 * @param verbose si va a imprimir la salida.
 * @returns network auxiliar
 */
static bool aux_network_find_blocking_flow(dinic_t *data, Network *aux_net, bool verbose) {
    Stack *dfs_stack = NULL;
    SList *neighbours = NULL;
    bool found_flow = false;
    SList *current_path = NULL;

    /* Vamos a hacer DFS en el grafo buscando caminos en el network auxiliar
     * y mandando lo que se pueda mandar por el camino. */

    dfs_stack = stack_new();
    stack_push(dfs_stack, data->s); /* Empezamos en s */

    while (!stack_is_empty(dfs_stack)) {
        Node *current_node = NULL;

        current_node = (Node *) stack_pop(dfs_stack);
        neighbours = network_get_edges(aux_net, current_node);

        while (neighbours != NULL) {
            Edge *edge = NULL;
            Node *neighbour = NULL;

            if (!slist_is_empty(neighbours)){
                edge = slist_head_data(neighbours);
                neighbour = edge_get_second(edge);

                if (edge_get_flow(edge) > 0) {
                    /* Tengo flujo para mandar, lo agrego al camino actual */
                    current_path = slist_append(current_path, edge);
                    stack_push(dfs_stack, neighbour);
                } else {
                    /* No puedo avanzar mas por este camino, tengo que volver
                     * hasta el ultimo punto que se divide el camino (que es
                     * justo el edge de current_path que tiene como segundo
                     * elemento el que este en el tope de la pila actualmente */

                    Node *stack_node = NULL;
                    Node *path_node = NULL;
                    keep_fixing = true;

                    /* Necesito ver el tope del stack
                     * pero no sacarlo */
                    stack_node = stack_pop(dfs_stack);
                    stack_push(dfs_stack, stack_node)

                    while (keep_fixing) {
                        path_edge = slist_head_data(current_path);
                        if (edge_get_second(path_edge) != stack_node){
                            /* Sacar el elemento de current_path */
                        } else {
                            keep_fixing = false;
                        }
                    }
                }

                if (neighbour == data->t){
                    /* Llegue a t por este camino, tengo que updatear
                     * los flujos de current_path con el minimo de ellos */
                }
            } else {
                /* No tiene vecinos: hay que hacer algo? */
            }
        }
    }

    return found_flow;
}

dinic_result *dinic(Network *network, Node *s, Node *t, bool verbose) {
    dinic_t data;
    Network *aux_net = NULL;
    bool found_flow = true;

    /* Inicializando las estructuras */
    data.network = network;
    data.s = s;
    data.t = t;
    data.backwards = network_create();
    data.result = calloc(1, sizeof(dinic_result));

    data.result->flow_value = 0;
    data.result->max_flow = NULL;

    while (found_flow) {
       aux_net = aux_network_new(&data);
       found_flow = aux_network_find_blocking_flow(&data, aux_net, verbose);
       network_destroy(aux_net);
    }
    network_destroy(data.backwards);
    return data.result;
}

