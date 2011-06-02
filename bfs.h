#ifndef BFS_H
#define BFS_H

#include "network.h"

/**
 * @file bfs.h
 * \brief Modulo para correr el algoritmo BFS sobre networks
 *
 * Devuelve el camino mas corto de el nodo s al nodo t y el flujo
 * que se puede enviar por este camino
 */

/**
 * \brief Una lista con el camino y el flujo que se puede enviar.
 *
 * Este es el resultado del programa.
 */
typedef struct s_bfs_result {
    SList *path;
    Flow flow;
} bfs_result;

/** Corre DFS sobre el network net y devuelve el camino mas corto de s a t */
bfs_result bfs(Network *net, Node s, Node t);

#endif
