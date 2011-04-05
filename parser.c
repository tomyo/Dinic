#include <stdio.h>
#include "parser.h"

#define FORMAT "%d %d %d"

Edge parse_edge(FILE *fh) {
    Node x1 = 0, x2 = 0;
    Weigth w = 0;
    Edge *result = NULL;

    if(fscanf(fh, FORMAT, &x1, &w, &x2) != 3) {
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
