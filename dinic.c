#include <assert.h>

#include "dinic.h"
#include "edge.h"
#include "node.h"
#include "queue.h"

extern bool print_aux_paths;


/**
 * @brief Estructura interna de Dinic
 */
typedef struct _dinic_t {
    Network *network;
    Node *s;
    Node *t;
    Network *backwards;
    dinic_result *result;
} dinic_t;

/**
 * Funcion de creacion de un nuevo network auxiliar a partir de un 
 * network y un par de nodos origen y destino (ambos dentro de s_dinic).
 * @param s_dinic data donde esta el network y los nodos origen-destino.
 * @returns network (network auxiliar)
 */
static Network *aux_network_new(dinic_t *data) {
    Network *main_network = NULL, *backwards = NULL, *result = NULL;
    Queue *bfs_queue = NULL, *next_level = NULL;
    Node *current_node = NULL, *s = NULL, *t = NULL;
    SList *fwd_edges = NULL, *bwd_edges = NULL, *edges = NULL;
    bool is_t_found = false;
    
    assert(data != NULL);
    
/* Algoritmo Basicos:
 * 1) Revisar bfs_queue:
 *        a) Si esta vacia, terminamos -> Devolver Netwok Auxiliar (result)
 *        b) Si esta t, listo -> vaciar la cola (para terminar).
 *
 * 2) Poner aristas vecinas <- y -> en *result* y en la cola de nodos del
 *    proximo nivel segun corresponda (*Lea Dinic) por cada nodo de la cola de
 *    BFS.
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
    while (bfs_queue != NULL) {
        SList *current_edges = NULL;
        Edge *current_edge = NULL;
        Node *neighbour = NULL;
        
        /* 1 */
        if (queue_find(bfs_queue, t) != NULL) {
            /* Ya llegamos a t, tenemos nuestro network auxiliar listo,
             * liberamos la cola para terminar */
            queue_empty(bfs_queue); /* TODO */
        }
        
        /* 2 */
        while (!queue_is_empty(bfs_queue)) {
            current_node = queue_pop_head(bfs_queue);
            /* TODO: los nodos no deberian ser punterops? (network) */
            fwd_edges = network_get_edges(main_network, *current_node);
            bwd_edges = network_get_edges(backwards, *current_node);
            edges = slist_concat(fwd_edges, bwd_edges);
            /* edges tiene todas las posibles aristas del proximo nivel */
            current_edges = edges;
            
            while (current_edges != NULL) {
                current_edge = slist_head_data(current_edges);
                neighbour = edge_get_second(current_edge);
                
                /* Ver si corresponde agregar la arista al NA (result) */ 
                if (!network_has_node(result, *neighbour)) {
                    network_add_edge(result, current_edge);
                    queue_push_head(next_level, neighbour);
                    
                } else {
                    /* Solo la agregamos si pertenece al nivel siguiente */
                    if queue_find(next_level, neighbour) {
                        network_add_edge(result, current_edge);
                    }
                }
                current_edges = slist_next(current_edges);
            }
            /* Revisamos (y agregamos segun correspondia) todas las
             * arista que contenian los vecinos de current_node */
        }
        
        /* 3 */ 
        bfs_queue = next_level;
    }
    
}

/**
 * Macro que define la funcion siguiente con nombre mas corto
 */
#define aux_network_fbf aux_network_find_blocking_flow

/**
 * Funcion que busca caminos entre nodo origen y destino hasta
 * saturar todos los caminos (flujo bloqueante)
 * @note Los caminos y los flujos son almacenados en s_dinic->result.
 * @param s_dinic data donde esta el network y los nodos origen-destino.
 * @param network network auxiliar donde operar.
 * @returns network auxiliar
 */
bool aux_network_find_blocking_flow(s_dinic data*, Network *) {
    return true;
}

dinic_result *dinic(Network *network, const Node *s, const Node *t) {
    /* basicamente el ciclo que hicimos en el pizarron. */
    /*  TODO */
    return NULL;
}

