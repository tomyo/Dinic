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
    /* Network *network = NULL; */
    /* struct parse_result options; */
    /* dinic_result *result = NULL; */
    /* Node s = 0, t = 1; */

    /* Las opciones se acceden con options.verbose, options.flujo
     * u options.corte y son de tipo booleano */
    /* if (parse_options(argc, argv, &options) == false) { */
        /* Parametros malos */
        /* exit(1); */
    /* } */

    /* Parsear el network de la entrada estandar */
    /* network = parse_network(stdin); */

    /* Ya tenemos el network, a correrle BFS ahora */
    /* result = dinic(network, s, t, options.verbose); */

    /* print_output(result, options); */

    /* Liberar la memoria alocada por los programas */
    /* network_destroy(network); */

    Network *net = NULL;
    dinic_t *dt = NULL;
    Edge *e1 = NULL, *e2 = NULL, *e3 = NULL;
    Edge *e4 = NULL, *e5 = NULL, *e6 = NULL, *e7 = NULL;
    Node s = 0, t = 1;
    DinicFlow *flow = NULL;
    SList *path = NULL;
    Edge *current_edge = NULL;
    unsigned int sequence[] = {0, 4, 5, 6, 1};
    int expected = 0;


    /*
       Network a representar

           2
          /  \
         s    3
          \  /
           4 —— 5 — 6 — t

        Todas las capacidades son 1
    */

    /* Creo un network vacio */
    net = network_create();

    /* Creo los lados */
    e1 = edge_create(s, 2, 1, 0);
    e2 = edge_create(s, 4, 1, 0);
    e3 = edge_create(2, 3, 1, 0);
    e4 = edge_create(4, 3, 1, 0);
    e5 = edge_create(5, 6, 1, 0);
    e6 = edge_create(6, t, 1, 0);
    e7 = edge_create(4, 5, 1, 0);

    network_add_edge(net, e2);
    network_add_edge(net, e1);
    network_add_edge(net, e3);
    network_add_edge(net, e4);
    network_add_edge(net, e5);
    network_add_edge(net, e6);
    network_add_edge(net, e7);


    /* Completo la estructura con los datos */
    dt = (dinic_t *) calloc(1, sizeof(*dt));
    dt->network = NULL; /* Not used. Not tested */
    dt->s = s;
    dt->t = t;
    dt->result = NULL; /* Not used. Not tested */

    flow = aux_network_find_flow(dt, net, false);
    path = flow->path;
    assert(!slist_is_empty(path));

    while (path != NULL) {
        current_edge = slist_head_data(path);
        edge_pprint(current_edge);

        assert(*edge_get_first(current_edge) == sequence[expected]);

        expected += 1;
        path = slist_next(path);
    }

    assert(expected == 4);

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
