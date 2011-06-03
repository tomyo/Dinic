#include <check.h>
#include <signal.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "test_queue.h"
#include "queue.h"

Queue *q = NULL; /* Global queue for testing */

/* Base tests */
START_TEST(test_queue_free_null)
{
    queue_free(NULL);
}
END_TEST


START_TEST(test_queue_is_empty_null)
{
   queue_is_empty(NULL);
}
END_TEST

START_TEST(test_queue_get_length_null)
{
    queue_get_length(NULL);
}
END_TEST

START_TEST(test_queue_push_head_null)
{
    void *useless_data = malloc(20);
    queue_push_head(NULL, useless_data);
    free(useless_data);
}
END_TEST

/* Creation tests */
START_TEST(test_queue_create_destroy)
{
    q = queue_new();
    queue_free(q);
}
END_TEST

/* Functionality tests */
START_TEST(test_queue_push_pop_basic)
{
    int in[] = {0, 1, 2, 3};
    int *out = NULL;
    int i = 0;

    q = queue_new();
    queue_push_tail(q, (void *) in);
    out = (int *) queue_pop_head(q);

    for(i=0; i<4; i++){
        fail_unless(in[i] == out[i]);
    }

    queue_free(q);
}
END_TEST

START_TEST(test_queue_push_pop)
{
    int in[] = {0, 1, 2, 3, 4, 5};
    int *out = NULL;
    int i = 0;

    q = queue_new();

    for(i=0; i<6; i++){
        queue_push_tail(q, &in[i]);
    }

    for(i=0; i<6; i++){
        out = (int *) queue_pop_head(q);
        fail_unless(in[i] == *out);
    }

    queue_free(q);
}
END_TEST

START_TEST(test_queue_pop)
{
    int in[] = {1, 3, 3, 7};
    int *out = NULL;

    q = queue_new();
    queue_push_head(q, (void *) in);
    out = (int *) queue_pop_head(q);

    fail_unless(in == out);

    queue_free(q);
}
END_TEST

START_TEST(test_queue_length)
{
    int in[] = {3, 4711201, 1, 6, 440204, 5, 1337, 4213944};
    unsigned int i = 0;

    q = queue_new();

    for(i=0; i<8; i++){
        fail_unless(queue_get_length(q) == i);
        queue_push_tail(q, &in[i]);
    }

    fail_unless(queue_get_length(q) == 8);

    for(i=8; i>0; i--){
        fail_unless(queue_get_length(q) == i);
        queue_pop_head(q);
    }

    fail_unless(queue_get_length(q) == 0);

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
    tcase_add_test_raise_signal(tc_base, test_queue_free_null, SIGABRT);
    tcase_add_test_raise_signal(tc_base, test_queue_get_length_null, SIGABRT);
    tcase_add_test_raise_signal(tc_base, test_queue_push_head_null, SIGABRT);
    tcase_add_test_raise_signal(tc_base, test_queue_is_empty_null, SIGABRT);
    suite_add_tcase(s, tc_base);

    /* Creation cases */
    tcase_add_test(tc_creation, test_queue_create_destroy);
    suite_add_tcase(s, tc_creation);

    /* Functionality cases */
    tcase_add_test(tc_functionality, test_queue_push_pop_basic);
    tcase_add_test(tc_functionality, test_queue_push_pop);
    tcase_add_test(tc_functionality, test_queue_pop);
    tcase_add_test(tc_functionality, test_queue_length);
    suite_add_tcase(s, tc_functionality);

    return s;
}
