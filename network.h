#ifndef NETWORK_H
#define NETWORK_H

/**
 * @file network.h
 * \brief Define como va a ser representado un network, junto
 * con las operaciones que posee
 */

#include <stdio.h>
#include "slist.h"
#include "edge.h"

/** El tipo Network se utiliza como un puntero a una estructura que no es
 *  accesible de forma directa, sino a traves de las funciones de este tipo. */
typedef struct s_Network Network;

/** Crea un network vacio */
Network *network_create(void);

/** Agregar arista al network */
void network_add_edge(Network *self, Edge *e);

/** Devuelve la lista con punteros a los vecinos del nodo n
 *  La lista devuelta por esta funcion debe ser liberada
 *  por el que la llame */
SList *network_neighbours(Network *self, const Node *n);

/** Devuelve una lista con todas las aristas que salen desde n.
 *  Esta lista no debe ser liberada. */
SList *network_get_edges(Network *self, const Node *n);

/**
 * Pregunta por la precencia de un nodo en el netwok.
 * @returns True si el nodo pertenece a los nodos, False c.c */
bool network_has_node(Network *self, const Node *n); 

/** Imprime un network en el archivo f */
void network_pretty_print(Network *self, FILE *f);

/** Destruye un network */
void network_destroy(Network *self);

#endif

