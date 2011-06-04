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
     * @brief Lista con los valores de los flujos obtenidos.
     * Formato: {Nodo, Nodo, Flujo, Capacidad}
     */
    Slist *max_flow;
    
    /**
     * @brief valor del flujo maximal obtenido
     */
    unsigned int flow_value;
    
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
 * @returns DinicResult
 */
dinic_result *dinic(Network * n, const Node * s, const Node * t);


#endif
