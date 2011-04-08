#ifndef PARSER_H
#define PARSER_H

#include "edge.h"

/* Lee una arista de fh, si no es valida devulve null */
Edge *parse_edge(FILE *fh);

#endif
