#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "edge.h"

struct s_Edge {
    Node x1;
    Node x2;
    Weight w;
};

Edge *edge_create(Node x1, Node x2, Weight w) {
    Edge *result = NULL;

    assert(x1 != x2); /* No se puede hacer una arista a si mismo */

    result = calloc(1, sizeof(*result));

    if (result != NULL) {
        result->x1 = x1;
        result->x2 = x2;
        result->w = w;
    }

    return result;
}

Weight edge_get_weight(Edge *self) {
    assert(self != NULL);
    return self->w;
}

Node *edge_get_first(Edge *self) {
    assert(self != NULL);
    return &self->x1;
}

Node *edge_get_second(Edge *self) {
    assert(self != NULL);
    return &self->x2;
}


bool edge_cmp(const Edge *e1, const Edge *e2) {
    assert(e1 != NULL && e2 != NULL);
    
    return (e1->x1 == e2->x1 && e1->x2 == e2->x2);
}

void edge_destroy(Edge *self) {
    assert(self != NULL);
    free(self);
}
