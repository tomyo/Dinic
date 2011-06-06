#ifndef DINIC_H
#define DINIC_H

#include <stdbool.h>

#include "slist.h"
#include "network.h"
#include "node.h"

/**
 * @struct s_dinic_result "dinic.h"
 * @brief Paquete para devolver los resultados de dinic
 */
typedef struct s_dinic_result {
    /**
     * @brief valor del flujo maximal obtenido
     */
    unsigned int flow_value;

    /**
     * @brief Lista con los valores de los flujos obtenidos.
     * Formato: {Edge, Edge, .... }
     */
    SList *max_flow;

    /**
     * @brief Lista
     * Formato: {Node, Node, ...}
     */
    SList *min_cut;

} dinic_result;

/**
 * @brief Funcion que corre el algoritmo de Dinic en un Network.
 * @param Network network a partir del cual operar.
 * @param Node node origen (en network)  (s)
 * @param Node node destino (en network) (t)
 * @param Booleano para hacer el resultado mas verbose
 * @returns DinicResult
 */
dinic_result *dinic(Network * n, Node s, Node t, bool);

/* *********** TEMPORALMENTE MOVIDO ACA ESTO VA EN EL .c ************ */

/**
 * @brief Estructura interna de Dinic
 */
typedef struct {
    /** El network original que va a ir cambiando durante el algoritmo */
    Network *network;

    /** Nodo origen */
    Node s;

    /** Nodo destino */
    Node t;

} dinic_t;

/**
 * @brief Estructura que contiene un flujo (camino y valor).
 */
typedef struct {
    /** Lista de edges que forman el flujo. Tiene la forma [(a, b), (b, c)..] */
    SList *path;

    /** Valor del flujo */
    Flow flow_value;
} DinicFlow;

Network *aux_network_new(dinic_t *);
DinicFlow *aux_network_find_flow(dinic_t *, Network *, bool );

/* *********** TEMPORALMENTE MOVIDO ACA ESTO VA EN EL .c ************ */


#endif
