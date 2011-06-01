#include <check.h>
#include "test_queue.h"

#include <signal.h>
#include <assert.h>
#include <stdio.h>

#include "queue.h"

Queue *q = NULL; /* Global queue for testing */

/* Base tests */
START_TEST(test_queue_is_empty_null)
{
    queue_is_empty(NULL);
}
END_TEST

START_TEST(test_queue_create_destroy)
{
    q = queue_new();
    queue_free(q);
}
END_TEST

/* Armado de la test suite */
Suite *queue_suite(void){
    Suite *s = suite_create("queue");
    TCase *tc_base = tcase_create("Base");
    TCase *tc_creation = tcase_create("Creation");
    TCase *tc_functionality = tcase_create("Functionality");

    /* Base cases */
    tcase_add_test_raise_signal(tc_base, test_queue_is_empty_null, SIGABRT);
    suite_add_tcase(s, tc_base);

    /* Creation */
    tcase_add_test(tc_creation, test_queue_create_destroy);
    suite_add_tcase(s, tc_creation);

    /* Funcionalidad */
    /* tcase_add_test(tc_functionality, test_slist_length); */
    suite_add_tcase(s, tc_functionality);

    return s;
}
