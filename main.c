#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "dinic.h"
#include "parser.h"
#include "defs.h"

/* TODO: Ver donde se puede meter la documentación de modularizacion y todo eso */

/**
 * @file main.c
 *
 * \brief Lee los datos de la entrada, carga el network, corre dinic y muestra
 * el resultado
 */

static void print_output(dinic_result *result, struct parse_result options);

int main(int argc, char *argv[]) {
    Network *network = NULL;
    struct parse_result options;
    dinic_result *result = NULL;
    Node s = 0, t = 1;

    /* Las opciones se acceden con options.verbose, options.flujo
     * u options.corte y son de tipo booleano */
    if (parse_options(argc, argv, &options) == false) {
        /* Parametros malos */
        exit(1);
    }

    /* Parsear el network de la entrada estandar */
    network = parse_network(stdin);

    /* Ya tenemos el network, a correrle BFS ahora */
    result = dinic(network, &s, &t, options.verbose);

    print_output(result, options);

    /* Liberar la memoria alocada por los programas */
    network_destroy(network);

    return 0;
}

/** Imprime la salida */
static void print_output(dinic_result *result, struct parse_result options) {
    SList *max_flow = NULL;
    SList *min_cut = NULL;

    if (options.flujo) {
        max_flow = result->max_flow;
        printf("Flujo Maximal\n");

        while(max_flow != NULL){
            edge_pprint((Edge *) slist_head_data(max_flow));
        }

        printf("\n");
        printf("Valor del Flujo: %u\n", result->flow_value);
    }

    if (options.corte) {
        min_cut = result->min_cut;

        printf("Corte: {");

        /* Iteramos sobre el corte separando el ultimo elemento */
        while(slist_next(min_cut) != NULL) {
            printf("%u, ", *((unsigned int *) slist_head_data(min_cut)));
            min_cut = slist_next(min_cut);
        }

        printf("%u}\n", *((unsigned int *) slist_head_data(min_cut)));
    }
}
