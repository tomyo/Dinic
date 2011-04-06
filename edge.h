#ifndef EDGE_H
#define EDGE_H

typedef struct s_Edge Edge;
typedef unsigned int Weight

/* Crea un edge desde x1 a x2 con peso w */
Edge *edge_create(Node x1, Node x2, Weight w);

/* Devuelve el peso de una arista */
Weight edge_get_weight(Edge *self);

/* Devuelve un puntero al primer nodo de una arista */
Node *edge_get_first(Edge *self);

/* Devuelve un puntero al segundo nodo de una arista */
Node *edge_get_second(Edge *self);

/* Indica que self es ahora referenciado por alguien mas. 
 * Esta funcion debe ser llamada cada vez que self se guarda     
 * en alguna estructura de datos, y vamos a querer (mas          
 * adelante) llamar a edge_destroy(self) dentro de esa estructura
 */
void edge_increment_reference(Edge *self);

/* Libera una arista */
void edge_destroy(Edge *self);

#endif /*EDGE_H*/
