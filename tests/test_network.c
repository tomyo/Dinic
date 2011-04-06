#include <check.h>
#include "test_network.h"

#include <signal.h>
#include <assert.h>

#include "network.h"

Network *net = NULL; /* Para networks temporales */

/* Precondiciones */
START_TEST(test_destroy_null)
{
    network_destroy(NULL);
}
END_TEST

/* Armado de la test suite */
Suite *network_suite(void){
    Suite *s = suite_create("network");
    TCase *tc_preconditions = tcase_create("Precondition");
    TCase *tc_creation = tcase_create("Creation");
    TCase *tc_functionality = tcase_create("Functionality");

    /* Precondiciones */
    tcase_add_test_raise_signal(tc_preconditions, test_destroy_null, SIGABRT);
    suite_add_tcase(s, tc_preconditions);

    /* Creation */
    /* tcase_add_test(tc_creation, test_new_destroy); */
    suite_add_tcase(s, tc_creation);

    /* Funcionalidad */
    /* tcase_add_test(tc_functionality, test_get_weight); */
    suite_add_tcase(s, tc_functionality);

    return s;
}

/* Para testing de memoria */
void network_memory_test(void){
    /* Codigo que deberia correr sin memory leaks */
}
