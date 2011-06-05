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

#define INF -1 /* Funciona porque los pesos son unsigned */
#define min(x,y) (x < y ? x : y)
#define max(x,y) (x < y ? y : x)

/* Nodos destino y origen estandard */
#define s 0
#define t 1


#endif /*DEFINITIONS_H*/
