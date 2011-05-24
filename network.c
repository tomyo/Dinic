#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "network.h"
#include "hashtable.h"

#define NODE_SIZE sizeof(Node)
#define EDGE_SIZE sizeof(Edge)

int compare_edges(gconstpointer a, gconstpointer b);
void destroy_glist(gpointer list);
void destroy_edge(gpointer edge);

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


/* Crea un network vacio */
Network *network_create(void) {
    Network *result = NULL;

    result = calloc(1, sizeof(*result));
    if(result != NULL) {
        result->node_to_edges = hash_table_new(MODE_ALLREF);
    }

    return result;
}

void network_add_edge(Network *self, Edge *e) {
    GSList *edges = NULL;
    gpointer first_node = NULL;
    int check = 0;

    /* Precondiciones */
    assert(self != NULL);
    assert(e != NULL);

    first_node = edge_get_first(e);
    edges = hash_table_lookup(self->node_to_edges, first_node, NODE_SIZE);

    if(edges == NULL) {
        /* El elemento no existia en la hash table */

        edges = g_slist_append(edges, (gpointer) e);
        hash_table_add(self->node_to_edges, first_node, NODE_SIZE,\
                            edges, 3*NODE_SIZE);
                            
    } else if(g_slist_find_custom(edges, e, compare_edges) == NULL) {
        /* El elemento no esta en la lista, lo agregamos */

        /* Hacemos un prepend, pues es O(1), append es O(n)*/
        edges = g_slist_prepend(edges, (gpointer) e);
        /* reemplazamos de la hash el elemento modificado */
        hash_table_add(self->node_to_edges, first_node, NODE_SIZE,\
                             edges, 3*NODE_SIZE);
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

GSList *network_get_edges(Network *self, Node n) {
    GSList *result = NULL;

    /* Checkeo de precondiciones */
    assert(self != NULL);

    result = hash_table_lookup(self->node_to_edges, (gpointer)&n, NODE_SIZE);

    return result;
}


void network_destroy(Network *self) {
    assert(self != NULL);
    /*TODO: liberar edges en hash manualmente */
    HT_DESTROY(self->node_to_edges);
    free(self); self = NULL;
}
