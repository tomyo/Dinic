#ifndef DINIC_H
#define DINIC_H

#include <stdbool.h>

#include "slist.h"
#include "network.h"
#include "node.h"


/**
 * @file dinic.h
 * @brief Operaciones exportadas para el algoritmo dinic.
 *
 * Define la interfaz para correr el algortimo.
 */

/**
 * @brief Estructura para almacenar variables de Dinic.
 */
typedef struct {
    /**
     * @brief El network original pasado al algoritmo.
     */
    Network *network;

    /**
     * @brief Nodo origen
     */
    Node *s;

    /**
     * @brief Nodo destino
     */
    Node *t;

} dinic_t;

/**
 * @brief Estructura que contiene un flujo parcial.
 */
typedef struct {
    /**
     * @brief Lista de edges que forman el flujo.
     *
     * Formato: {Edge, Edge, ...}
     */
    SList *path;

    /**
     * @brief Valor del flujo.
     */
    Flow flow_value;
} DinicFlow;



/**
 * @brief Paquete para devolver los resultados de dinic
 */
typedef struct {
    /**
     * @brief Valor del flujo maximal obtenido
     */
    unsigned int flow_value;

    /**
     * @brief Lista con los valores de los flujos obtenidos.
     *
     * Formato: {Edge, Edge, .... }
     */
    SList *max_flow;

    /**
     * @brief Lista con el corte minimal
     *
     * Formato: {Node, Node, ...}
     */
    SList *min_cut;

} dinic_result;


/**
 * @brief Funcion que corre el algoritmo de Dinic en un Network.
 * @warning El resultado contiene punteros a las aristas del network pasado.
 * @param n network a partir del cual operar.
 * @param s Nodo origen (en network).
 * @param t Nodo destino (en network).
 * @param verbose imprime los flujos que se mandan en cada network auxiliar.
 * @returns dinic_result Con el resultado del algoritmo.
 */
dinic_result *dinic(Network *n, Node *s, Node *t, bool verbose);


/**
 * @brief Devuelve el network auxiliar.
 * @note La creacion del network auxiliar se basa en los flujos pasados.
 * @warning El network devuelto tiene punteros a las aristas del network
 * pasado.
 * @param data dinit_t con los parametros necesarios para la funcion
 * @returns Network con el network auxiliar.
 */
Network *aux_network_new(dinic_t *data);

/**
 * @brief Funcion que busca caminos entre nodo origen y destino.
 * @note Los caminos y los flujos son almacenados en s_dinic->result.
 * @warning El path devuelto tiene punteros a las aristas del network pasado.
 * @param data donde esta el network y los nodos origen-destino.
 * @param network network auxiliar donde operar.
 * @param verbose si va a imprimir la salida.
 * @returns DinicFlow con el camino encontrado y su flujo.
 */
DinicFlow *aux_network_find_flow(dinic_t *data, Network *network, bool verbose);

#endif
