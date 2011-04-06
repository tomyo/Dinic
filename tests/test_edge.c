#include <check.h>
#include "test_edge.h"

#include <signal.h>
#include <assert.h>
#include <string.h> /* para strcmp */
#include <stdlib.h> /* para calloc */
#include <stdio.h> /* para sprintf */

#include "edge.h"

Edge *edge = NULL; /* Para crear edges temporales */

/* Testeo precondiciones. */
START_TEST(test_destroy_null)
{
    edge_destroy(NULL);
}
END_TEST

START_TEST(test_get_first_null)
{
    edge_get_first(NULL);
}
END_TEST

START_TEST(test_get_second_null)
{
    edge_get_second(NULL);
}
END_TEST

START_TEST(test_increment_reference_null)
{
    edge_increment_reference(NULL);
}
END_TEST

START_TEST(test_invalid_nodes)
{
    /* No deberia tener una arista de un nodo a si mismo */
    edge = edge_create(0, 0, 1);
    edge_destroy(edge);
}
END_TEST

/* Crear y destruir */
START_TEST(test_new_destroy)
{
    edge = edge_create(0, 1, 1);
    edge_destroy(edge);
}
END_TEST

START_TEST(test_destroy_multiple_reference)
{
    edge = edge_create(3, 14, 159);
    edge_increment_reference(edge);
    edge_destroy(edge);
    edge_destroy(edge);
}
END_TEST

/* Testeo de funcionalidad */
START_TEST(test_get_weight)
{
    int w = 0;

    edge = edge_create(0, 1, 10);
    w = edge_get_weight(edge);

    fail_unless(w == 10);
}
END_TEST

START_TEST(test_get_first_node)
{
    Node *node = NULL;
    edge = edge_create(13, 37, 1337);

    node = edge_get_first(edge);

    fail_unless(*node == 13);
    edge_destroy(edge);
}
END_TEST

START_TEST(test_get_second_node)
{
    Node *node = NULL;
    edge = edge_create(13, 37, 1337);

    node = edge_get_second(edge);

    fail_unless(*node == 37);
    edge_destroy(edge);
}
END_TEST


/* Armado de la test suite */
Suite *edge_suite (void)
{
    Suite *s = suite_create("edge");
    TCase *tc_preconditions = tcase_create("Precondition");
    TCase *tc_creation = tcase_create("Creation");
    TCase *tc_functionality = tcase_create("Functionality");

    /* Precondiciones */
    tcase_add_test_raise_signal(tc_preconditions, test_invalid_nodes, SIGABRT);
    tcase_add_test_raise_signal(tc_preconditions, test_destroy_null, SIGABRT);
    tcase_add_test_raise_signal(tc_preconditions, test_get_first_null, SIGABRT);
    tcase_add_test_raise_signal(tc_preconditions, test_get_second_null, SIGABRT);
    tcase_add_test_raise_signal(tc_preconditions, test_increment_reference_null, SIGABRT);
    suite_add_tcase(s, tc_preconditions);

    /* Creation */
    tcase_add_test(tc_creation, test_new_destroy);
    tcase_add_test(tc_creation, test_destroy_multiple_reference);
    suite_add_tcase(s, tc_creation);

    /* Funcionalidad */
    tcase_add_test(tc_functionality, test_get_weight);
    tcase_add_test(tc_functionality, test_get_first_node);
    tcase_add_test(tc_functionality, test_get_second_node);
    suite_add_tcase(s, tc_functionality);

    return s;
}

/* Para testing de memoria */
void edge_memory_test (void) {
    /* Codigo que deberia correr sin memory leaks */
}

