#ifndef DINIC_H
#define DINIC_H

#include <stdbool.h>

#include "slist.h"
#include "network.h"

typedef s_dinic_result dinic_result;

/**
 * @struct s_dinic_result "dinic.h"
 * @brief Paquete para devolver los resultados de dinic
 */
struct s_dinic_result {
    /**
     * @brief valor del flujo maximal obtenido
     */ 
    unsigned int flow_value;
    
    /**
     * @brief Lista con los valores de los caminos obtenidos en cada N.A.
     * Formato: {(Node, ->), (Node, ->), ... , (Node, <-), ... , Flujo} 
     */
    SList *aux_flows;
    
    /**
     * @brief Lista 
     * Formato: {Node, Node, ...}
     */
    SList *min_cut;
    
}   

/**
 * @brief Funcion que corre el algoritmo de Dinic en un Network.
 * @param Network network a partir del cual operar.
 * @param Node node origen (en network)  (s)
 * @param Node node destino (en network) (t)
 * @returns DinicResult
 */
dinic_result *dinic(Network *, const Node *, const Node *); 
 

#endif 
