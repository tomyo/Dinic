#ifndef BFS_H
#define BFS_H

#include "network.h"
#include <glib-2.0/glib.h>

/*Corre bfs sobre el network self
  Source: Node s
  Sink: Node t
  */

typedef struct s_bfs_result {
    GSList *path;
    Weight flow;
} bfs_result;

bfs_result bfs(Network *self, Node s, Node t);

#endif
