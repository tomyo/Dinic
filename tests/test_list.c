#include <check.h>
#include "test_list.h"

#include <signal.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>

#include "slist.h"

#define SIZE 8 /* Size > 5 */

/* Base tests. */
START_TEST(test_slist_free_1_null)
{
    slist_free_1(NULL);
}
END_TEST

START_TEST(test_slist_free_null)
{
    slist_free(NULL);
}
END_TEST

START_TEST(test_slist_insert_out_of_index)
{
    slist_insert(NULL, NULL, 1);
}
END_TEST

START_TEST(test_slist_nth_out_of_index)
{
    /* It should return an empty list */
    slist_nth(NULL, 5);
}
END_TEST

START_TEST(test_slist_find_null)
{
    int dummy = 9;
    slist_find(NULL, &dummy);
}
END_TEST

START_TEST(test_slist_index_null)
{
    int dummy = 9;
    slist_index(NULL, &dummy);
}
END_TEST

START_TEST(test_slist_last_NULL)
{
    slist_last(NULL);
}
END_TEST

START_TEST(test_slist_length_NULL)
{
    fail_unless(slist_length(NULL) == 0);
}
END_TEST

START_TEST(test_slist_nth_data_NULL)
{
    slist_nth_data(NULL, 1);
}
END_TEST

/* Crear y destruir */

START_TEST(test_slist_create_destroy_basic)
{
    SList *list = NULL;
    int times = 9, i;

    for(i=0; i<times; i++) {
        list = slist_prepend(list, &i);
    }

    slist_free(list);
    list = NULL;

    for(i=0; i<times; i++) {
        list = slist_append(list, &i);
    }

    slist_free(list);
}
END_TEST

/* Testeo de funcionalidad */
START_TEST(test_slist_append)
{
    int array[SIZE], iter;
    SList *list = NULL;

    for(iter = 0; iter< SIZE; iter++) {
        array[iter] = iter;
        list = slist_append(list, &array[iter]);
    }

    fail_unless(slist_length(list) == SIZE);

    iter = 0;
    while(list!= NULL) {
        fail_unless(*(int *)(list->data) == array[iter]);
        fail_unless(iter < SIZE);
        list = slist_next(list);
        iter++;
    }

    slist_free(list);
}
END_TEST

START_TEST(test_slist_prepend)
{
    int array[SIZE], iter;
    SList *list = NULL;

    for(iter = 0; iter< SIZE; iter++) {
        array[iter] = iter;
        list = slist_prepend(list, &array[iter]);
    }

    fail_unless(slist_length(list) == SIZE);

    iter = SIZE-1;
    while(list!= NULL) {
        fail_unless(*(int *)(list->data) == array[iter]);
        fail_unless(iter >= 0);
        list = slist_next(list);
        iter--;
    }

    slist_free(list);
}
END_TEST

START_TEST(test_slist_insert)
{
    int array[SIZE], iter, insert1 = 183, insert2 = 147;
    SList *list = NULL;

    for(iter = 0; iter< SIZE; iter++) {
        array[iter] = iter;
        list = slist_append(list, &array[iter]);
    }

    fail_unless(slist_length(list) == SIZE);
    list = slist_insert(list, &insert1, 4);
    fail_unless(slist_length(list) == SIZE+1);
    list = slist_insert(list, &insert2, 4);
    fail_unless(slist_length(list) == SIZE+2);
    iter = 0;

    while(iter < 4) {
        list = slist_next(list);
        iter++;
    }
    fail_unless(*(int *)(list->data) == insert2);
    list = slist_next(list);
    fail_unless(*(int *)(list->data) == insert1);
    list = slist_next(list);
    fail_unless(*(int *)(list->data) == array[4]);

    slist_free(list);
}
END_TEST

START_TEST(test_slist_concat)
{
    SList *l1 = NULL, *l2 = NULL;
    int array[SIZE], iter;

    for(iter = 0; iter < SIZE; iter++) {
        l1 = slist_append(l1, &array[iter]);
    }

    fail_unless(slist_length(l1) == SIZE);

    for(iter = 0; iter < SIZE - 3; iter++) {
        l2 = slist_append(l2, &array[iter]);
    }

    fail_unless(slist_length(l2) == SIZE - 3);

    l1 = slist_concat(l1, l2);

    fail_unless(slist_length(l1) == SIZE + SIZE -3);

    slist_free(l1);
}
END_TEST

START_TEST(test_slist_reverse)
{
    int array[SIZE], iter = 0;
    SList *list = NULL;

    for(iter=0;iter<SIZE;iter++) {
        array[iter] = iter;
        list = slist_append(list, &array[iter]);
    }

    fail_unless(slist_length(list) == SIZE);

    list = slist_reverse(list);

    iter = SIZE-1;
    while(list != NULL) {
        fail_unless(*(int *)(list->data) == array[iter]);
        fail_unless(iter >= 0);
        list = slist_next(list);
        iter--;
    }

    slist_free(list);
}
END_TEST

START_TEST(test_slist_nth)
{
    int array[SIZE], iter;
    SList *list = NULL;

    for(iter = 0; iter< SIZE; iter++) {
        array[iter] = iter;
        list = slist_append(list, &array[iter]);
    }

    fail_unless(slist_length(list) == SIZE);

    list = slist_nth(list, 4);

    fail_unless(slist_length(list) == SIZE - 4);

    iter = 4;
    while(list!= NULL) {
        fail_unless(*(int *)(list->data) == array[iter]);
        fail_unless(iter < SIZE);
        list = slist_next(list);
        iter++;
    }

    slist_free(list);
}
END_TEST

/*
START_TEST(test_slist_index)
{
int array[SIZE], iter;
    SList *list = NULL;
    int *a;

    for(iter = 0; iter< SIZE; iter++) {
        array[iter] = iter;
        list = slist_append(list, &array[iter]);
    }

    fail_unless(slist_length(list) == SIZE);
    a = slist_index(list, 4);
    fail_unless(*a == array[4]);

    slist_free(list);
}
END_TEST
*/

START_TEST(test_slist_length)
{
    SList *list = NULL;
    int iter, array[SIZE];

    for(iter = 0; iter< SIZE; iter++) {
        array[iter] = iter;
        list = slist_append(list, &array[iter]);
    }

    fail_unless(slist_length(list) == SIZE);

    slist_free(list);
}
END_TEST



/* Armado de la test suite */
Suite *slist_suite(void){
    Suite *s = suite_create("slist");
    TCase *tc_base = tcase_create("Base");
    TCase *tc_creation = tcase_create("Creation");
    TCase *tc_functionality = tcase_create("Functionality");

    /* Base cases */
    tcase_add_test(tc_base, test_slist_free_1_null);
    tcase_add_test(tc_base, test_slist_free_null);
    tcase_add_test_raise_signal(tc_base, test_slist_insert_out_of_index, SIGABRT);
    tcase_add_test(tc_base, test_slist_nth_out_of_index);
    tcase_add_test(tc_base, test_slist_find_null);
    tcase_add_test(tc_base, test_slist_last_NULL);
    tcase_add_test(tc_base, test_slist_length_NULL);
    tcase_add_test_raise_signal(tc_base, test_slist_nth_data_NULL, SIGABRT);
    tcase_add_test(tc_base, test_slist_index_null);
    suite_add_tcase(s, tc_base);

    /* Creation */
    tcase_add_test(tc_creation, test_slist_create_destroy_basic);
    suite_add_tcase(s, tc_creation);

    /* Funcionalidad */
    tcase_add_test(tc_functionality, test_slist_append);
    tcase_add_test(tc_functionality, test_slist_prepend);
    tcase_add_test(tc_functionality, test_slist_insert);
    tcase_add_test(tc_functionality, test_slist_concat);
    tcase_add_test(tc_functionality, test_slist_reverse);
    tcase_add_test(tc_functionality, test_slist_nth);
    /*tcase_add_test(tc_functionality, test_slist_index);*/
    tcase_add_test(tc_functionality, test_slist_length);
    suite_add_tcase(s, tc_functionality);

    return s;
}
