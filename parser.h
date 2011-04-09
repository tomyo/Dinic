#ifndef PARSER_H
#define PARSER_H

#include "edge.h"

/**
 * @file parser.h
 * Modulo que se utiliza para parsear y devolver una arista
 */

/** Lee una arista de fh, si no es valida devulve null */
Edge *parse_edge(FILE *fh);

#endif
