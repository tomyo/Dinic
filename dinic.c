#include "dinic.h"
#include "edge.h"
#include "node.h"

extern mode;
/**
 * @brief Estructura interna de Dinic
 *
 */
struct s_dinic {
    Network *network;
    Node *s;
    Node *t;
    Network *backwards;
    DinicResult *result;
}

dinic_result *dinic(Network *network, const Node *s, const Node *t) {
    /* basicamente el ciclo que hicimos en el pizarron. */
    
    return s_dinic->result;
}

/**
 * Funcion de creacion de un nuevo network auxiliar a partir de un 
 * network y un par de nodos origen y destino (ambos dentro de s_dinic).
 * @param s_dinic data donde esta el network y los nodos origen-destino.
 * @returns network (network auxiliar)
 */
Network *aux_network_new(s_dinic *data) {
    Network *result = NULL;
    
    Queue *bfs_queue = NULL;
    Edge *node_pivot = NULL, *edge_pivot = NULL;
    
    Node *tmp = NULL;
    SList *fwd_edges = NULL, *bwd_edges = NULL, *edges = NULL;
    SList *iter = NULL;
    SList *nodes_in_next_level = NULL; /* Aristas agregadas al siguiente nivel */
    bool is_t_found = false;
    
    assert(data != NULL);
    
    /* Pasos Basicos:
     * Pararte en s
     * Poner vecinos <- y -> en cola sy network (que no haya agregado antes)
     * Repetir hasta llegar a t o terminar con corte
     * Devolver network
     */
     
    result = network_create();
    bfs_queue = queue_new();
    node_pivot = data->s;
    queue_push_head(bfs_queue, node_pivot);
    
    while ((node_pivot != data->t) || (queue_is_empty(bfs_queue))) {
        fwd_edges = network_get_edges(data->network, node_pivot);
        bwd_edges = network_get_edges(data->backwards, node_pivot);
        edges = slist_concat(fwd_edges, bwd_edges);
        /* edges tiene todas las posibles aristas del nivel i-esimo */

        iter = edges;
        while (iter != NULL && !is_t_found) {
            edge_pivot = slist_head_data(iter);
            if (network_lookup(result, node_pivot) == NULL) {
                network_add_edge(result, edge_pivot);
                slist_prepend(nodes_in_next_level, node_pivot);
            } else {
                if slist_find(nodes_in_next_level, node_pivot) {
                    /* Si ya esta, pero en el nivel i+1, igual se agrega */
                    network_add_edge(result, edge_pivot);
                    slist_prepend(nodes_in_next_level, node_pivot);
                }
            }
        }
        slist_free(nodes_in_next_level);
    
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
bool aux_network_find_blocking_flow(s_dinic data*, Network *);


