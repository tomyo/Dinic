#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "network.h"
#include "slist.h"
#include "hashtable/hashtable.h"


static int compare_edges(const void *a, const void *b);
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
static int compare_edges(const void *a, const void *b) {
    int result = 0;

    if(edge_cmp((Edge *) a, (Edge *) b)) {
        result = 0;
    } else {
        result = 1;
    }

    return result;
}

static void destroy_slist(void *list) {
    SList *to_free = (SList *) list, *tmp;
    void *edge = NULL;

    /* Precondicion */
    assert(to_free != NULL);

    tmp = to_free;

    while (tmp != NULL) {
        edge = slist_head_data(tmp);
        edge_destroy(edge);
        tmp = slist_next(tmp);
    }

    slist_free(to_free);
}

static bool edge_is_dummy(Edge *edge) {
    return ((edge_get_second(edge) == NULL) && (edge_get_capacity(edge) == 0));
}

static void network_add_dummy_edge(Network *self, Node *node) {
    Edge *dummy = edge_create(node, NULL, 0, 0);
    network_add_edge(self, dummy);
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

void network_add_edge(Network *self, Edge *e) {
    SList *edges = NULL;
    Node *first_node = NULL;

    /* Precondiciones */
    assert(self != NULL);
    assert(e != NULL);
    first_node = edge_get_first(e);
    edges = ht_lookup(self->node_to_edges, first_node);

    if(edges == NULL) {
        /* No existe arista que comienze por el primer nodo de 'e' en network
         * => la arista no existe en la hash table (la agregamos) */
        edges = slist_append(edges, (void *) e);
        ht_insert(self->node_to_edges, first_node, edges);

    } else if(slist_find_custom(edges, e, compare_edges) == NULL) {
        /* Arista no esta en edges (lista de aristas vecinas al primer nodo),
         * i.e: la arista no existe en la hash table (la agregamos) */

        /* Si esta solo la arista dummy, la reemplazamos */
        if (slist_length(edges) == 1) {
        /* NOTA: INVARIANTE -> Si hay mas de una arista => Nou dummy */
            if (edge_is_dummy(slist_head_data(edges))) {
                edge_destroy(slist_head_data(edges));
                edges->data = e; /* TODO: slist_replace_data */
            }
        } else {
            /* Hacemos un prepend, pues es O(1) (append es O(n)) */
            edges = slist_prepend(edges, (void *) e);
            /* Ahora la lista empieza por la nueva arista => hay que
             * reinsertar la lista en la hash (actualizar referencia) */
            ht_steal(self->node_to_edges, first_node);
            ht_insert(self->node_to_edges, first_node, edges);
        }

    } else {
        /* Arista repetida. Le avisamos al usuario,
         * Liberamos la arista y continuamos con la ejecucion */
        edge_destroy(e);
        return;
    }
    /* Ahora vemos si el segundo nodo ya pertenece al network, si no,
     * lo agregamos con una arista dummy (si no es ya una dummy!)*/
    if ((!edge_is_dummy(e)) && !network_has_node(self, edge_get_second(e))) {
        network_add_dummy_edge(self, edge_get_second(e));
    }
}

SList *network_neighbours(Network *self, const Node *n) {
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

SList *network_get_edges(Network *self, const Node *node) {
    SList *result = NULL;

    /* Checkeo de precondiciones */
    assert(self != NULL);
    result = ht_lookup(self->node_to_edges, node);

    /* En caso de ser la lista con la arista dummy, se saca de result */
    if (slist_length(result) == 1) {
        /* NOTA: INVARIANTE -> Si hay mas de una arista, dummy ya fue quitado.*/
        if (edge_is_dummy(slist_head_data(result))) {
            result = NULL;
        }
    }

    return result;
}

bool network_has_node(Network *self, const Node *n) {
    return (network_get_edges(self, n) != NULL);
}

void network_destroy(Network *self) {
    assert(self != NULL);
    ht_destroy(self->node_to_edges);
    free(self); self = NULL;
}

Edge * network_del_edge(Network *self, Edge *e) {
    SList *edges = NULL;
    Node *node = NULL;
    Edge *result = NULL;

    /* Checkeo de precondiciones */
    assert(self != NULL);
    assert(e != NULL);

    node = edge_get_first(e);

    edges = ht_lookup(self->node_to_edges, node);

    if (edges == NULL) {
        /* El nodo no esta */
        return NULL;
    }

    result = slist_find(edges, e);

    if (result == NULL) {
        /* La arista no esta */
        return NULL;
    }

    edges = slist_remove(edges, e);

    if (edges == NULL) {
        /* Ya no tiene aristas */
        ht_remove(self->node_to_edges, node);
    }
    else {
        /* Insertamos de vuelta porque el inicio puede haber cambiado */
        ht_insert(self->node_to_edges, node, edges);
    }

    return result;
}
