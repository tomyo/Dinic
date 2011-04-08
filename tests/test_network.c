#include <check.h>
#include "test_network.h"

#include <signal.h>
#include <assert.h>
#include <stdlib.h>

#include "network.h"

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
    edge = edge_create(0, 0, 1);
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
    network_get_edges(NULL, 42);
}
END_TEST

START_TEST(test_network_neighbours_null)
{
    network_neighbours(NULL, 42);
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
    GList *es = NULL, *tmp = NULL;
    unsigned int i = 0;

    e1 = edge_create(0, 1, 1);
    e2 = edge_create(0, 2, 1);
    e3 = edge_create(2, 3, 1);

    net = network_create();
    network_add_edge(net, e1);
    network_add_edge(net, e2);
    network_add_edge(net, e3);

    es = network_get_edges(net, 0);

    tmp = es;
    i = 1;
    while(tmp != NULL){
        etmp = (Edge *) g_list_nth_data(tmp, 0);

        fail_unless(*edge_get_second(etmp) == i);
        i++;

        tmp = g_list_next(tmp);
    }

    network_destroy(net);
}
END_TEST

START_TEST(test_network_get_neightbours)
{
    Edge *e1 = NULL, *e2 = NULL, *e3 = NULL;
    Node *ntmp = NULL;
    GList *es = NULL, *tmp = NULL;

    e1 = edge_create(2, 3, 1);
    e2 = edge_create(1, 4, 1);
    e3 = edge_create(2, 1, 1);

    net = network_create();
    network_add_edge(net, e1);
    network_add_edge(net, e2);
    network_add_edge(net, e3);

    es = network_neighbours(net, 2);

    tmp = es;
    while(tmp != NULL){
        ntmp = (Node *) g_list_nth_data(tmp, 0);

        fail_unless(*ntmp == 3 || *ntmp == 1);

        tmp = g_list_next(tmp);
    }

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

    Edge *e1 = NULL, *e2 = NULL, *e3 = NULL, *etmp = NULL;
    GList *es = NULL, *tmp = NULL;

    e1 = edge_create(0, 1, 1);
    e2 = edge_create(0, 2, 1);
    e3 = edge_create(2, 3, 1);

    net = network_create();
    network_add_edge(net, e1);
    network_add_edge(net, e2);
    network_add_edge(net, e3);

    es = network_get_edges(net, 0);

    tmp = es;
    while(tmp != NULL){
        etmp = (Edge *) g_list_nth_data(tmp, 0);
        edge_pprint(etmp);
        tmp = g_list_next(tmp);
    }

    network_destroy(net);
}
