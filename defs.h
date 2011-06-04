#ifndef DEFINITIONS_H
#define DEFINITIONS_H

/**
 * @file defs.h
 * \brief Definición de algunas constantes
 */

#define memory_check(m) do\
{\
    if(m == NULL) {\
        fprintf(stderr, "Memoria insuficiente\n");\
        exit(1);\
    }\
} while(0)

#endif /*DEFINITIONS_H*/
