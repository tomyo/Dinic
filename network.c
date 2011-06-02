#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "network.h"
#include "slist.h"
#include "hashtable/hashtable.h"


int compare_edges(const void *a, const void *b);
void destroy_slist(void *list);
void destroy_edge(void *edge);

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
int compare_edges(const void *a, const void *b) {
    int result = 0;

    if(edge_cmp(a, b)) {
        result = 0;
    } else {
        result = 1;
    }

    return result;
}

/*----- Destructoras */
void destroy_slist(void *list) {
    SList *to_free = (SList *) list, *tmp;
    void *edge = NULL;

    /* Precondicion */
    assert(to_free != NULL);

    tmp = to_free;

    while (tmp != NULL) {
        edge = slist_nth_data (tmp, 0);
        edge_destroy(edge);
        tmp = slist_next(tmp);
    }

    slist_free(to_free);
}

void destroy_edge(void *edge) {
    edge_destroy((Edge *) edge);
}

/* Crea un network vacio */
Network *network_create(void) {
    Network *result = NULL;

    result = calloc(1, sizeof(*result));
    if(result != NULL) {
        result->node_to_edges = ht_new_full(MODE_ALLREF, NULL, destroy_slist);
    }

    return result;
}

void network_add_edge(Network *self, Edge *e) {
    SList *edges = NULL;
    void *first_node = NULL;

    /* Precondiciones */
    assert(self != NULL);
    assert(e != NULL);

    first_node = edge_get_first(e);
    edges = ht_lookup(self->node_to_edges, first_node);

    if(edges == NULL) {
        /* El elemento no existia en la hash table */

        edges = slist_append(edges, (void *) e);
        ht_insert(self->node_to_edges, first_node, edges);

    } else if(slist_find_custom(edges, e, compare_edges) == NULL) {
        /* El elemento no esta en la lista, lo agregamos */

        /* Hacemos un prepend, pues es O(1), append es O(n)*/
        edges = slist_prepend(edges, (void *) e);
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


SList *network_neighbours(Network *self, Node n) {
    SList *result = NULL, *tmp = NULL;
    Edge *e = NULL;

    /* Checkeo de precondiciones */
    assert(self != NULL);

    tmp = network_get_edges(self, n);
    while (tmp != NULL) {
        /* Tomamos la cabeza de la lista */
        e = (Edge *) slist_nth_data(tmp, 0);
        result = slist_append(result, (void *) edge_get_second(e));
        tmp = slist_next(tmp);
    }

    return result;
}

SList *network_get_edges(Network *self, Node node) {
    SList *result = NULL;

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
