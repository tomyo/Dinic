#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "dinic.h"
#include "parser.h"
#include "defs.h"

/**
 * @file main.c
 *
 * @brief Main del programa.
 *
 * Lee los datos de la entrada, carga el network, corre dinic y muestra
 * el resultado.
 */

/**
 * @mainpage Proyecto de Discreta
 *
 * Segundo Laboratorio de la materia Discreta II de FaMAF.
 *
 * @section usage Uso
 * @subsection args Argumentos
 *   - -f --flujo      Imprime una tabla con los valores del flujo
 *   - -v --verbose    Imprime los caminos en cada network auxiliar
 *   - -c --corte      Se muestra el corte al finalizar
 *   - -h --help       Muestra la lista de argumentos validos
 *
 *
 * @subsection input Entrada
 * Toma de la entrada estandar las aristas pasadas para generar el network.
 *
 * Para ingresar una arista:
 *     A -> B con capacidad C
 *
 * Se usara el siguiente formato:
 *   A B C
 *
 * La lista de aristas ingresadas se separaran mediante un salto de linea.
 *
 * Tanto nodos como capacidades son enteros.
 * No se aceptaran otros labels para los nodos.
 *
 * Para empezar el calculo (Se ha terminado de entrar el network), te terminara
 * la entrada con cualquier arista no valida.
 *
 *
 * @section authors Autores
 *  - Andres Bordese
 *  - Gisela Rossi
 *  - Gonzalo Garcia Berrotaran
 *  - Julia Medina
 *  - Tomas Hayes
 *
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
    memory_check(network);
    assert(network_has_node(network, s));
    assert(network_has_node(network, t));

    /* Ya tenemos el network, ahora le corremos Dinic*/
    result = dinic(network, &s, &t, options.verbose);
    memory_check(result);

    print_output(result, options);

    /* Liberar la memoria alocada */
    network_destroy(network);
    slist_free(result->max_flow);
    slist_free(result->min_cut);
    free(result);

    return 0;
}

/* Imprime la salida */
static void print_output(dinic_result *result, struct parse_result options) {
    SList *max_flow = NULL;
    SList *min_cut = NULL;

    if (options.flujo) {
        max_flow = result->max_flow;
        printf("Flujo Maximal\n");

        while(max_flow != NULL){
            edge_pprint((Edge *) slist_head_data(max_flow));
            max_flow = slist_next(max_flow);
        }

        printf("\n");
    }
    printf("Valor del Flujo: %u\n", result->flow_value);

    printf("Valor del Flujo: %u\n", result->flow_value);

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
