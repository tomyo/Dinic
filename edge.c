#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "edge.h"

struct s_Edge {
    Node x1;
    Node x2;
    Weight w;
};

Edge edge_create(Node x1, Node x2, Weight w) {
    Edge *result = NULL;
    result = calloc(1, sizeof(*result));

    if (result != NULL) {
        result->x1 = x1;
        result->x2 = x2;
        result->w  = w;
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

void edge_destroy(Edge *self) {
    if(self != NULL) {
        free(self);
    }
}
