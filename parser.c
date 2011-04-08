#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "edge.h"

#define FORMAT "%u %u %u"

Edge *parse_edge(FILE *fh) {
    Node x1 = 0, x2 = 0;
    Weight w = 0;
    Edge *result = NULL;

    printf("> ");
    if(fscanf(fh, FORMAT, &x1, &x2, &w) != 3) {
        /* La entrada no era valida */
        result = NULL;
    } else {
        result = edge_create(x1, x2, w);
        /* Manejando falta de memoria */
        if(result == NULL) {
            fprintf(stderr, "Memoria insuficiente\n");
            exit(1);
        }
    }

    return result;
}
