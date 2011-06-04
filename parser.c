#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include "parser.h"
#include "edge.h"
#include "defs.h"

#define FORMAT "%u %u %u"
#define HELP_MESSAGE "Usage: dinic [OPTIONS]\n"\
                     "Encuentra el flujo maximal y el corte minimal en"\
                     "un network usando el algoritmo de Dinic\n\n"\
                     "Opciones:\n"\
                     "  -f --flujo      Imprime una tabla con los valores del flujo\n"\
                     "  -v --verbose    Imprime los caminos en cada network auxiliar\n"\
                     "  -c --corte      Se muestra el corte al finalizar\n"\
                     "  -h --help       Muestra esta ayuda\n"

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


bool parse_options(int argc, char *argv[], struct parse_result *parse) {
    int opt = 0, option_index = 0;
    bool result = true;

    static struct option long_options[] = {
                   {"verbose", 0, 0, 'v'},
                   {"flujo"  , 0, 0, 'f'},
                   {"corte"  , 0, 0, 'c'},
                   {"help"   , 0, 0, 'h'},
                   { 0       , 0, 0, 0}
    };


    parse->verbose = false;
    parse->flujo = false;
    parse->corte = false;

    while((opt = getopt_long(argc, argv, "cvfh",
                             long_options, &option_index)) != -1) {
        switch (opt) {
            case 'v':
                parse->verbose = true;
                break;

            case 'f':
                parse->flujo = true;
                break;

            case 'c':
                parse->corte = true;
                break;
            case 'h':
                puts(HELP_MESSAGE);
                result = false;
                break;
            default:
                puts(HELP_MESSAGE);
                result = false;
                break;
        }
    }
    return result;
}
