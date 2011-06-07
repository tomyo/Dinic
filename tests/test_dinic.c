#include <check.h>
#include <signal.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <stdlib.h>

#include "test_dinic.h"
#include "dinic.h"
#include "network.h"
#include "edge.h"
#include "defs.h"

/* Precondition tests */
START_TEST(test_dinic_aux_net_null)
{
    aux_network_new(NULL);

}
END_TEST

START_TEST(test_dinic_net_null)
{
    dinic(NULL, 0, 1, false);

}
END_TEST


/* Functionality test */
START_TEST(test_dinic_aux_net)
{
    Network *net = NULL, *aux_net = NULL;
    dinic_t *dt = NULL;
    Edge *e1 = NULL, *e2 = NULL, *e3 = NULL, *e4 = NULL;
    Edge *e5 = NULL, *e6 = NULL, *e7 = NULL, *e8 = NULL;
    SList *n1 = NULL;
    Node head = 0;
    Node s = 0, t = 1;

    /*
       Network a representar              |       Network auxiliar
                                          |
           2 — 3                          |              2 —
          /   / \                         |             /    \
         s   /    — t                     |            s    — 3 — t
          \ /       |                     |             \  /
           4 — 5  — 6                     |              4 —— 5 — 6
                                          |
       (todas las capacidades son 1)      |
    */

    /* Creo un network vacio */
    net = network_create();

    /* Creo los lados */
    e1 = edge_create(s, 2, 1, 0);
    e2 = edge_create(s, 4, 1, 0);
    e3 = edge_create(2, 3, 1, 0);
    e4 = edge_create(4, 3, 1, 0);
    e5 = edge_create(4, 5, 1, 0);
    e6 = edge_create(3, t, 1, 0);
    e7 = edge_create(5, 6, 1, 0);
    e8 = edge_create(6, t, 1, 0);

    /* Lleno el network */
    network_add_edge(net, e1);
    network_add_edge(net, e2);
    network_add_edge(net, e3);
    network_add_edge(net, e4);
    network_add_edge(net, e5);
    network_add_edge(net, e6);
    network_add_edge(net, e7);
    network_add_edge(net, e8);

    /* Completo la estructura con los datos */
    dt = (dinic_t *) calloc(1, sizeof(*dt));
    dt->network = net;
    dt->s = s;
    dt->t = t;

    /* Creo el network auxiliar */
    aux_net = aux_network_new(dt);

    /* Verifico los vecinos de s. Tienen que ser 2 y 4 */
    n1 = network_neighbours(aux_net, s);
    fail_unless(slist_length(n1) == 2);
    while (n1 != NULL) {
        head = *((Node *) slist_head_data(n1));
        fail_unless(head == 2 || head == 4);
        n1 = slist_next(n1);
    }
    slist_free(n1);

    /* Verifico los vecinos de 2. Tiene que ser solamente 3 */
    n1 = network_neighbours(aux_net, 2);
    fail_unless(slist_length(n1) == 1);
    head = *((Node *) slist_head_data(n1));
    fail_unless(head == 3);
    slist_free(n1);

    /* Verifico los vecinos de 4. Tienen que ser 3 y 5 */
    n1 = network_neighbours(aux_net, 4);
    fail_unless(slist_length(n1) == 2);
    while (n1 != NULL) {
        head = *((Node *) slist_head_data(n1));
        fail_unless(head == 3 || head == 5);
        n1 = slist_next(n1);
    }
    slist_free(n1);

    /* Verifico los vecinos de 3. Tiene que ser solamente t */
    n1 = network_neighbours(aux_net, 3);
    fail_unless(slist_length(n1) == 1);
    head = *((Node *) slist_head_data(n1));
    fail_unless(head == t);
    slist_free(n1);

    /* Verifico los vecinos de 5, tiene que ser 6 */
    n1 = network_neighbours(aux_net, 5);
    fail_unless(slist_length(n1) == 1);
    head = *((Node *) slist_head_data(n1));
    fail_unless(head == 6);

    /* Verifico los vecinos de t, tiene que ser una lista vacia */
    n1 = network_neighbours(aux_net, t);
    fail_unless(slist_is_empty(n1));

    /* Verifico los vecinos de 6, tiene que ser una lista vacia */
    n1 = network_neighbours(aux_net, 6);
    fail_unless(slist_is_empty(n1));
}
END_TEST

START_TEST(test_dinic_aux_net_2)
{
    Network *net = NULL, *aux_net = NULL;
    dinic_t *dt = NULL;
    Edge *e1 = NULL, *e2 = NULL, *e3 = NULL, *e4 = NULL;
    Edge *e5 = NULL, *e6 = NULL, *e7 = NULL, *e8 = NULL;
    SList *n1 = NULL;
    Node head = 0;
    Node s = 0, t = 1;

    /*
       Network a representar              |       Network auxiliar
                                          |
           2 — 3                          |              2 —
          /   / \ 0                       |             /    \
         s   /    — t                     |            s    — 3
          \ /       |                     |             \  /
           4 — 5  — 6                     |              4 —— 5 — 6 — t
                                          |
       Mismo network que en el test       |
       anterior pero con la arista        |
       3-t con capacidad 0 y el resto 1   |
    */

    /* Creo un network vacio */
    net = network_create();

    /* Creo los lados */
    e1 = edge_create(s, 2, 1, 0);
    e2 = edge_create(s, 4, 1, 0);
    e3 = edge_create(2, 3, 1, 0);
    e4 = edge_create(4, 3, 1, 0);
    e5 = edge_create(4, 5, 1, 0);
    e6 = edge_create(3, t, 0, 0);
    e7 = edge_create(5, 6, 1, 0);
    e8 = edge_create(6, t, 1, 0);

    /* Lleno el network */
    network_add_edge(net, e1);
    network_add_edge(net, e2);
    network_add_edge(net, e3);
    network_add_edge(net, e4);
    network_add_edge(net, e5);
    network_add_edge(net, e6);
    network_add_edge(net, e7);
    network_add_edge(net, e8);

    /* Completo la estructura con los datos */
    dt = (dinic_t *) calloc(1, sizeof(*dt));
    dt->network = net;
    dt->s = s;
    dt->t = t;

    /* Creo el network auxiliar */
    aux_net = aux_network_new(dt);

    /* Verifico los vecinos de s. Tienen que ser 2 y 4 */
    n1 = network_neighbours(aux_net, s);
    fail_unless(slist_length(n1) == 2);
    while (n1 != NULL) {
        head = *((Node *) slist_head_data(n1));
        fail_unless(head == 2 || head == 4);
        n1 = slist_next(n1);
    }
    slist_free(n1);

    /* Verifico los vecinos de 2. Tiene que ser solamente 3 */
    n1 = network_neighbours(aux_net, 2);
    fail_unless(slist_length(n1) == 1);
    head = *((Node *) slist_head_data(n1));
    fail_unless(head == 3);
    slist_free(n1);

    /* Verifico los vecinos de 4. Tienen que ser 3 y 5 */
    n1 = network_neighbours(aux_net, 4);
    fail_unless(slist_length(n1) == 2);
    while (n1 != NULL) {
        head = *((Node *) slist_head_data(n1));
        fail_unless(head == 3 || head == 5);
        n1 = slist_next(n1);
    }

    /* Verifico los vecinos de 3. Tiene que ser una lista vacia */
    n1 = network_neighbours(aux_net, 3);
    fail_unless(slist_is_empty(n1));

    /* Verifico los vecinos de 5, tiene que ser 6 */
    n1 = network_neighbours(aux_net, 5);
    fail_unless(slist_length(n1) == 1);
    head = *((Node *) slist_head_data(n1));
    fail_unless(head == 6);

    /* Verifico los vecinos de 6, tiene que ser solamente t */
    n1 = network_neighbours(aux_net, 6);
    fail_unless(slist_length(n1) == 1);
    head = *((Node *) slist_head_data(n1));
    fail_unless(head == t);

    /* Verifico los vecinos de t, tiene que ser una lista vacia */
    n1 = network_neighbours(aux_net, t);
    fail_unless(slist_is_empty(n1));
}
END_TEST

START_TEST(test_dinic_aux_net_backward)
{
    Network *net = NULL, *aux_net = NULL;
    dinic_t *dt = NULL;
    Edge *e1 = NULL, *e2 = NULL, *e3 = NULL;
    Edge *e4 = NULL, *e5 = NULL, *e6 = NULL;
    SList *n1 = NULL;
    Edge *head = NULL;
    Node s = 0, t = 1;

    /*
       Network a representar              |       Network auxiliar
                                          |
           4 ——————                       |
          /    \    \                     |
         s      3 —— t                    |      s —— 2 —— 3 —<— 4 —— t
          \    /                          |
           2 —                            |

       Capacidades:
        x1_|__x2_|__flow_|__cap
        s  |  4  |   3   |   3
        s  |  2  |   0   |   5
        4  |  3  |   3   |   5
        4  |  t  |   0   |   5
        2  |  3  |   0   |   5
        3  |  t  |   3   |   3
    */

    /* Creo un network vacio */
    net = network_create();

    /* Creo los lados forward */
    e1 = edge_create(s, 4, 3, 3); /* Tambien Backward */
    e2 = edge_create(s, 2, 5, 0);
    e3 = edge_create(4, 3, 5, 3); /* Tambien Backward */
    e4 = edge_create(4, t, 5, 0);
    e5 = edge_create(2, 3, 5, 0);
    e6 = edge_create(3, t, 3, 3); /* Tambien Backward */

    /* Lleno el network con los lados forward */
    network_add_edge(net, e1);
    network_add_edge(net, e2);
    network_add_edge(net, e3);
    network_add_edge(net, e4);
    network_add_edge(net, e5);
    network_add_edge(net, e6);
    /* Lleno los backwards */
    network_add_edge_backward(net, e1);
    network_add_edge_backward(net, e3);
    network_add_edge_backward(net, e6);

    /* Completo la estructura con los datos */
    dt = (dinic_t *) calloc(1, sizeof(*dt));
    dt->network = net;
    dt->s = s;
    dt->t = t;

    /* Creo el network auxiliar */
    aux_net = aux_network_new(dt);

    /* Verifico los vecinos de s, tiene que ser solamente 2 */
    n1 = network_get_edges(aux_net, s);
    fail_unless(slist_length(n1) == 1);
    head = (Edge *) slist_head_data(n1);
    fail_unless(*edge_get_second(head) == 2);

    /* Verifico los vecinos de 2, tiene que ser solamente 3 */
    n1 = network_get_edges(aux_net, 2);
    fail_unless(slist_length(n1) == 1);
    head = (Edge *) slist_head_data(n1);
    fail_unless(*edge_get_second(head) == 3);

    /* Verifico los vecinos de 3, tiene que ser 1 y como backward */
    n1 = network_get_edges(aux_net, 3);
    fail_unless(slist_length(n1) == 1);
    head = (Edge *) slist_head_data(n1);
    fail_unless(*edge_get_first(head) == 4);
    fail_unless(*edge_get_second(head) == 3);

    /* Verifico los vecinos de 4, tiene que ser solamente t */
    n1 = network_get_edges(aux_net, 4);
    fail_unless(slist_length(n1) == 1);
    head = (Edge *) slist_head_data(n1);
    fail_unless(*edge_get_second(head) == t);

    /* Verifico los vecinos de t, tiene que ser una lista vacia */
    n1 = network_get_edges(aux_net, t);
    fail_unless(slist_is_empty(n1));
}
END_TEST

START_TEST(test_dinic_aux_net_no_t)
{
    Network *net = NULL, *aux_net = NULL;
    dinic_t *dt = NULL;
    Edge *e1 = NULL, *e2 = NULL, *e3 = NULL, *e4 = NULL;
    SList *n1 = NULL, *min_cut = NULL;
    Node head = 0;
    Node s = 0, t = 1;

    /*
       Network a representar              |       Network auxiliar
                                          |
           2                              |        2
          / \                             |       / \
         s   4                            |      s   4
          \ /                             |       \ /
           3                              |        3

       Capacidades:
        x1_|__x2_|__flow_|__cap
        s  |  2  |   0   |   1
        s  |  3  |   0   |   2
        2  |  4  |   0   |   3
        3  |  4  |   0   |   4
    */

    /* Creo un network vacio */
    net = network_create();

    /* Creo los lados forward */
    e1 = edge_create(s, 2, 1, 0);
    e2 = edge_create(s, 3, 2, 0);
    e3 = edge_create(2, 4, 3, 0);
    e4 = edge_create(3, 4, 4, 0);

    /* Lleno el network con los lados forward */
    network_add_edge(net, e1);
    network_add_edge(net, e2);
    network_add_edge(net, e3);
    network_add_edge(net, e4);

    /* Completo la estructura con los datos */
    dt = (dinic_t *) calloc(1, sizeof(*dt));
    dt->network = net;
    dt->s = s;
    dt->t = t; /* Not used. Not tested */

    /* Creo el network auxiliar */
    aux_net = aux_network_new(dt);

    /* Verifico los vecinos de s, tiene que ser 2 y 3 */
    n1 = network_neighbours(aux_net, s);
    fail_unless(slist_length(n1) == 2);
    while (n1 != NULL) {
        head = *((Node *) slist_head_data(n1));
        fail_unless(head == 2 || head == 3);
        n1 = slist_next(n1);
    }

    /* Verifico los vecinos de 2, tiene que ser solamente 4 */
    n1 = network_neighbours(aux_net, 2);
    fail_unless(slist_length(n1) == 1);
    head = *(Node *) slist_head_data(n1);
    fail_unless(head == 4);

    /* Verifico los vecinos de 3, tiene que ser solamente 4 */
    n1 = network_neighbours(aux_net, 3);
    fail_unless(slist_length(n1) == 1);
    head = *(Node *) slist_head_data(n1);
    fail_unless(head == 4);

    /* Verifico los vecinos de 4, tiene que ser una lista vacia */
    n1 = network_neighbours(aux_net, 4);
    fail_unless(slist_is_empty(n1));

    /* Verifico el que sea el corte minimo */
    min_cut = network_nodes(net);
    fail_unless(min_cut != NULL);
    fail_unless(slist_length(min_cut) == 4);
    while (min_cut != NULL)
    {
        fail_unless(*((Node *)slist_head_data(min_cut)) <= 4);
        fail_unless(*((Node *)slist_head_data(min_cut)) != t);
        min_cut = slist_next(min_cut);
    }
}
END_TEST

START_TEST(test_dinic_aux_net_find_flow)
{
    Network *net = NULL;
    dinic_t *dt = NULL;
    Edge *e1 = NULL, *e2 = NULL, *e3 = NULL;
    Edge *e4 = NULL, *e5 = NULL, *e6 = NULL, *e7 = NULL, *e8 = NULL;
    Edge *e9 = NULL, *e10 = NULL, *e11 = NULL, *e12 = NULL, *e13 = NULL;
    Node s = 0, t = 1;
    DinicFlow *flow = NULL;
    SList *path = NULL;
    Edge *current_edge = NULL;
    unsigned int sequence[] = {0, 4, 5, 10, 1};
    int expected = 0;


    /*
       Network a representar

           2
          /  \
         s    3 —— 10 —— t    11
          \  /   /          /
           4 —— 5 — 6 — 7 — 8 — 9

        Todas las capacidades son 1
    */

    /* Creo un network vacio */
    net = network_create();

    /* Creo los lados */
    e1 = edge_create(s, 2, 1, 0);
    e3 = edge_create(2, 3, 1, 0);
    e10 = edge_create(7, 8, 1, 0);
    e5 = edge_create(5, 6, 1, 0);
    e8 = edge_create(10, t, 1, 0);
    e2 = edge_create(s, 4, 1, 0);
    e9 = edge_create(6, 7, 1, 0);
    e6 = edge_create(5, 10, 1, 0);
    e4 = edge_create(4, 3, 1, 0);
    e11 = edge_create(8, 9, 1, 0);
    e7 = edge_create(4, 5, 1, 0);
    e12 = edge_create(8, 11, 1, 0);
    e13 = edge_create(3, 10, 0, 0);

    network_add_edge(net, e6);
    network_add_edge(net, e5);
    network_add_edge(net, e2);
    network_add_edge(net, e4);
    network_add_edge(net, e10);
    network_add_edge(net, e7);
    network_add_edge(net, e12);
    network_add_edge(net, e11);
    network_add_edge(net, e1);
    network_add_edge(net, e8);
    network_add_edge(net, e3);
    network_add_edge(net, e9);
    network_add_edge(net, e13);


    /* Completo la estructura con los datos */
    dt = (dinic_t *) calloc(1, sizeof(*dt));
    dt->network = net;
    dt->s = s;
    dt->t = t;
    flow = aux_network_find_flow(dt, net, false);

    path = flow->path;
    fail_unless(!slist_is_empty(path));

    while (path != NULL) {
        current_edge = slist_head_data(path);

        fail_unless(*edge_get_first(current_edge) == sequence[expected]);

        expected += 1;
        path = slist_next(path);
    }

    fail_unless(expected == 4, "La secuencia termino antes?");
}
END_TEST


START_TEST(test_dinic_net_simple)
{
    Network *net = NULL;
    dinic_result *result = NULL;
    Node s = 0, t = 1;
    Edge *single = NULL;
    bool verbose_mode = false; /* EDITME */

    /* Network a crear
             s —> t
     */

    net = network_create();
    single = edge_create(s, t, 1, 0);
    network_add_edge(net, single);
    result = dinic(net, s, t, verbose_mode);

    fail_unless(result != NULL);
    fail_unless(result->flow_value == 1);

}
END_TEST

START_TEST(test_dinic_net_small)
{
    Network *net = NULL;
    dinic_result *result = NULL;
    Edge *e1 = NULL, *e2 = NULL, *e3 = NULL, *e4 = NULL;
    Edge *e5 = NULL, *e6 = NULL, *e7 = NULL;
    Node s = 0, t = 1;
    
    bool verbose_mode = false; /* EDITME */

    /*
       Network a representar

           ,2
          / |
         s——3——t
          \ |
           `4——5
        Todas las capacidades son 1 menos 3——t = 3

       NA 1
          ,2
         /
        s——3——t
         \
          `4——5
       FLUJOS: s-3-t:1 (saturo s-3)

       NA 2
          ,2
         /  \
        s    ——3——t
         \  /
          `4———5
       FLUJOS: s-2-3-t: 1 (saturo s-2 y 2-3)
               s-4-3-t: 1 (saturo s-4 , 4-3 y 3-t)

      NA 3
      s  (no se imprime)

    */
    net = network_create();

    /* Creo los lados forward */
    e1 = edge_create(s, 2, 1, 0);
    e2 = edge_create(s, 3, 1, 0);
    e3 = edge_create(s, 4, 1, 0);
    e4 = edge_create(2, 3, 1, 0);
    e5 = edge_create(4, 3, 1, 0);
    e6 = edge_create(4, 5, 1, 0);
    e7 = edge_create(3, t, 3, 0);


    /* Lleno el network con los lados forward */
    network_add_edge(net, e1);
    network_add_edge(net, e2);
    network_add_edge(net, e3);
    network_add_edge(net, e4);
    network_add_edge(net, e5);
    network_add_edge(net, e6);
    network_add_edge(net, e7);

    result = dinic(net, s, t, verbose_mode);


    fail_unless(result != NULL);
    {
        SList *edges = NULL;
        
        edges = result->max_flow;
        while(edges != NULL) {
            edge_pprint(slist_head_data(edges));
            
            edges = slist_next(edges);
     }
    }
    fail_unless(result->flow_value == 3);
    fail_unless(slist_length(result->max_flow) == 7);
}
END_TEST

/* Armado de la test suite */
Suite *dinic_suite(void){
    Suite *s = suite_create("dinic");
    TCase *tc_preconditions = tcase_create("Precondition");
    TCase *tc_creation = tcase_create("Creation");
    TCase *tc_functionality = tcase_create("Functionality");

    /* Precondiciones */
    tcase_add_test_raise_signal(tc_preconditions, test_dinic_aux_net_null, SIGABRT);
    tcase_add_test_raise_signal(tc_preconditions, test_dinic_net_null, SIGABRT);
    suite_add_tcase(s, tc_preconditions);

    /* Creation */
    suite_add_tcase(s, tc_creation);

    /* Funcionalidad */
    tcase_add_test(tc_functionality, test_dinic_aux_net);
    tcase_add_test(tc_functionality, test_dinic_aux_net_2);
    tcase_add_test(tc_functionality, test_dinic_aux_net_backward);
    tcase_add_test(tc_functionality, test_dinic_aux_net_no_t);
    tcase_add_test(tc_functionality, test_dinic_aux_net_find_flow);
    tcase_add_test(tc_functionality, test_dinic_net_simple);
    tcase_add_test(tc_functionality, test_dinic_net_small);
    suite_add_tcase(s, tc_functionality);

    return s;
}

/* Para testing de memoria */
void dinic_memory_test(void){

}
