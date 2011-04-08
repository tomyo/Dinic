#ifndef EDGE_H
#define EDGE_H

#include <stdbool.h>
#include "node.h"

typedef struct s_Edge Edge;
typedef unsigned int Weight;

/* Crea un edge desde x1 a x2 con peso w */
Edge *edge_create(Node x1, Node x2, Weight w);

/* Devuelve el peso de una arista */
Weight edge_get_weight(Edge *self);

/* Devuelve un puntero al primer nodo de una arista */
Node *edge_get_first(Edge *self);

/* Devuelve un puntero al segundo nodo de una arista */
Node *edge_get_second(Edge *self);

/* Decimos que dos aristas de la forma xy, zw son iguales
 * si x == z e y == w. Es decir son iguales si los nodos
 * son iguales, sin importar el peso.
 */
bool edge_cmp(const Edge *e1, const Edge *e2);

/* Imprime por pantalla un edge */
void edge_pprint(const Edge *self);

/* Libera una arista */
void edge_destroy(Edge *self);

#endif /*EDGE_H*/
