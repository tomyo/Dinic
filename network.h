#ifndef NETWORK_H
#define NETWORK_H

#include <stdio.h>
#include <glib/glib.h>
#include "edge.h"

typedef struct s_Network Network;

/* Crea un network vacio */
Network *create_network(void);

/* Agregar arista al network */
void network_add_edge(Network *self, Edge *e);

/* Devuelve la lista con punteros a los vecinos del nodo n */
GList *network_neighbours(Network *self, Node n);

/* Devuelve todas las aristas que salen desde n */
GList *network_get_edges(Network *self, Node n);

/* Imprime un network en f*/
void network_pretty_print(Network *self, FILE *f);

/* Destruye un network */
void network_destroy(Network *self);

#endif

