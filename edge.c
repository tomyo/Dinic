#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "edge.h"
#include "defs.h"

/**
 * \brief
 * Arista: Node (x1) --  Flow(f)[\Capacity(c)] -- Node (x2).
 *
 * Contiene los Nodos, la capacidad y el flujo de una arista.
 */

struct s_Edge {
    Node *x1;
    Node *x2;
    Capacity c;
    Flow f;
};

Edge *edge_create(Node *x1, Node *x2, Capacity c, Flow f) {
    Edge *result = NULL;
    
    assert(x1 != x2);/* No se puede hacer una arista a si mismo */
    if (x2 != NULL) {
        /* parche feo para que ande con los edges dummies en network.c */
         assert(*x1 != *x2);
    }
    assert(x1 != x2); 
    
    result = calloc(1, sizeof(*result));
    if (result != NULL) {
        result->x1 = x1;
        result->x2 = x2;
        result->c = c;
        result->f = f;
    }

    return result;
}

Capacity edge_get_capacity(Edge *self) {
    assert(self != NULL);
    return self->c;
}

Flow edge_get_flow(Edge *self) {
    assert(self != NULL);
    return self->f;
}

Node *edge_get_first(Edge *self) {
    assert(self != NULL);
    return self->x1;
}

Node *edge_get_second(Edge *self) {
    assert(self != NULL);
    return self->x2;
}


bool edge_cmp(const Edge *e1, const Edge *e2) {
    assert(e1 != NULL && e2 != NULL);

    return (*e1->x1 == *e2->x1 && *e1->x2 == *e2->x2);
}

void edge_pprint(const Edge *self) {
    assert(self != NULL);

    printf("%u %u %u (%u)\n", *self->x1, *self->x2, self->f, self->c);
}

void edge_destroy(Edge *self) {
    assert(self != NULL);
    free(self);
}

void edge_update_flow(Edge *self, unsigned int new_flow) {
    assert(self != NULL);
    self->f = new_flow;
}
