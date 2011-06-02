#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "edge.h"

#define FORMAT "%u %u %u"

Edge *parse_edge(FILE *fh) {
    Node x1 = 0, x2 = 0;
    Capacity c = 0;
    Edge *result = NULL;

    if(fscanf(fh, FORMAT, &x1, &x2, &c) != 3) {
        /* La entrada no era valida */
        result = NULL;
    } else {
        /* El flujo inicial se setea en 0 */
        result = edge_create(x1, x2, c, 0);
        /* Manejando falta de memoria */
        if(result == NULL) {
            fprintf(stderr, "Memoria insuficiente\n");
            exit(1);
        }
    }

    return result;
}
