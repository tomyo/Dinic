#include "edge.h"
#include "node.h"
#include "slist.h"
#include "dinic_trucho.h"

/**
 * @brief Estructura interna de Dinic
 *
 */

dinic_result *dinic(Network *network, const Node *s, const Node *t, bool print_flow) {
    dinic_result *result = NULL;
    SList *max_flow = NULL;
    SList *min_cut = NULL;
    int i = 0;

    for(i=1; i<8; i++){
        Edge *e = edge_create(i, i*2, i*3, i*4);

        slist_append(max_flow, (void *) e);
    }

    for(i=0; i<5; i++){
        slist_append(min_cut, &i);
    }

    result = (dinic_result *) malloc(sizeof(*result));

    return result;
}
