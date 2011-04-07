#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <glib/glib.h>
#include "network.h"

void destroy_glist(gpointer list);
void destroy_edge(gpointer edge);

struct s_Network {
    /* Mapea  el numero de nodo con todas las aristas forward desde el nodo */
    GHashTable *node_to_edges;
};

/* Funciones Auxiliares */
/* Destructoras */
void destroy_glist(gpointer list) {
    /* GList *to_free = (GList *) list; */

    /* g_list_free_full(to_free, destroy_edge) */
}

void destroy_edge(gpointer edge) {
    edge_destroy((Edge *) edge);
}

/* Crea un network vacio */
Network *create_network(void) {
    Network *result = NULL;

    result = calloc(1, sizeof(*result));
    if(result != NULL) {
        result->node_to_edges = g_hash_table_new_full(g_int_hash, g_int_equal,
                                                      NULL, destroy_glist);
    }

    return result;
}

void network_add_edge(Network *self, Edge *e) {
    GList *edges = NULL;
    gpointer first_node = NULL;

    /* Precondiciones */
    assert(self != NULL);
    assert(e != NULL);

    first_node = (gpointer) edge_get_first(e);

    edges = (GList *) g_hash_table_lookup(self->node_to_edges, first_node);

    if(edges == NULL) {
        /* El elemento no existia en la hash table */

        edges = g_list_append(edges, (gpointer) e);
        g_hash_table_insert(self->node_to_edges, (gpointer) first_node,
                            (gpointer) edges);
    } else {
        /* Agregamos el nuevo elemento al final de la lista */

        /* Buscamos el edge e en la lista */
        if(g_list_find(edges, e) != NULL) {
            /* Ya esta en la lista */

            /* Aumentamos el contador de referencia de e */
            edge_increment_reference(e);
        } else {
            /* Agregamos el nuevo edge a la lista */

            /* TODO: Comprobar que esto anda como suponemos, agregando un elemento
             * al final de la lista y no importa lo que devuelve. */
            edges = g_list_append(edges, (gpointer) e);

        }
    }
}

GList *network_neighbours(Network *self, Node n) {
    GList *result = NULL, *tmp = NULL;
    Edge *e = NULL;

    /* Checkeo de precondiciones */
    assert(self != NULL);

    tmp = network_get_edges(self, n);
    while (tmp != NULL) {
        /* Tomamos la cabeza de la lista */
        e = (Edge *) g_list_nth_data(tmp, 0);
        result = g_list_append(result, (gpointer) edge_get_first(e));
        tmp = g_list_next(tmp);
    }

    return result;
}

GList *network_get_edges(Network *self, Node n) {
    GList *result = NULL;

    /* Checkeo de precondiciones */
    assert(self != NULL);

    result = g_hash_table_lookup(self->node_to_edges, (gpointer)&n);

    return result;
}

void network_destroy(Network *self) {
    assert(self != NULL);
    /*TODO*/

}

