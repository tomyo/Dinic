#ifndef NETWORK_H
#define NETWORK_H

#include <stdio.h>
#include <glib-2.0/glib.h>
#include "edge.h"

typedef struct s_Network Network;

/* Crea un network vacio */
Network *network_create(void);

/* Agregar arista al network */
void network_add_edge(Network *self, Edge *e);

/* Devuelve la lista con punteros a los vecinos del nodo n
 * La lista devuelta por esta funcion debe ser liberada
 * por el que la llame */
GSList *network_neighbours(Network *self, Node n);

/* Devuelve todas las aristas que salen desde n */
GSList *network_get_edges(Network *self, Node n);

/* Imprime un network en f*/
void network_pretty_print(Network *self, FILE *f);

/* Destruye un network */
void network_destroy(Network *self);

#endif

