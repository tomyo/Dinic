#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "network.h"
#include "slist.h"
#include "defs.h"
#include "hashtable/hashtable.h"


static void destroy_slist(void *list);

/**
 * \brief
 * Tabla Hash (node_to_edges) con el network
 *
 */
struct s_Network {
    /* Mapea  el numero de nodo con todas las aristas forward desde el nodo */
    hash_table_t *node_to_edges;
};

/*------------------------ Funciones Auxiliares ---------------------------- */

static void destroy_slist(void *list) {
    SList *to_free = (SList *) list, *tmp;
    void *edge = NULL;

    /* Precondicion */
    assert(to_free != NULL);

    /* Liberando el reference counter */
    free(slist_head_data(list));
    tmp = slist_next(to_free);

    /* Liberando los edges */
    while (tmp != NULL) {
        edge = slist_head_data(tmp);
        edge_destroy(edge);
        tmp = slist_next(tmp);
    }

    /* Liberando estructura de listas */
    slist_free(to_free);
}

/*--------------------------- Funciones del ADT ----------------------------*/

Network *network_create(void) {
    Network *result = NULL;

    result = calloc(1, sizeof(*result));
    if(result != NULL) {
        result->node_to_edges = ht_new_full(MODE_ALLREF, NULL, destroy_slist);
    }

    return result;
}

void network_add_edge(Network *network, Edge *edge) {
    Node *x1 = NULL, *x2 = NULL;
    SList *neighbours_x1 = NULL, *neighbours_x2 = NULL;

    x1 = edge_get_first(edge);
    x2 = edge_get_second(edge);

    neighbours_x1 = ht_lookup(network->node_to_edges, x1);
    neighbours_x2 = ht_lookup(network->node_to_edges, x2);

    /* Yo se, que en la primera posicion de la lista deberia ir el Ref Coun */
    if (neighbours_x1 == NULL) {
        /* No estaba en la Hash */
        unsigned int *reference_counter_x1 = NULL;
        reference_counter_x1 = (unsigned int *) calloc(1, sizeof(*reference_counter_x1));
        memory_check(reference_counter_x1);
        *reference_counter_x1 = 0;
        neighbours_x1 = slist_prepend(neighbours_x1, reference_counter_x1);
        ht_insert(network->node_to_edges, x1, neighbours_x1);
    }
    if (neighbours_x2 == NULL) {
        /* No estaba en la hash */
        unsigned int *reference_counter_x2 = NULL;
        reference_counter_x2 = (unsigned int *) calloc(1, sizeof(*reference_counter_x2));
        memory_check(reference_counter_x2);
        *reference_counter_x2 = 0;
        neighbours_x2 = slist_prepend(neighbours_x2, reference_counter_x2);
        ht_insert(network->node_to_edges, x2, neighbours_x2);
    }
    /* Actualizar Refernce Counters */
    {
        unsigned int *rc_x1 = NULL;
        unsigned int *rc_x2 = NULL;

        rc_x1 = slist_head_data(neighbours_x1);
        rc_x2 = slist_head_data(neighbours_x2);

        *rc_x1 = *rc_x1 + 1;
        *rc_x2 = *rc_x2 + 1;
    }

    /* Si repetimos aristas, algo anda mal.. mmmmmmmm*/
    assert((slist_find(neighbours_x1, (void *) edge) != NULL) &&
            "No soportamos aristas repetidas");

    /* En la pos 0 de las listas esta el reference counter */
    /* En la pos 1 empieza la lista de vecinos */
    slist_insert(neighbours_x1, (void *) edge, 1);
}

SList *network_neighbours(Network *self, const Node n) {
    SList *result = NULL, *tmp = NULL;
    Edge *e = NULL;

    /* Checkeo de precondiciones */
    assert(self != NULL);

    tmp = network_get_edges(self, n);
    while (tmp != NULL) {
        /* Tomamos la cabeza de la lista */
        e = (Edge *) slist_head_data(tmp);

        result = slist_append(result, (void *) edge_get_second(e));
        tmp = slist_next(tmp);
    }

    return result;
}

SList *network_get_edges(Network *self, const Node node) {
    SList *result = NULL;

    /* Checkeo de precondiciones */
    assert(self != NULL);
    assert(node != NULL);

    result = ht_lookup(self->node_to_edges, (void *) &node);

    if (result != NULL) {
        result = slist_next(result);
    }

    return result;
}

bool network_has_node(Network *self, const Node node) {
    assert(self != NULL);
    assert(node != NULL);
    return (ht_lookup(self->node_to_edges, (void *) &node) != NULL);
}

void network_destroy(Network *self) {
    assert(self != NULL);
    ht_destroy(self->node_to_edges);
    free(self); self = NULL;
}

Edge *network_del_edge(Network *network, Edge *edge) {
    unsigned int *rc_x1 = NULL;
    unsigned int *rc_x2 = NULL;
    Node *x1 = NULL, *x2 = NULL;
    SList *neighbours_x1 = NULL, *neighbours_x2 = NULL;

    x1 = edge_get_first(edge);
    x2 = edge_get_second(edge);

    neighbours_x1 = ht_lookup(network->node_to_edges, x1);
    neighbours_x2 = ht_lookup(network->node_to_edges, x2);

    /* Yo se, que en la primera posicion de la lista deberia ir el Ref Coun */
    assert(neighbours_x1 != NULL);
    assert(neighbours_x2 != NULL);

    /* Los reference counters existen */
    rc_x1 = slist_head_data(neighbours_x1);
    rc_x2 = slist_head_data(neighbours_x2);

    assert(*rc_x1 > 0);
    assert(*rc_x2 > 0);

    *rc_x1 = *rc_x1 - 1;
    *rc_x2 = *rc_x2 - 1;

    assert(slist_find(neighbours_x1, edge));
    slist_remove(neighbours_x1, edge);
    if (*rc_x1 == 0) {
        /* Este nodo ya no existe mas en la Hash */
        ht_remove(network->node_to_edges, x1);
    }

    assert(slist_find(neighbours_x1, edge));
    slist_remove(neighbours_x2, edge);
    if (*rc_x2 == 0) {
        /* Este nodo ya no existe mas en la Hash */
        ht_remove(network->node_to_edges, x2);
    }

    return edge;
}
