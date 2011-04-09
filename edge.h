#ifndef EDGE_H
#define EDGE_H

/**
 * @file edge.h
 * Modulo donde se define la estructura de una arista, basicamente
 * va a ser una 3-upla que contiene 2 nodos y la capacidad de la arista.
 */

#include <stdbool.h>
#include "node.h"

/** El tipo Edge se utiliza como un puntero a una estructura que no es
 *  accesible de forma directa, sino a traves de las funciones de este tipo. */
typedef struct s_Edge Edge;

/** La capacidad es representada por un entero sin signo */
typedef unsigned int Weight;

/** Crea una arista desde el nodo x1 hacia el nodo x2 con capacidad w */
Edge *edge_create(Node x1, Node x2, Weight w);

/** Devuelve el peso de una arista */
Weight edge_get_weight(Edge *self);

/** Devuelve un puntero al primer nodo de una arista */
Node *edge_get_first(Edge *self);

/** Devuelve un puntero al segundo nodo de una arista */
Node *edge_get_second(Edge *self);

/**
 * Funcion para comparar dos aristas, o sea para saber si
 * dos aristas son iguales.
 *
 * Decimos que dos aristas de la forma xy, zw son iguales
 * si x == z e y == w. i.e son iguales si los nodos
 * son iguales, sin importar el peso.
 */
bool edge_cmp(const Edge *e1, const Edge *e2);

/** Imprime por pantalla una arista */
void edge_pprint(const Edge *self);

/** Libera una arista */
void edge_destroy(Edge *self);

#endif /* EDGE_H */
