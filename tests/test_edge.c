#include <check.h>
#include "test_edge.h"

#include <signal.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <limits.h>

#include "edge.h"

Edge *edge = NULL; /* Para crear edges temporales */

/* Testeo precondiciones. */
START_TEST(test_edge_destroy_null)
{
    edge_destroy(NULL);
}
END_TEST

START_TEST(test_edge_get_first_null)
{
    edge_get_first(NULL);
}
END_TEST

START_TEST(test_edge_get_second_null)
{
    edge_get_second(NULL);
}
END_TEST

START_TEST(test_edge_invalid_nodes)
{
    unsigned int x1 = 0, x2 = 0;

    /* No deberia tener una arista de un nodo a si mismo */
    edge = edge_create(&x1, &x2, 1, 0);
    edge_destroy(edge);
}
END_TEST

/* Crear y destruir */
START_TEST(test_edge_new_destroy)
{
    unsigned int x1 = 0, x2 = 1;

    edge = edge_create(&x1, &x2, 1, 0);
    edge_destroy(edge);
}
END_TEST

/* Testeo de funcionalidad */
START_TEST(test_edge_get_capacity)
{
    int w = 0;
    unsigned int x1 = 0, x2 = 1;

    edge = edge_create(&x1, &x2, 10, 0);
    w = edge_get_capacity(edge);

    fail_unless(w == 10);
}
END_TEST

START_TEST(test_edge_get_first_node)
{
    Node *node = NULL;
    unsigned int x1 = 13, x2 = 37;

    edge = edge_create(&x1, &x2, 1337, 0);

    node = edge_get_first(edge);

    fail_unless(*node == 13);
    edge_destroy(edge);
}
END_TEST

START_TEST(test_edge_get_second_node)
{
    Node *node = NULL;
    unsigned int x1 = 13, x2 = 37;

    edge = edge_create(&x1, &x2, 1337, 0);

    node = edge_get_second(edge);

    fail_unless(*node == 37);
    edge_destroy(edge);
}
END_TEST

START_TEST(test_edge_big_edge)
{
    Node *node = NULL;
    unsigned int x1 = 0, x2 = 0;

    x1 = UINT_MAX;
    edge = edge_create(&x1, &x2, 10, 0);

    node = edge_get_first(edge);

    fail_unless(*node == x1);
    edge_destroy(edge);
}
END_TEST


/* Armado de la test suite */
Suite *edge_suite(void){
    Suite *s = suite_create("edge");
    TCase *tc_preconditions = tcase_create("Precondition");
    TCase *tc_creation = tcase_create("Creation");
    TCase *tc_functionality = tcase_create("Functionality");

    /* Precondiciones */
    tcase_add_test_raise_signal(tc_preconditions, test_edge_invalid_nodes, SIGABRT);
    tcase_add_test_raise_signal(tc_preconditions, test_edge_destroy_null, SIGABRT);
    tcase_add_test_raise_signal(tc_preconditions, test_edge_get_first_null, SIGABRT);
    tcase_add_test_raise_signal(tc_preconditions, test_edge_get_second_null, SIGABRT);
    suite_add_tcase(s, tc_preconditions);

    /* Creation */
    tcase_add_test(tc_creation, test_edge_new_destroy);
    suite_add_tcase(s, tc_creation);

    /* Funcionalidad */
    tcase_add_test(tc_functionality, test_edge_get_capacity);
    tcase_add_test(tc_functionality, test_edge_get_first_node);
    tcase_add_test(tc_functionality, test_edge_get_second_node);
    tcase_add_test(tc_functionality, test_edge_big_edge);
    suite_add_tcase(s, tc_functionality);

    return s;
}

/* Para testing de memoria */
void edge_memory_test(void){
    /* Codigo que deberia correr sin memory leaks */
    Edge *edge = NULL, *edge2 = NULL;
    unsigned int *first = NULL, *second = 0;
    unsigned int capacity = 0;
    unsigned int x1 = 0, x2 = 0;
    unsigned int x3 = 0, x4 = 0;

    x1 = UINT_MAX;
    x2 = 14;
    x3 = 1;
    x4 = 2;

    edge = edge_create(&x1, &x2, 15, 0);
    edge2 = edge_create(&x3, &x4, 5, 0);

    first = edge_get_first(edge);
    capacity = edge_get_capacity(edge);

    second = edge_get_second(edge2);

    if(!edge_cmp(edge, edge2)){
        edge_pprint(edge);
        edge_pprint(edge2);
    }

    edge_destroy(edge);
    edge_destroy(edge2);
}
