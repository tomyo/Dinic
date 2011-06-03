#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "bfs.h"
#include "parser.h"
#include "defs.h"

#define s 0
#define t 1

/**
 * \mainpage dinic
 *
 * \section Introduction
 *
 * \section Implementacion
 *
 * \section other_aspects Otros Aspectos
 *
 */

/**
 * @file main.c
 *
 * \brief Lee los datos de la entrada, carga el network, corre dinic y muestra
 * el resultado
 */

int main(int argc, char *argv[]) {
    Network *network = NULL;
    struct parse_result options;

    if (parse_params(argc, argv, &options) == false) {
        /* Parametros malos */
        exit(1);
    }

    if (options.verbose) puts("verbose");
    if (options.flujo) puts("flujo");
    if (options.corte) puts("corte");

    /* Parsear el network de la entrada estandar*/
    network = parse_network(stdin);

    /* Ya tenemos el network, a correrle BFS ahora */
    /* result = dinic(network, s, t); */

    /* Liberar la memoria alocada por los programas */
    network_destroy(network);

    return 0;
 }
