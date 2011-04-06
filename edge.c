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
        result->x1                = x1;
        result->x2                = x2;
        result->w                 = w;
        result->reference_counter = 0;
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

void edge_increment_reference(Edge *self) {
    assert(self != NULL);
    self->rederence_counter++;
}

void edge_destroy(Edge *self) {
    if(self != NULL) {
        if(self->reference_counter <= 1) {
            free(self);
        } else {
            self->reference_counter--;
        }
    }
}
