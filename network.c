#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <glib/glib.h>
#include "network.h"

int compare_edges(gconstpointer a, gconstpointer b);
void destroy_glist(gpointer list);
void destroy_edge(gpointer edge);

struct s_Network {
    /* Mapea  el numero de nodo con todas las aristas forward desde el nodo */
    GHashTable *node_to_edges;
};

/*--------------- Funciones Auxiliares */
/*----- De Comparacion */
int compare_edges(gconstpointer a, gconstpointer b) {
    int result = 0;

    if(edge_cmp(a, b)) {
        result = 0;
    } else {
        result = 1;
    }

    return result;
}

/*----- Destructoras */
void destroy_glist(gpointer list) {
    GList *to_free = (GList *) list, *tmp;
    gpointer edge = NULL;

    /* Precondicion */
    assert(to_free != NULL);

    tmp = to_free;

    while (tmp != NULL) {
        edge = g_list_nth_data (tmp, 0);
        edge_destroy(edge);
        tmp = g_list_next(tmp);
    }

    g_list_free(to_free);
}

void destroy_edge(gpointer edge) {
    edge_destroy((Edge *) edge);
}


/* Crea un network vacio */
Network *network_create(void) {
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
    gboolean must_be_true = TRUE;
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
    } else if(g_list_find_custom(edges, e, compare_edges) == NULL) {
        /* El elemento no esta en la lista */
        /* Agregamos el nuevo edge a la lista */

        /* Hacemos un prepend, pues es O(1), append es O(n)*/
        edges = g_list_prepend(edges, (gpointer) e);
        /* Reinsertamos en la hash */
        /* ---- Primero retiremos el valor anterior */
        must_be_true = g_hash_table_steal(self->node_to_edges, 
                                          (gpointer) first_node);
        assert(must_be_true == TRUE);
        /* ----- Insertamos el elemento modificado */
        g_hash_table_insert(self->node_to_edges, (gpointer) first_node,
                            (gpointer) edges);
    } else {
        /* Arista repetida.
         * Le avisamos al usuario, liberamos la arista y continuamos con la
         * ejecucion
         */
        fprintf(stderr, "Arista Repetida\n");
        edge_destroy(e);
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
        result = g_list_append(result, (gpointer) edge_get_second(e));
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

    g_hash_table_destroy(self->node_to_edges);
    free(self); self = NULL;
}
