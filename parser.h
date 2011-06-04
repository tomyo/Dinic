#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>
#include "edge.h"
#include "network.h"

/**
 * @file parser.h
 * \brief Modulo que se utiliza para parsear y devolver una arista
 */

/** Lee una arista de fh, si no es valida devulve null */
Edge *parse_edge(FILE *fh);

/** Parsea un network de fh */
Network *parse_network(FILE *fh);

struct parse_result {
    bool verbose;
    bool flujo;
    bool corte;
};

/** Parsea en busca de  las opciones especificadas
 * Devuelve true si el parsiing fue exitoso, false caso contrario
 * En parse sera guardado los parametros que se pasaron */
bool parse_options(int argc, char *argv[], struct parse_result *parse);

#endif
