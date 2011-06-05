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
     * Formato: {Nodo, Nodo, Flujo, Capacidad}
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

/* *********** TEMPORALMENTE MOVIDO ACA ESTO VA EN EL .h ************ */

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

    /** Un network que va a contener los lados backwards */
    Network *backwards;

    /** La estructura que devuelve el resultado */
    dinic_result *result;
} dinic_t;

Network *aux_network_new(dinic_t *);

/* *********** TEMPORALMENTE MOVIDO ACA ESTO VA EN EL .h ************ */


#endif
