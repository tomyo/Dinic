#ifndef NETWORK_H
#define NETWORK_H

/**
 * @file network.h
 * @brief Define como va a ser representado un network, junto
 * con las operaciones que posee.
 */

#include <stdio.h>
#include "slist.h"
#include "edge.h"

/** El tipo Network se utiliza como un puntero a una estructura que no es
 *  accesible de forma directa, sino a traves de las funciones de este tipo. */
typedef struct s_Network Network;

/**
 * @brief Crea un network vacio.
 * @returns Puntero al nuevo network creado.
 */
Network *network_create(void);

/**
 * @brief Agrega una arista al network.
 *
 * No se debe agregar una arista ya agregada.
 * @param self network donde ejecutar la operacion.
 * @param e la arista a agregar.
 */
void network_add_edge(Network *self, Edge *e);

/**
 * @brief Agrega una arista al network en sentido backward.
 *
 * No se debe agregar una arista ya agregada.
 * @param self network donde ejecutar la operacion.
 * @param e la arista a agregar.
 */
void network_add_edge_backward(Network *self, Edge *e);

/**
 * @brief Devuelve la lista con punteros a los vecinos del nodo n.
 * @note La lista devuelta por esta funcion debe ser liberada
 * por el que la llame.
 * @param self network donde ejecutar la operacion.
 * @param n nodo al que se le van a buscar los vecinos.
 * @returns Lista de nodos vecinos.
 */
SList *network_neighbours(Network *self, const Node n);


/**
 * @brief Devuelve una lista con todas las aristas que salen desde n.
 * @warning Esta lista no debe ser liberada.
 * @param self network donde ejecutar la operacion.
 * @param n nodo del que se van a buscar las aristas que salen de el.
 * @return Lista de edges que salen de n.
 */
SList *network_get_edges(Network *self, const Node n);

/** Devuelve una lista con todos los nodos.
 *  Esta lista debe ser liberada con slist_free. */
SList *network_get_nodes(Network *self);

/**
 * @brief Pregunta si esta un nodo en el network.
 * @param self network donde ejecutar la operacion.
 * @returns True si el nodo pertenece a los nodos.
 * @returns False en caso contrario.
 */
bool network_has_node(Network *self, const Node n);

/**
 * @brief Imprime un network en el archivo pasado.
 * @param self network donde ejecutar la operacion.
 * @param f file descriptor donde escribir el network.
 */
void network_pretty_print(Network *self, FILE *f);

/**
 * @brief Destruye el network y sus aristas, en caso de queres conservarlas,
 * utilizar network_free.
 * @param self network donde ejecutar la operacion.
 */
void network_destroy(Network *self);

/**
 * @brief Libera la estructura del network, sin eliminar aristas, en caso de
 * queres liberar todo, usar network_destroy.
 * @param self network donde ejecutar la operacion.
 */
void network_free(Network *self);

/**
 * @brief Elimina una arista del network.
 *
 * La arista debe pertenecer al network.
 * @note Se devuelve la arista para quien llame la funcion la libere o no.
 * @param self network donde ejecutar la operacion.
 * @param e edge para borrar.
 * @returns Arista borrada del network.
 */
Edge *network_del_edge(Network *self, Edge *e);


/**
 * @brief Elimina una arista del network agregada backward.
 *
 * La arista debe pertenecer al network en sentido backward.
 * @note Se devuelve la arista para quien llame la funcion la libere o no.
 * @param self network donde ejecutar la operacion.
 * @param e edge para borrar.
 * @returns Arista borrada del network.
 */
Edge *network_del_edge_backward(Network *self, Edge *e);

/**
 * @brief Actualiza un camino del network con el flow pasado.
 * @pre path es un camino aumentante.
 * @pos El network no tiene edges backward si estos no pueden devolver flujo.
 * @param self network donde ejecutar la operacion.
 * @param path camino de edges para actualidar el flujo.
 * @param flow valor con que actualizar las aristas. Este valor es del
 * incremento (o decremento) del flow que ya tienen las aristas.
 */
void network_update(Network *self, SList *path, Flow flow);

void _network_add_edge(Network *network, Edge *edge, char mode);

Edge *_network_del_edge(Network *network, Edge *edge, char mode);

#endif /* NETWORK_H */
