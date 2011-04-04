#ifndef EDGE_H
#define EDGE_H

typedef struct s_Edge *Edge;
typedef int Weight

/*Crea un edge desde x1 a x2 con peso w*/
Edge edge_create(Node x1, Node x2, Weight w);

/*Devuelve el peso de una arista*/
Weight edge_get_weight(Edge)

/*Devuelve el primer nodo de una arista*/
Node edge_get_first(Edge)

/*Devuelve el segundo nodo de una arista*/
Node edge_get_second(Edge)

#endif /*EDGE_H*/
