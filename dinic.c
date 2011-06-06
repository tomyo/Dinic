#include <assert.h>
#include <stdlib.h>
#include <limits.h>

#include "dinic.h"
#include "network.h"
#include "edge.h"
#include "node.h"
#include "queue.h"
#include "stack.h"
#include "defs.h"


/* ******************* Estructuras internas ****************** */


/* ******************* Funciones internas ******************** */

bool aux_network_find_blocking_flow(dinic_t *, Network *, bool);
static int compare_nodes(const void *x1, const void *x2);


/* TODO: Descripcion */
static int compare_nodes(const void *x1, const void *x2) {
    return ((*(const Node *)x1) == (*(const Node *)x2))?0:1;
}

/**
 * @brief Determina si se puede mandar flujo o devolverlo por edge.
 * @param edge el lado a examinar
 * @param mode 'f' si quiere mandar flujo o 'b' si quiero devolverlo
 */
static bool can_send_flow(Edge *edge, char mode) {
    /* TODO: Modificar esto porque ya no hay lados backwards */
    /* Asumimos que primero se pregunta por los lados fordward */
    bool result = false;

    assert(mode == 'f' || mode == 'b');

    if (mode == 'f') {
        /* Podemos mandar flujo? */
        result = ((edge_get_capacity(edge) - edge_get_flow(edge)) > 0);
    } else {
        /* Podemos devolver flujo? */
        /* Por construccion, no deberiamos tener lados backwards que no se
         * pueden usar */
        assert(edge_get_flow(edge) > 0);
        result = true;
    }

    return result;
}

/* ************************ Funciones ************************ */

/**
 * Funcion de creacion de un nuevo network auxiliar a partir de un
 * network y un par de nodos origen y destino (ambos dentro de s_dinic).
 * @param s_dinic data donde esta el network y los nodos origen y destino.
 * @returns network (network auxiliar)
 */
Network *aux_network_new(dinic_t *data) {
    Network *main_network = NULL, *result = NULL;
    Queue *bfs_queue = NULL, *next_level = NULL;
    Node *current_node = NULL;
    Node s = 0, t = 0;
    SList *edges = NULL;
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

    /* Inicializando variables */
    result = network_create(); /* NA (vacio) */
    bfs_queue = queue_new();   /* BFS queue (vacia) */
    next_level = queue_new(); /* Siguiente nivel queue (vacia) */

    queue_push_head(bfs_queue, &s); /* Empezamos desde *s* */
    while (!queue_is_empty(bfs_queue) && !is_t_found) {
        SList *current_edges = NULL;
        Edge *current_edge = NULL;
        Node *neighbour = NULL;

        /* 2 */
        while (!queue_is_empty(bfs_queue)) {
            current_node = queue_pop_head(bfs_queue);
            edges = network_get_edges(main_network, *current_node);
            /* edges tiene todas las posibles aristas del proximo nivel */
            current_edges = edges;


            while (current_edges != NULL) {
                char mode = 'f';

                current_edge = slist_head_data(current_edges);

                if (*edge_get_first(current_edge) == *current_node) {
                    mode = 'f';
                    neighbour = edge_get_second(current_edge);
                } else {
                    mode = 'b';
                    neighbour = edge_get_first(current_edge);
                }

                /* Ver si corresponde agregar la arista al NA (result) */
                if (!network_has_node(result, *neighbour)) {
                    /* Si no esta en el network, solo me tengo que fijar que
                     * puedo mandar o devolver flujo */
                    if (can_send_flow(current_edge, mode)) {
                        _network_add_edge(result, current_edge, mode);
                        queue_push_head(next_level, neighbour);
                        if (*neighbour == t) {
                            is_t_found = true;
                        }
                    }
                } else {
                    /* Solo la agregamos si pertenece al nivel siguiente */
                    if (queue_find_custom(next_level, neighbour, compare_nodes)
                        && can_send_flow(current_edge, mode)) {
                        _network_add_edge(result, current_edge, mode);
                    }
                }
                current_edges = slist_next(current_edges);

            }

            /* Revisamos (y agregamos segun correspondia) todas las
             * arista que contenian los vecinos de current_node */
            /* No hay que liberar esto, es parte de network */
            /* slist_free(edges); */
        }

        /* 3 */
        queue_free(bfs_queue); /* supuestamente vacia */
        bfs_queue = next_level;
        next_level = queue_new();
    }
    queue_free(bfs_queue);
    queue_free(next_level);

    return result;
}

/* Funcion Auxiliar de DinicFlow, para no "ensuciar" a la funcion antes
 * mencionada.
 */
static void flow_pretty_print(dinic_t *data, DinicFlow *to_print) {
    /* if to_print->path */
}

/**
 * Funcion que busca caminos entre nodo origen y destino.
 * @note Los caminos y los flujos son almacenados en s_dinic->result.
 * @param data donde esta el network y los nodos origen-destino.
 * @param network network auxiliar donde operar.
 * @param verbose si va a imprimir la salida.
 * @returns DinicFlow con el camino encontrado y su flujo
 */
DinicFlow *aux_network_find_flow(dinic_t *data, Network *aux_net, bool verbose) {
    Node *expected_node = NULL;
    Stack *flow_edges = NULL;
    SList *neighbours = NULL, *path = NULL;
    bool is_t_found = false;
    DinicFlow *result = NULL;

    /* Precondicion */
    assert(network_has_node(aux_net, data->t));

    result = (DinicFlow *) calloc(1, sizeof(*result));
    memory_check(result);

    /* En flow_edges vamos mateniendo las aristas por las que pasamos */
    flow_edges = stack_new();

    /* TODO: comentario */
    while (!is_t_found && (!stack_is_empty(flow_edges) ||
                   slist_is_empty(network_get_edges(aux_net, data->s)))) {
        Edge *current_edge = NULL;
        char mode = 'f';

        /* TODO: comentario */
        if (stack_is_empty(flow_edges)) {
            SList *edges = NULL;

            edges = network_get_edges(aux_net, data->s);
            stack_push(flow_edges, slist_head_data(edges));
            expected_node = &data->s;
        }

        current_edge = (Edge *) stack_head(flow_edges);
        /* TODO: Comentar esto, con dibujito */
        if (*edge_get_first(current_edge) == *expected_node) {
            mode = 'f';
            expected_node = edge_get_second(current_edge);
        } else {
            mode = 'b';
            expected_node = edge_get_first(current_edge);
        }
        /* TODO: Cambiar nombre neighbours */
        neighbours = network_get_edges(aux_net, *expected_node);

        if (!slist_is_empty(neighbours)) {
            Edge *edge = NULL;

            edge = slist_head_data(neighbours);

            /* TODO: Ver si se llego a t. Comentar porque t no tiene vecinos. */
            /* TODO: Ahora no es verdad */
            if (can_send_flow(current_edge, mode)) {
                /* Tengo flujo para mandar, lo agrego al camino actual */
                stack_push(flow_edges, edge);
            } else {
                /* No puedo mandar nada por ese edge, lo borro del network, pero
                 * es menester actualizar mode */
                mode = (*edge_get_first(current_edge) == *expected_node)?'f':'b';
                _network_del_edge(aux_net, edge, mode);
            }
        } else {
            /* No tiene vecinos */
            if (*expected_node == data->t) {
                is_t_found = true;
            } else {
                /* Ese camino no me lleva a ningun lado, borremoslo del network
                 * y quitemoslo de nuestros posibles caminos */
                _network_del_edge(aux_net, current_edge, mode);
                stack_pop(flow_edges);
                if (mode == 'f') {
                    expected_node = edge_get_first(current_edge);
                } else {
                    expected_node = edge_get_second(current_edge);
                }
            }
        }
    }

    /* path es una lista de la forma [(s, a), (a, b), ..., (r, t)]  o []*/
    path = slist_reverse(stack_to_list(flow_edges));
    stack_free(flow_edges);

    {
        Flow current_flow = UINT_MAX;
        Node next_node = data->s;
        while (path != NULL) {
            /* Calcular el valor del flujo de current_flow) */
            Edge *edge = slist_head_data(path);
            if(*edge_get_first(edge) == next_node) {
                /* Forward */
                current_flow = min(current_flow, edge_get_capacity(edge) - edge_get_flow(edge));
            } else {
                /* Backward */
                current_flow = min(current_flow, edge_get_flow(edge));
            }
        }

        result->path = path;
        result->flow_value = current_flow;
    }

    if (verbose) {
        /* TODO: Escribir la funcion */
        flow_pretty_print(data, result);
    }

    return result;
}

bool aux_network_find_blocking_flow(dinic_t *data, Network *aux_net,
                                    bool verbose) {
    DinicFlow *partial = NULL;
    static unsigned int aux_net_number = 1;
    bool result = false;

    if (verbose) {
        printf("N.A. %d:\n", aux_net_number);
    }
    partial = aux_network_find_flow(data, aux_net, verbose);
    if (slist_is_empty(partial->path)) {
        result = true;
    }

    /* Hasta que no halla camino de data->s a data->t*/
    while(slist_is_empty(partial->path)){
        SList *iter = partial->path;

        while (slist_is_empty(iter)){
            /* Para cada arista de aux_net actualiza el valor de la arista
             * basado en el valor de partial. */
            Edge *current = NULL;
            Flow new_flow = 0;

            current = slist_head_data(iter);
            new_flow = edge_get_flow(current) - partial->flow_value;
            edge_update_flow(current, new_flow);
        }
        /* Corremos una vez mas el DFS en busca de un flujo adicional */
        slist_free(partial->path);
        free(partial);
        partial = aux_network_find_flow(data, aux_net,verbose);
    }

    return result;
}

dinic_result *dinic(Network *network, const Node s, const Node t, bool verbose) {
    dinic_t data;
    Network *aux_net = NULL;
    bool found_flow = true;

    /* Inicializando las estructuras */
    data.network = network;
    data.s = s;
    data.t = t;
    data.result = calloc(1, sizeof(dinic_result));

    data.result->flow_value = 0;
    data.result->max_flow = NULL;

    while (found_flow) {
        aux_net = aux_network_new(&data);
        found_flow = aux_network_find_blocking_flow(&data, aux_net, verbose);
        
        if (found_flow) {
            /* Encontramos flujo (nuevos caminos), pero aun no es maximal, para
             * eso debemos encontrar el corte minimal */
            network_free(aux_net);
        }
        /* Aca ya tenemos Max-Flow y Min-Cut (we're good to go) */
    }
    /* El Corte Minimal son los nodos que quedan en el ultimo NA */
    data.result->min_cut = network_get_nodes(aux_net);
    
    /* Post Condicion */
    assert(slist_find_custom(data.result->min_cut, &t, compare_nodes) == NULL);

    return data.result;
}

