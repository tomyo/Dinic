#include <check.h>
#include "test_edge.h"

#include <signal.h>
#include <assert.h>
#include <string.h> /* para strcmp */
#include <stdlib.h> /* para calloc */
#include <stdio.h> /* para sprintf */

#include "edge.h"


/* Testeo precondiciones. */

/* Crear y destruir */

/* Testeo de funcionalidad */

/* Armado de la test suite */
Suite *edge_suite (void)
{
	Suite *s = suite_create("edge");
	TCase *tc_preconditions = tcase_create("Precondition");
	TCase *tc_creation = tcase_create("Creation");
	TCase *tc_functionality = tcase_create("Functionality");

	/* Precondiciones */
	/* EJEMPLO: tcase_add_test_raise_signal (tc_preconditions, test_destroy_null, SIGABRT); */
	suite_add_tcase (s, tc_preconditions);

	/* Creation */
	/* EJEMPLO: tcase_add_test (tc_creation, test_new_destroy); */
	suite_add_tcase (s, tc_creation);

	/* Funcionalidad */
	/* EJEMPLO: tcase_add_test (tc_functionality, test_to_string); */
	suite_add_tcase (s, tc_functionality);

	return s;
}

/* Para testing de memoria */
void scommand_memory_test (void) {
    /* Codigo que deberia correr sin memory leaks */
}

