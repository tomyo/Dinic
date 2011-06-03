#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include "parser.h"
#include "edge.h"
#include "defs.h"

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

Network *parse_network(FILE *fh) {
    Edge *edge = NULL;
    Network *result = NULL;
    bool reach_EOF = false;

    /* Creamos network vacio */
    result = network_create();
    memory_check(result);

    while(not reach_EOF) {
        edge = parse_edge(fh);
        if(edge == NULL) {
            reach_EOF = true;
        } else {
            network_add_edge(result, edge);
        }
    }

    return result;
}

#define NO_OPTIONS 0
#define PRINT_HELP fprintf(stderr, "Usage: %s [--verbose] [--corte]"\
                   " [--flujo]\n",argv[0]);


bool parse_params(int argc, char *argv[], struct parse_result *parse) {
    int opt = 0;
    int option_index = 0;
    static struct option long_options[] = {
                   {"verbose", 0, 0, 0},
                   {"flujo"  , 0, 0, 0},
                   {"corte"  , 0, 0, 0},
                   { 0       , 0, 0, 0}
    };


    parse->verbose = false;
    parse->flujo = false;
    parse->corte = false;

    if (argc > 4) {
        PRINT_HELP
        return false;
    }

    while((opt = getopt_long(argc, argv, "",
                                         long_options, &option_index)) != -1) {
        if (opt != 0) {
            PRINT_HELP
            return false;
        }

        switch (option_index) {
            case 0:
                if (parse->verbose) {
                    PRINT_HELP
                    return false;
                }
                parse->verbose = true;
                break;

            case 1:
                if (parse->flujo) {
                    PRINT_HELP
                    return false;
                }
                parse->flujo = true;
                break;

            case 2:
                if (parse->corte) {
                    PRINT_HELP
                    return false;
                }
                parse->corte = true;
                break;
        }
    }
    return true;
}
