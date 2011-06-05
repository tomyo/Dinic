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

/* Precondition tests */
START_TEST(test_dinic_aux_net_null)
{
    aux_network_new(NULL);

}
END_TEST

/* Functionality test */
START_TEST(test_dinic_aux_net)
{
    Network *net = NULL, *backwards = NULL, *aux_net = NULL;
    dinic_t *dt = NULL;
    unsigned int s = 0, t = 1;
    unsigned int x2 = 2, x3 = 3, x4 = 4, x5 = 5, x6 = 6;
    Edge *e1 = NULL, *e2 = NULL, *e3 = NULL, *e4 = NULL;
    Edge *e5 = NULL, *e6 = NULL, *e7 = NULL, *e8 = NULL;
    SList *n1 = NULL;

    /*
       Network a representar

           2 — 3
          /   / \
         s   /    — t
          \ /       |
           4 — 5  — 6

       (todas las capacidades son 1)
    */


    /* Creo un network vacio */
    net = network_create();
    backwards = network_create();

    /* Creo los lados */
    e1 = edge_create(&s, &x2, 1, 0);
    e2 = edge_create(&s, &x4, 1, 0);
    e3 = edge_create(&x2, &x3, 1, 0);
    e4 = edge_create(&x4, &x3, 1, 0);
    e5 = edge_create(&x4, &x5, 1, 0);
    e6 = edge_create(&x3, &t, 1, 0);
    e7 = edge_create(&x5, &x6, 1, 0);
    e8 = edge_create(&x6, &t, 1, 0);

    /* Lleno el network */
    network_add_edge(net, e1);
    network_add_edge(net, e2);
    network_add_edge(net, e3);
    network_add_edge(net, e4);
    network_add_edge(net, e5);
    network_add_edge(net, e6);
    network_add_edge(net, e7);
    network_add_edge(net, e8);

    n1 = network_get_edges(aux_net, &s);

    /* Completo la estructura con los datos */
    dt = (dinic_t *) calloc(1, sizeof(*dt));
    dt->network = net;
    dt->s = &s;
    dt->t = &t;
    dt->backwards = backwards;
    dt->result = NULL; /* Not used. Not tested */

    /* Creo el network auxiliar */
    aux_net = aux_network_new(dt);

    /* Me fijo los vecinos de s */
    n1 = network_get_edges(aux_net, &s);
    fail_unless(!slist_is_empty(n1)); /* No puede estar vacia */

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
