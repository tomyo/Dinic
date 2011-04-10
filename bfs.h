#ifndef BFS_H
#define BFS_H

#include "network.h"
#include <glib-2.0/glib.h>

/**
 * @file bfs.h
 * \brief Modulo para correr el algoritmo BFS sobre networks
 *
 * Devuelve el camino mas corto de el nodo s al nodo t y el flujo
 * que se puede enviar por este camino
 */

/** El resultado sera devuelto en esta estructura que contiene una lista
 * con los nodos del camino y el flujo que se puede enviar */
typedef struct s_bfs_result {
    GSList *path;
    Weight flow;
} bfs_result;

/** Corre DFS sobre net y devuelve el camino mas corto de s a t */
bfs_result bfs(Network *net, Node s, Node t);

#endif
