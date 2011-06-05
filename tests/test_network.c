#include <check.h>
#include "test_network.h"

#include <signal.h>
#include <assert.h>
#include <stdlib.h>

#include "network.h"
#include "slist.h"

Network *net = NULL; /* Para networks temporales */

/* Precondiciones */
START_TEST(test_network_destroy_null)
{
    network_destroy(NULL);
}
END_TEST

START_TEST(test_network_add_edge_null)
{
    Edge *edge = NULL;
    unsigned int x1 = 0, x2 = 0;

    edge = edge_create(&x1, &x2, 1, 0);
    network_add_edge(NULL, edge);
    edge_destroy(edge);
}
END_TEST

START_TEST(test_network_add_edge_null_2)
{
    net = network_create();
    network_add_edge(net, NULL);
}
END_TEST

START_TEST(test_network_get_edge_null)
{
    unsigned int x1 = 42;
    network_get_edges(NULL, &x1);
}
END_TEST

START_TEST(test_network_neighbours_null)
{
    unsigned int x1 = 42;
    network_neighbours(NULL, &x1);
}
END_TEST

/* Crear y destruir */
START_TEST(test_network_new_destroy)
{
    net = network_create();
    network_destroy(net);
}
END_TEST

/* Testeo de funcionalidad */
START_TEST(test_network_get_edges)
{
    Edge *e1 = NULL, *e2 = NULL, *e3 = NULL, *etmp = NULL;
    SList *el = NULL, *tmp = NULL;
    Node ntmp = 0;
    unsigned int x1 = 0, x2 = 1;
    unsigned int x3 = 0, x4 = 2;
    unsigned int x5 = 2, x6 = 3;

    e1 = edge_create(&x1, &x2, 1, 0);
    e2 = edge_create(&x3, &x4, 1, 0);
    e3 = edge_create(&x5, &x6, 1, 0);

    net = network_create();
    network_add_edge(net, e1);
    network_add_edge(net, e2);
    network_add_edge(net, e3);

    el = network_get_edges(net, &x1);

    tmp = el;
    while(tmp != NULL){
        etmp = (Edge *) slist_nth_data(tmp, 0);
        ntmp = *edge_get_second(etmp);
        fail_unless(ntmp == 1 || ntmp == 2);

        tmp = slist_next(tmp);
    }

    network_destroy(net);
}
END_TEST

START_TEST(test_network_get_neightbours)
{
    Edge *e1 = NULL, *e2 = NULL, *e3 = NULL;
    Node *ntmp = NULL;
    SList *el = NULL, *tmp = NULL;
    unsigned int x1 = 2, x2 = 3;
    unsigned int x3 = 1, x4 = 4;
    unsigned int x5 = 2, x6 = 1;

    e1 = edge_create(&x1, &x2, 1, 0);
    e2 = edge_create(&x3, &x4, 1, 0);
    e3 = edge_create(&x5, &x6, 1, 0);

    net = network_create();
    network_add_edge(net, e1);
    network_add_edge(net, e2);
    network_add_edge(net, e3);

    el = network_neighbours(net, &x1);

    tmp = el;
    while(tmp != NULL){
        ntmp = (Node *) slist_nth_data(tmp, 0);

        fail_unless(*ntmp == 3 || *ntmp == 1);

        tmp = slist_next(tmp);
    }

    slist_free(el);
    network_destroy(net);
}
END_TEST

/* Armado de la test suite */
Suite *network_suite(void){
    Suite *s = suite_create("network");
    TCase *tc_preconditions = tcase_create("Precondition");
    TCase *tc_creation = tcase_create("Creation");
    TCase *tc_functionality = tcase_create("Functionality");

    /* Precondiciones */
    tcase_add_test_raise_signal(tc_preconditions, test_network_destroy_null, SIGABRT);
    tcase_add_test_raise_signal(tc_preconditions, test_network_add_edge_null, SIGABRT);
    tcase_add_test_raise_signal(tc_preconditions, test_network_add_edge_null_2, SIGABRT);
    tcase_add_test_raise_signal(tc_preconditions, test_network_get_edge_null, SIGABRT);
    tcase_add_test_raise_signal(tc_preconditions, test_network_neighbours_null, SIGABRT);
    suite_add_tcase(s, tc_preconditions);

    /* Creation */
    tcase_add_test(tc_creation, test_network_new_destroy);
    suite_add_tcase(s, tc_creation);

    /* Funcionalidad */
    tcase_add_test(tc_functionality, test_network_get_edges);
    tcase_add_test(tc_functionality, test_network_get_neightbours);
    suite_add_tcase(s, tc_functionality);

    return s;
}

/* Para testing de memoria */
void network_memory_test(void){
    /* Codigo que deberia correr sin memory leaks */

    Edge *e1 = NULL, *e2 = NULL, *e3 = NULL, *e4 = NULL, *etmp = NULL;
    Node *ntmp = NULL;
    SList *el = NULL, *nl = NULL, *tmp = NULL;
    unsigned int x1 = 0, x2 = 1;
    unsigned int x3 = 0, x4 = 2;
    unsigned int x5 = 2, x6 = 3;
    unsigned int x7 = 2, x8 = 0;

    e1 = edge_create(&x1, &x2, 1, 0);
    e2 = edge_create(&x3, &x4, 4, 0);
    e3 = edge_create(&x5, &x6, 6, 0);
    e4 = edge_create(&x7, &x8, 0, 0);

    net = network_create();
    network_add_edge(net, e1);
    network_add_edge(net, e2);
    network_add_edge(net, e3);
    network_add_edge(net, e4);

    el = network_get_edges(net, 0);

    tmp = el;
    while(tmp != NULL){
        etmp = (Edge *) slist_nth_data(tmp, 0);
        edge_pprint(etmp);
        tmp = slist_next(tmp);
    }

    nl = network_neighbours(net, &x5);
    printf("Vecinos del nodo: 2\n");

    tmp = nl;
    while(tmp != NULL){
        ntmp = (Node *) slist_nth_data(tmp, 0);
        tmp = slist_next(tmp);

        printf("  %d\n", *ntmp);
    }

    slist_free(nl);
    network_destroy(net);
}
