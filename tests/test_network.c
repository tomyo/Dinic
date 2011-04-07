#include <check.h>
#include "test_network.h"

#include <signal.h>
#include <assert.h>

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
    /* tcase_add_test(tc_functionality, test_get_weight); */
    suite_add_tcase(s, tc_functionality);

    return s;
}

/* Para testing de memoria */
void network_memory_test(void){
    /* Codigo que deberia correr sin memory leaks */

    net = network_create();
    network_destroy(net);
}
