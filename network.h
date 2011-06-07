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
 * @returns NULL si no hay memoria suficiente.
 */
Network *network_create(void);


/**
 * @brief Idem network_create pero su funcion destructora no toca
 * las aristas, que usualmente pertencen a un network ordinario.
 * @returns Puntero al nuevo network creado.
 * @returns NULL si no hay memoria suficiente.
 */
Network *network_aux_create(void);

/**
 * @brief Agrega una arista al network.
 *
 * @warning No se debe agregar una arista ya agregada.
 * @see _network_add_edge
 * @param self network donde ejecutar la operacion.
 * @param e la arista a agregar.
 */
void network_add_edge(Network *self, Edge *e);

/**
 * @brief Agrega una arista al network en sentido backward.
 * @warning No se debe agregar una arista ya agregada.
 * @see _network_add_edge
 * @param self network donde ejecutar la operacion.
 * @param e la arista a agregar.
 */
void network_add_edge_backward(Network *self, Edge *e);

/**
 * @brief Agrega una arista al network segun el modo pasado.
 * @warning No se debe agregar una arista ya agregada.
 * @see network_add_edge
 * @see network_add_edge_backward
 * @param self network donde ejecutar la operacion.
 * @param e la arista a agregar.
 * @param mode 'f' si se agrega como fordward, 'b' en caso contrario.
 */
void _network_add_edge(Network *network, Edge *edge, char mode);

/**
 * @brief Elimina una arista del network.
 * @note La arista debe pertenecer al network.
 * @note Se devuelve la arista para quien llame la funcion la libere o no.
 * @see _network_del_edge
 * @param self network donde ejecutar la operacion.
 * @param e edge para borrar.
 * @returns Arista borrada del network.
 */
Edge *network_del_edge(Network *self, Edge *e);

/**
 * @brief Elimina una arista del network agregada backward.
 * @note La arista debe pertenecer al network en sentido backward.
 * @note Se devuelve la arista para quien llame la funcion la libere o no.
 * @see _network_del_edge
 * @param self network donde ejecutar la operacion.
 * @param e edge para borrar.
 * @returns Arista borrada del network.
 */
Edge *network_del_edge_backward(Network *self, Edge *e);

/**
 * @brief Elimina una arista del network agregada segun el modo pasado.
 * @note La arista debe pertenecer al network en la direccion pasada.
 * @note Se devuelve la arista para quien llame la funcion la libere o no.
 * @see network_del_edge
 * @see network_del_edge_backward
 * @param self network donde ejecutar la operacion.
 * @param e edge para borrar.
 * @param mode 'f' si la arista fue agregada forward, 'b' en caso contrario.
 * @returns Arista borrada del network.
 */
Edge *_network_del_edge(Network *network, Edge *edge, char mode);

/**
 * @brief Devuelve una lista con todas las aristas que salen desde n.
 *
 * Las aristas devueltas son las agregadas por forward (siendo n de donde
 * salea la arista) y las agregadas backwards (siendo n a donde llega la
 * arista).
 * @note Esta lista debe ser liberada por quien la llame.
 * No asi sus elementos.
 * @warning Esta lista no debe ser liberada.
 * @param self network donde ejecutar la operacion.
 * @param n nodo del que se van a buscar las aristas que salen de el.
 * @return Lista de edges que salen de n.
 */
SList *network_get_edges(Network *self, const Node n);

/**
 * @brief Devuelve la lista con punteros a los vecinos del nodo n.
 * @note Esta lista debe ser liberada por quien la llame.
 * No asi sus elementos.
 * @param self network donde ejecutar la operacion.
 * @param n nodo al que se le van a buscar los vecinos.
 * @returns Lista de nodos vecinos.
 */
SList *network_neighbours(Network *self, const Node n);

SList *network_forward_edges(Network *self);

/**
 * @brief Pregunta si esta un nodo en el network.
 * @param self network donde ejecutar la operacion.
 * @returns True si el nodo pertenece a los nodos.
 * @returns False en caso contrario.
 */
bool network_has_node(Network *self, const Node n);

/**
 * @brief Actualiza un camino del network con el flow pasado.
 * @pre path es un camino aumentante.
 * @post El network no tiene edges backward si estos no pueden devolver flujo.
 * @param self network donde ejecutar la operacion.
 * @param path camino de edges para actualidar el flujo.
 * @param flow valor con que actualizar las aristas. Este valor es del
 * incremento (o decremento) del flow que ya tienen las aristas.
 */
void network_update(Network *self, SList *path, Flow flow);

/**
 * @brief Devuelve una lista con todos los nodos del network.
 * @note Esta lista debe ser liberada por quien la llame.
 * No asi sus elementos.
 * @param self network donde ejecutar la operacion.
 */
SList *network_nodes(Network *self);

/**
 * @brief Devuelve una lista de todas las aristas forward del network.
 * @note Esta lista debe ser liberada por quien la llame.
 * No asi sus elementos.
 * @param self network donde ejecutar la operacion.
 */
SList *network_forward_edges(Network *self);

/**
 * @brief Imprime un network en el archivo pasado.
 * @param self network donde ejecutar la operacion.
 * @param f file descriptor donde escribir el network.
 */
void network_pretty_print(Network *self, FILE *f);

/**
 * @brief Destruye el network y sus aristas.
 * @see network_free
 * @param self network donde ejecutar la operacion.
 */
void network_destroy(Network *self);

/**
 * @brief Libera la estructura del network, sin eliminar aristas.
 * @see network_destroy
 * @param self network donde ejecutar la operacion.
 */
void network_free(Network *self);

#endif /* NETWORK_H */
