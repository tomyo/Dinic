#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "network.h"
#include "hashtable/hashtable.h"


int compare_edges(gconstpointer a, gconstpointer b);
void destroy_glist(gpointer list);
void destroy_edge(gpointer edge);

/* Hash Table redefines */
#define KEY_SIZE sizeof(int)
#define VALUE_SIZE sizeof(int)

/**
 * \brief
 * Tabla Hash (node_to_edges) con el network
 *
 */
struct s_Network {
    /* Mapea  el numero de nodo con todas las aristas forward desde el nodo */
    hash_table_t *node_to_edges;
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
    GSList *to_free = (GSList *) list, *tmp;
    gpointer edge = NULL;

    /* Precondicion */
    assert(to_free != NULL);

    tmp = to_free;

    while (tmp != NULL) {
        edge = g_slist_nth_data (tmp, 0);
        edge_destroy(edge);
        tmp = g_slist_next(tmp);
    }

    g_slist_free(to_free);
}

void destroy_edge(gpointer edge) {
    edge_destroy((Edge *) edge);
}

void g_slist_pprint(GSList *l) {
    int len, i;
    gpointer value = NULL;
    len = g_slist_length(l);
    for (i = 0; i < len; i++) {
        value = g_slist_nth_data(l, i);
        edge_pprint((Edge *)value);
    }
}


/* Crea un network vacio */
Network *network_create(void) {
    Network *result = NULL;

    result = calloc(1, sizeof(*result));
    if(result != NULL) {
        result->node_to_edges = ht_new_full(MODE_ALLREF, NULL, destroy_glist);
    }

    return result;
}

void network_add_edge(Network *self, Edge *e) {
    GSList *edges = NULL;
    gpointer first_node = NULL;

    /* Precondiciones */
    assert(self != NULL);
    assert(e != NULL);

    first_node = edge_get_first(e);
    edges = ht_lookup(self->node_to_edges, first_node);

    if(edges == NULL) {
        /* El elemento no existia en la hash table */

        edges = g_slist_append(edges, (gpointer) e);
        ht_insert(self->node_to_edges, first_node, edges);

    } else if(g_slist_find_custom(edges, e, compare_edges) == NULL) {
        /* El elemento no esta en la lista, lo agregamos */

        /* Hacemos un prepend, pues es O(1), append es O(n)*/
        edges = g_slist_prepend(edges, (gpointer) e);
        /* Sacamos la lista vieja del hash, sin liberarla */
        ht_steal(self->node_to_edges, first_node);
        /* Agregamos la nueva lista */
        ht_insert(self->node_to_edges, first_node, edges);
    } else {
        /* Arista repetida.
         * Le avisamos al usuario, liberamos la arista y continuamos con la
         * ejecucion
         */
        fprintf(stderr, "Arista Repetida\n");
        edge_destroy(e);
    }
}


GSList *network_neighbours(Network *self, Node n) {
    GSList *result = NULL, *tmp = NULL;
    Edge *e = NULL;

    /* Checkeo de precondiciones */
    assert(self != NULL);

    tmp = network_get_edges(self, n);
    while (tmp != NULL) {
        /* Tomamos la cabeza de la lista */
        e = (Edge *) g_slist_nth_data(tmp, 0);
        result = g_slist_append(result, (gpointer) edge_get_second(e));
        tmp = g_slist_next(tmp);
    }

    return result;
}

GSList *network_get_edges(Network *self, Node node) {
    GSList *result = NULL;

    /* Checkeo de precondiciones */
    assert(self != NULL);

    result = ht_lookup(self->node_to_edges, &node);

    return result;
}


void network_destroy(Network *self) {
    assert(self != NULL);
    ht_destroy(self->node_to_edges);
    free(self); self = NULL;
}

#undef VALUE_SIZE
#undef KEY_SIZE
