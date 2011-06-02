#include "dinic.h"
#include "edge.h"
#include "node.h"

/**
 * @brief Estructura interna de Dinic
 *
 */
struct s_dinic {
    Network *network;
    Node *s;
    Node *t;
    hash_table_t *backwards;
    DinicResult *result;

}

dinic_result *dinic(const Network network, const Node *s, const Node *t) {
    /* TODO: basicamente el ciclo que hicimos en el pizarron. */
    return s_dinic->result;
}

/**
 * Funcion de creacion de un nuevo network auxiliar a partir de un 
 * network y un par de nodos origen y destino (ambos dentro de s_dinic).
 * @param s_dinic data donde esta el network y los nodos origen-destino.
 * @returns network (network auxiliar)
 */
Network *aux_network_new(s_dinic *data);

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


