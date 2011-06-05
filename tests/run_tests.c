#include <check.h>
#include <stdlib.h> /* Para tener EXIT_XXX */
#include "test_edge.h"
#include "test_network.h"
#include "test_list.h"
#include "test_queue.h"
#include "test_dinic.h"

int main (void)
{
    int number_failed;
    SRunner *sr = srunner_create(NULL);
    srunner_add_suite(sr, edge_suite());
    srunner_add_suite(sr, network_suite());
    srunner_add_suite(sr, slist_suite());
    srunner_add_suite(sr, queue_suite());
    srunner_add_suite(sr, dinic_suite());

    srunner_set_log(sr, "test.log");
    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

