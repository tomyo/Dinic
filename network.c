#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "network.h"
#include "slist.h"
#include "defs.h"
#include "hashtable/hashtable.h"


static void destroy_slist(void *list);
static int compare_edges(const void *a, const void *b);
void _remove_backwards(Node *nodo, SList *list);

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

    if(edge_cmp(a, b)) {
        result = 0;
    } else {
        result = 1;
    }

    return result;
}

static void destroy_slist_internal(void *list, bool aux_mode) {
    SList *current = NULL;

    /* Precondicion */
    assert(list != NULL);

    current = (SList *) list;

    /* Liberamos el reference counter */
    free(slist_head_data(current));
    current = slist_next(current);

    if (!aux_mode) {
        /* Liberando los edges */
        while(!slist_is_empty(current)) {
            edge_destroy(slist_head_data(current));
            current = slist_next(current);
        }
    }

    /* Liberando estructura de listas (ambos casos) */
    slist_free(list);
}

/* Esta funcion no debe llamarse si son los vecinos de t los nodos a liberar */
static void destroy_slist(void *list) {
    destroy_slist_internal(list, false);
}

static void destroy_slist_aux(void *list) {
    destroy_slist_internal(list, true);
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

Network *network_aux_create(void) {
    Network *result = NULL;

    result = calloc(1, sizeof(*result));
    if(result != NULL) {
        result->node_to_edges = ht_new_full(MODE_ALLREF, NULL, destroy_slist_aux);
    }

    return result;
}

/* TODO: Comentar */
/* mode == 'f' forward, 'b' backward */
void _network_add_edge(Network *network, Edge *edge, char mode) {
    Node *x1 = NULL, *x2 = NULL;
    SList *neighbours_x1 = NULL, *neighbours_x2 = NULL;

    assert(mode == 'f' || mode == 'b');

    x1 = (mode == 'f')?edge_get_first(edge):edge_get_second(edge);
    x2 = (mode == 'f')?edge_get_second(edge):edge_get_first(edge);

    neighbours_x1 = ht_lookup(network->node_to_edges, x1);
    neighbours_x2 = ht_lookup(network->node_to_edges, x2);

    /* Yo se, que en la primera posicion de la lista deberia ir el Ref Count */
    if (neighbours_x1 == NULL) {
        /* No estaba en la Hash */
        unsigned int *reference_counter_x1 = NULL;

        reference_counter_x1 = (unsigned int *) calloc(1,
                                                sizeof(*reference_counter_x1));
        memory_check(reference_counter_x1);
        *reference_counter_x1 = 0;
        neighbours_x1 = slist_prepend(neighbours_x1, reference_counter_x1);

        assert(!ht_has_key(network->node_to_edges, x1));
        ht_insert(network->node_to_edges, x1, neighbours_x1);
    }
    if (neighbours_x2 == NULL) {
        /* No estaba en la hash */
        unsigned int *reference_counter_x2 = NULL;

        reference_counter_x2 = (unsigned int *) calloc(1,
                                                sizeof(*reference_counter_x2));
        memory_check(reference_counter_x2);
        *reference_counter_x2 = 0;
        neighbours_x2 = slist_prepend(neighbours_x2, reference_counter_x2);

        assert(!ht_has_key(network->node_to_edges, x2));
        ht_insert(network->node_to_edges, x2, neighbours_x2);
    }
    /* Actualizar Reference Counters */
    {
        unsigned int *rc_x1 = NULL;
        unsigned int *rc_x2 = NULL;

        rc_x1 = slist_head_data(neighbours_x1);
        rc_x2 = slist_head_data(neighbours_x2);

        *rc_x1 = *rc_x1 + 1;
        *rc_x2 = *rc_x2 + 1;
    }

    /* Si repetimos aristas, algo anda mal.. mmmmmmmm*/
    assert((slist_find_custom(slist_next(neighbours_x1), (void *) edge, compare_edges)
            == NULL) &&  "No soportamos aristas repetidas");

    /* En la pos 0 de las listas esta el reference counter */
    /* En la pos 1 empieza la lista de vecinos */
    neighbours_x1 = slist_insert(neighbours_x1, (void *) edge, 1);

    /* Postcondicion, salteandome el primero (Ref counter) tiene que estar
     * el que yo quiero agregar */
    neighbours_x1 = ht_lookup(network->node_to_edges, x1);
    neighbours_x1 = slist_next(neighbours_x1);
    assert(slist_head_data(neighbours_x1) == edge);
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

    result = ht_lookup(self->node_to_edges, &node);

    if (result != NULL) {
        result = slist_next(result);
    }

    return result;
}

SList *network_nodes(Network *self) {
    SList *result = NULL;
    Node *current = NULL;

    /* Precondiciones */
    assert(self != NULL);

    ht_iter_keys_init(self->node_to_edges);

    while (!ht_iter_keys_is_done(self->node_to_edges)) {
        current = (Node *) ht_iter_keys_next(self->node_to_edges);
        result = slist_prepend(result, current);
    }

    return result;
}

SList *network_forward_edges(Network *self){
    SList *nodes = NULL, *iter_nodes = NULL, *result = NULL;

    /* Precondiciones */
    assert(self != NULL);

    /* Itero sobre los nodos del network */
    nodes = network_nodes(self);
    iter_nodes = nodes;

    while (iter_nodes != NULL) {
        SList *edges = NULL, *iter_edges = NULL;
        Node node = 0;

        node = *((Node *)slist_head_data(iter_nodes));

        /* Itero sobre las aristas vecinas del nodo actual */
        edges = network_get_edges(self, node);
        iter_edges = edges;

        while (iter_edges != NULL) {
            Edge *edge = NULL;

            edge = slist_head_data(iter_edges);

            if(node == *edge_get_first(edge)) {
                /* Arista fordward */
                result = slist_prepend(result, edge);
            }
            iter_edges = slist_next(iter_edges);
        }
        iter_nodes = slist_next(iter_nodes);
    }
    slist_free(nodes);

    /* Post Condicion */
    return result;
}

bool network_has_node(Network *self, const Node node) {
    assert(self != NULL);
    return (ht_lookup(self->node_to_edges, &node) != NULL);
}

void _remove_backwards(Node *nodo, SList *list) {
    SList *iter = NULL;
    assert(!slist_is_empty(list));

    /* Skipping reference counter */
    iter = slist_next(list);
    while (!slist_is_empty(iter) &&
            (*edge_get_first((Edge *)slist_head_data(iter)) != *(Node *)nodo)) {
        SList *iter_temp = slist_next(iter);
        slist_free_1(iter);
        iter = iter_temp;
    }
    /* Hardcoding */
    list->next = iter;
}

void network_destroy(Network *self) {
    assert(self != NULL);

    {
    /* <FIX> */
        Node *nodo = NULL;

        ht_iter_keys_init(self->node_to_edges);

        while (!ht_iter_keys_is_done(self->node_to_edges)) {
            nodo = (Node *) ht_iter_keys_next(self->node_to_edges);
            _remove_backwards(nodo, ht_lookup(self->node_to_edges, nodo));
        }
    /* </FIX> */
    }

    ht_destroy(self->node_to_edges);
    free(self);
}


void network_free(Network *self) {
    assert(self != NULL);
    ht_free(self->node_to_edges);
    free(self); self = NULL;
}


/* TODO: Comentar */
/* mode == 'f' forward, 'b' backward */
Edge *_network_del_edge(Network *network, Edge *edge, char mode) {
    unsigned int *rc_x1 = NULL;
    unsigned int *rc_x2 = NULL;
    Node *x1 = NULL, *x2 = NULL;
    SList *neighbours_x1 = NULL, *neighbours_x2 = NULL;

    assert(network != NULL);
    assert(edge != NULL);
    assert(mode == 'f' || mode == 'b');

    x1 = (mode=='f')?edge_get_first(edge):edge_get_second(edge);
    x2 = (mode=='f')?edge_get_second(edge):edge_get_first(edge);

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

    assert(slist_find_custom(slist_next(neighbours_x1), edge, compare_edges));
    neighbours_x1->next = slist_remove(slist_next(neighbours_x1), edge, compare_edges);
    if (*rc_x1 == 0) {
        /* Este nodo ya no existe mas en la Hash */
        ht_remove(network->node_to_edges, x1);
    }

    if (*rc_x2 == 0) {
        /* Este nodo ya no existe mas en la Hash */
        ht_remove(network->node_to_edges, x2);
    }

    return edge;
}

void network_update(Network *self, SList *path, Flow flow){
    Node *next_node = NULL;
    SList *iter = NULL;

    /* Precondicion */
    assert(self != NULL);
    assert(path != NULL);
    assert(flow > 0);

    iter = path;

    next_node = edge_get_first((Edge *)slist_head_data(iter));
    while (iter != NULL) {
        Edge *edge = slist_head_data(iter);
        if(*edge_get_first(edge) == *next_node) {
            /* Forward */
            Flow new_flow = 0;
            Flow previous_flow = 0;

            previous_flow = edge_get_flow(edge);

            if (previous_flow == 0) {
                /* Tengo que poner al lado como backward */
                network_add_edge_backward(self, edge);
            }

            new_flow = previous_flow + flow;
            assert(new_flow <= edge_get_capacity(edge));
            edge_set_flow(edge, new_flow);

            next_node = edge_get_second(edge);
        } else {
            /* Backward */
            Flow new_flow = 0;

            new_flow = edge_get_flow(edge) -  flow;
            /* new_flow es mayor o igual a cero siempre por ser unsigned */

            if (new_flow == 0) {
                /* El lado se vacio */
                network_del_edge_backward(self, edge);
            } else {
                edge_set_flow(edge, new_flow);
            }

            next_node = edge_get_first(edge);
        }
        iter = slist_next(iter);
    }
}

void network_add_edge(Network *network, Edge *edge) {
    _network_add_edge(network, edge, 'f');
}

void network_add_edge_backward(Network *network, Edge *edge) {
    _network_add_edge(network, edge, 'b');
}

Edge *network_del_edge(Network *network, Edge *edge) {
    return _network_del_edge(network, edge, 'f');
}

Edge *network_del_edge_backward(Network *network, Edge *edge) {
    return _network_del_edge(network, edge, 'b');
}
