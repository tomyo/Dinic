#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>
#include "edge.h"
#include "network.h"

/**
 * @file parser.h
 * @brief Modulo de parseo de la entrada estandar.
 */

/**
 * @brief Estructura para los argumentos del programa.
 */
struct parse_result {
    /**
     * @brief Imprime o no los pasos intermedios?
     */
    bool verbose;

    /**
     * @brief Imprime o no el flujo final?
     */
    bool flujo;

    /**
     * @brief Imprime o no el corte?
     */
    bool corte;
};

/**
 * @brief Lee una arista de fh.
 * @param fh File descriptor de donde leer.
 * @returns La arista leida (Si es valida).
 * @returns NULL (Si no lo es).
 */
Edge *parse_edge(FILE *fh);

/**
 * @brief Parsea un network de fh.
 * @param fh File descriptor de donde leer.
 * @returns El network leido.
 */
Network *parse_network(FILE *fh);

/**
 * @brief Parsea en busca de las argumentos del programa.
 * @param argc Cantidad de argumentos pasados en argv.
 * @param argv Argumentos pasados al ejecutable cuando se corre.
 * @param parse Estructura donde almacenar los parametros.
 * @returns true si el parsing fue exitoso.
 * @returns false en caso contrario.
 */
bool parse_options(int argc, char *argv[], struct parse_result *parse);

#endif
