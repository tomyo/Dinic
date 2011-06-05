#ifndef EDGE_H
#define EDGE_H

/**
 * @file edge.h
 * @brief Modulo donde se define la estructura de una arista.
 *
 * Basicamente va a ser una 4-upla que contiene 2 nodos, la
 * capacidad de la arista y el flujo actual.
 */

#include <stdbool.h>
#include "node.h"

/** La capacidad es representada por un entero sin signo */
typedef unsigned int Capacity;

/** El flujo es representado por un entero sin signo */
typedef unsigned int Flow;

/** El tipo Edge se utiliza como un puntero a una estructura que no es
 *  accesible de forma directa, sino a traves de las funciones de este tipo. */
typedef struct s_Edge Edge;

/** Crea una arista desde el nodo x1 hacia el nodo x2 con capacidad w y flujo
 *  inicial f*/
Edge *edge_create(Node x1, Node x2, Capacity c, Flow f);

/** Devuelve la capacidad de una arista */
Capacity edge_get_capacity(Edge *self);

/** Devuelve el flujo de una arista */
Flow edge_get_flow(Edge *self);

/** Devuelve un puntero al primer nodo de una arista */
Node *edge_get_first(Edge *self);

/** Devuelve un puntero al segundo nodo de una arista */
Node *edge_get_second(Edge *self);

/**
 * Funcion para comparar dos aristas, o sea para saber si
 * dos aristas son iguales.
 *
 * @note Decimos que dos aristas de la forma xy, zw son iguales
 * si x == z e y == w. i.e son iguales si los nodos
 * son iguales, sin importar el peso.
 */
bool edge_cmp(const Edge *e1, const Edge *e2);

/** Imprime por pantalla una arista */
void edge_pprint(const Edge *self);

/** Libera una arista */
void edge_destroy(Edge *self);

/** Modifica el valor del flujo de la arista por el valor de new_flow */
void edge_update_flow(Edge *self, unsigned int new_flow);

#endif /* EDGE_H */

