#include <check.h>
#include <signal.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <stdlib.h>

#include "test_dinic.h"
#include "dinic.h"
#include "network.h"
#include "edge.h"
#include "defs.h"

/* Precondition tests */
START_TEST(test_dinic_aux_net_null)
{
    aux_network_new(NULL);

}
END_TEST


/* Functionality test */
START_TEST(test_dinic_aux_net)
{
    Network *net = NULL, *aux_net = NULL;
    dinic_t *dt = NULL;
    Edge *e1 = NULL, *e2 = NULL, *e3 = NULL, *e4 = NULL;
    Edge *e5 = NULL, *e6 = NULL, *e7 = NULL, *e8 = NULL;
    SList *n1 = NULL;
    Node head = 0;
    Node s = 0, t = 1;

    /*
       Network a representar              |       Network auxiliar
                                          |
           2 — 3                          |              2 —
          /   / \                         |             /    \
         s   /    — t                     |            s    — 3 — t
          \ /       |                     |             \  /
           4 — 5  — 6                     |              4 —— 5 — 6
                                          |
       (todas las capacidades son 1)      |
    */

    /* Creo un network vacio */
    net = network_create();

    /* Creo los lados */
    e1 = edge_create(s, 2, 1, 0);
    e2 = edge_create(s, 4, 1, 0);
    e3 = edge_create(2, 3, 1, 0);
    e4 = edge_create(4, 3, 1, 0);
    e5 = edge_create(4, 5, 1, 0);
    e6 = edge_create(3, t, 1, 0);
    e7 = edge_create(5, 6, 1, 0);
    e8 = edge_create(6, t, 1, 0);

    /* Lleno el network */
    network_add_edge(net, e1);
    network_add_edge(net, e2);
    network_add_edge(net, e3);
    network_add_edge(net, e4);
    network_add_edge(net, e5);
    network_add_edge(net, e6);
    network_add_edge(net, e7);
    network_add_edge(net, e8);

    /* Completo la estructura con los datos */
    dt = (dinic_t *) calloc(1, sizeof(*dt));
    dt->network = net;
    dt->s = s;
    dt->t = t;
    dt->result = NULL; /* Not used. Not tested */

    /* Creo el network auxiliar */
    aux_net = aux_network_new(dt);

    /* Verifico los vecinos de s. Tienen que ser 2 y 4 */
    n1 = network_neighbours(aux_net, s);
    fail_unless(slist_length(n1) == 2);
    while (n1 != NULL) {
        head = *((Node *) slist_head_data(n1));
        fail_unless(head == 2 || head == 4);
        n1 = slist_next(n1);
    }
    slist_free(n1);

    /* Verifico los vecinos de 2. Tiene que ser solamente 3 */
    n1 = network_neighbours(aux_net, 2);
    fail_unless(slist_length(n1) == 1);
    head = *((Node *) slist_head_data(n1));
    fail_unless(head == 3);
    slist_free(n1);

    /* Verifico los vecinos de 4. Tienen que ser 3 y 5 */
    n1 = network_neighbours(aux_net, 4);
    fail_unless(slist_length(n1) == 2);
    while (n1 != NULL) {
        head = *((Node *) slist_head_data(n1));
        printf("%u\n", head);
        fail_unless(head == 3 || head == 5);
        n1 = slist_next(n1);
    }
    slist_free(n1);

    /* Verifico los vecinos de 3. Tiene que ser solamente t */
    n1 = network_neighbours(aux_net, 3);
    fail_unless(slist_length(n1) == 1);
    head = *((Node *) slist_head_data(n1));
    fail_unless(head == t);
    slist_free(n1);

    /* Verifico los vecinos de 5, tiene que ser 6 */
    n1 = network_neighbours(aux_net, t);
    head = *((Node *) slist_head_data(n1));
    fail_unless(head == 6);

    /* Verifico los vecinos de t, tiene que ser una lista vacia */
    n1 = network_neighbours(aux_net, t);
    fail_unless(slist_is_empty(n1));

    /* Verifico los vecinos de 6, tiene que ser una lista vacia */
    n1 = network_neighbours(aux_net, 6);
    fail_unless(slist_is_empty(n1));
}
END_TEST

/* Armado de la test suite */
Suite *dinic_suite(void){
    Suite *s = suite_create("dinic");
    TCase *tc_preconditions = tcase_create("Precondition");
    TCase *tc_creation = tcase_create("Creation");
    TCase *tc_functionality = tcase_create("Functionality");

    /* Precondiciones */
    tcase_add_test_raise_signal(tc_preconditions, test_dinic_aux_net_null, SIGABRT);
    suite_add_tcase(s, tc_preconditions);

    /* Creation */
    suite_add_tcase(s, tc_creation);

    /* Funcionalidad */
    tcase_add_test(tc_functionality, test_dinic_aux_net);
    suite_add_tcase(s, tc_functionality);

    return s;
}

/* Para testing de memoria */
void dinic_memory_test(void){

}

#undef s
#undef t
