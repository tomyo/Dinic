#include <assert.h>
#include <stdlib.h>
#include <limits.h>

#include "dinic.h"
#include "network.h"
#include "edge.h"
#include "node.h"
#include "queue.h"
#include "stack.h"
#include "defs.h"


/* ******************* Funciones internas ******************** */

bool aux_network_find_blocking_flow(dinic_t *, Network *, bool);
static int compare_nodes(const void *x1, const void *x2);


/**
 * @brief Comprara dos edges segun su valor.
 * Devuelve 0 si son distintos y 1 si son iguales
 */
static int compare_nodes(const void *x1, const void *x2) {
    return ((*(const Node *)x1) == (*(const Node *)x2))?0:1;
}

/**
 * @brief Determina si se puede mandar flujo o devolverlo por edge.
 * @param edge el lado a examinar
 * @param mode 'f' si quiere mandar flujo o 'b' si quiero devolverlo
 */
static bool can_send_flow(Edge *edge, char mode) {
    /* Asumimos que primero se pregunta por los lados fordward */
    bool result = false;

    assert(mode == 'f' || mode == 'b');

    if (mode == 'f') {
        /* Podemos mandar flujo? */
        result = ((edge_get_capacity(edge) - edge_get_flow(edge)) > 0);
    } else {
        /* Podemos devolver flujo? */
        /* Por construccion, no deberiamos tener lados backwards que no se
         * pueden usar */
        result = true;
    }

    return result;
}

/* ************************ Funciones ************************ */

Network *aux_network_new(dinic_t *data) {
    Network *main_network = NULL, *result = NULL;
    Queue *bfs_queue = NULL, *next_level = NULL;
    Node *current_node = NULL;
    Node *s = NULL, *t = NULL;
    SList *edges = NULL;
    bool is_t_found = false;

    /* Precondicion */
    assert(data != NULL);

    main_network = data->network;
    s = data->s;
    t = data->t;

    /* Precondicion */
    assert(network_has_node(data->network, *s));

    /* Algoritmo Basicos:
     * 1) Revisar bfs_queue:
     *        a) Si esta vacia, terminamos -> Devolver Netwok Auxiliar (result)
     *        b) Si esta t, terminamos -> Devolver Netwok Auxiliar (result)
     *
     * 2) Poner aristas vecinas backward y forward en *result* y en la cola
     *    de nodos del proximo nivel segun corresponda (*Lea Dinic) por cada
     *    nodo de la cola de BFS.
     *
     * 3) Una vez vacia la cola BFS, Reemplazarla por la cola de nodos del
     *    siguiente nivel creada en 2) (nueva cola a iterar) -> GOTO 1).
     */

    /* Referencias a datos de *data* que vamos a necesitar constantemente */


    /* Inicializando variables */
    result = network_aux_create(); /* NA (vacio) */
    bfs_queue = queue_new();   /* BFS queue (vacia) */
    next_level = queue_new(); /* Siguiente nivel queue (vacia) */

    queue_push_head(bfs_queue, s); /* Empezamos desde *s* */
    while (!queue_is_empty(bfs_queue) && !is_t_found) {
        SList *current_edges = NULL;
        Edge *current_edge = NULL;
        Node *neighbour = NULL;

        /* 2 */
        while (!queue_is_empty(bfs_queue)) {
            current_node = queue_pop_head(bfs_queue);
            edges = network_get_edges(main_network, *current_node);
            /* edges tiene todas las posibles aristas del proximo nivel */
            current_edges = edges;

            while (current_edges != NULL) {
                char mode = 'f';

                current_edge = slist_head_data(current_edges);

                if (*edge_get_first(current_edge) == *current_node) {
                    mode = 'f';
                    neighbour = edge_get_second(current_edge);
                } else {
                    mode = 'b';
                    neighbour = edge_get_first(current_edge);
                }

                /* Ver si corresponde agregar la arista al NA (result) */
                if (!network_has_node(result, *neighbour)) {
                    /* Si no esta en el network, solo me tengo que fijar que
                     * puedo mandar o devolver flujo */
                    if (can_send_flow(current_edge, mode)) {
                        _network_add_edge(result, current_edge, mode);
                        queue_push_head(next_level, neighbour);
                        if (*neighbour == *t) {
                            is_t_found = true;
                        }
                    }
                } else {
                    /* Solo la agregamos si pertenece al nivel siguiente */
                    if (queue_find_custom(next_level, neighbour, compare_nodes)
                        && can_send_flow(current_edge, mode)) {
                        _network_add_edge(result, current_edge, mode);
                    }
                }
                current_edges = slist_next(current_edges);

            }

            /* Revisamos (y agregamos segun correspondia) todas las
             * arista que contenian los vecinos de current_node */
            /* No hay que liberar esto, es parte de network */
            /* slist_free(edges); */
        }

        /* 3 */
        queue_free(bfs_queue); /* supuestamente vacia */
        bfs_queue = next_level;
        next_level = queue_new();
    }
    queue_free(bfs_queue);
    queue_free(next_level);

    /* Post Condiccion */
    /*assert(network_has_node(result, s)); -> chota implementacion*/

    return result;
}

/* Funcion Auxiliar de DinicFlow, para no "ensuciar" a la funcion antes
 * mencionada.
 */
static void flow_pretty_print(dinic_t *data, DinicFlow *to_print) {
    Node *next_node = data->s;
    SList *path = to_print->path;

    /* Si la lista es vacia, no habia camino */
    if (path != NULL) {
        printf("0");
        while (path != NULL) {
            /* Calcular el valor del flujo de current_flow) */
            Edge *edge = slist_head_data(path);
            if(*edge_get_first(edge) == *next_node) {
                /* Forward */
                printf(" %u", *edge_get_second(edge));
                next_node = edge_get_second(edge);
            } else {
                /* Backward */
                printf(" <- %u", *edge_get_first(edge));
                next_node = edge_get_first(edge);
            }
            path = slist_next(path);
        }
        printf(" (flujo transportado: %u)\n", to_print->flow_value);
    }
}

DinicFlow *aux_network_find_flow(dinic_t *data, Network *aux_net, bool verbose) {

    Node *next_node = NULL;
    Stack *dfs_stack = NULL;
    SList *neighbours = NULL, *path = NULL;
    bool is_t_reached = false;
    DinicFlow *result = NULL;

    /* Precondicion */
    assert(aux_net != NULL);
    assert(data != NULL);
    assert(data->network != NULL);
    assert(network_has_node(aux_net, *data->s));

    assert(network_has_node(aux_net, *data->t));

    result = (DinicFlow *) calloc(1, sizeof(*result));
    memory_check(result);


    /* dfs_stack matiene las aristas por las que pasamos */
    /* Como invariante, en dfs siempre hay un solo camino */
    dfs_stack = stack_new();

    /* Invariante del ciclo:
     * No llegamos a t
     * (Si no ya encontramos el path)
     *
     * Si es stack esta vacio, tiene que que haber algun vecino que salga de s
     * (Si no, t no es alcanzable) */

    while (!is_t_reached && (!stack_is_empty(dfs_stack) ||
           !slist_is_empty(network_get_edges(aux_net, *data->s)))) {

        Edge *current_edge = NULL;
        char mode = 'f';

        /* Si el stack esta vacio, pusheamos alguna de las aristas de s */
        if (stack_is_empty(dfs_stack)) {
            SList *edges = NULL;
            Edge *edge = NULL;

            edges = network_get_edges(aux_net, *data->s);
            edge = (Edge *)slist_head_data(edges);
            if (can_send_flow(edge, 'f')) {
                stack_push(dfs_stack, slist_head_data(edges));
            }
            else {
                _network_del_edge(aux_net, edge, 'f');
                continue;
            }

            next_node = data->s;
        }

        current_edge = (Edge *) stack_head(dfs_stack);

        /* Actualizo el proximo nodo esperado
         *
         * Edge:
         *   (A - B)
         *
         * Si el nodo que debia agregar era el primero (A), este lado se agrego por
         * fordward, por lo que el proximo que tengo que agregar es el
         * segundo (B).
         *
         * Si el nodo que debia agregar era el segundo (B), este lado es backwards,
         * por lo que el proximo que tengo que agregar es el primero (A).
         *
         */
        if (*edge_get_first(current_edge) == *next_node) {
            mode = 'f';
            next_node = edge_get_second(current_edge);
        } else {
            mode = 'b';
            next_node = edge_get_first(current_edge);
        }

        neighbours = network_get_edges(aux_net, *next_node);

        if (!slist_is_empty(neighbours)) {
            Edge *edge = NULL;

            edge = slist_head_data(neighbours);
            mode = (*edge_get_first(edge) == *next_node)?'f':'b';

            /* No hacemos chequeo de si llegamos a t porque t no tiene vecinos */
            if (can_send_flow(edge, mode)) {
                /* Tengo flujo para mandar, lo agrego al camino actual */
                stack_push(dfs_stack, edge);
            } else {
                /* No puedo mandar nada por ese edge, lo borro del network */
                _network_del_edge(aux_net, edge, mode);

                if (*edge_get_first(current_edge) == *next_node) {
                    next_node = edge_get_second(current_edge);
                } else {
                    next_node = edge_get_first(current_edge);
                }
            }
        } else {
            /* No tiene vecinos */
            if (*next_node == *data->t && can_send_flow(current_edge, mode)) {
                is_t_reached = true;
            } else {
                /* Ese camino no me lleva a ningun lado.
                 * Ejemplo:
                 *
                 * (...) -> C -> A -> B
                 *
                 * Edges en stack:
                 * (A - B) (C - A) (...)
                 *
                 * La proxima arista que se deberia agregar sale de B (O
                 * vuelve, si es backwards). Este es nuestro next_node.
                 *
                 * Si llegamos a esta instancia, B no tiene vecinos.
                 * Nos volvemos un paso, asignando next_node a A.
                 *
                 * Borramos el arista, y la sacamos del stack.
                 * El stack nos queda:
                 * (C - A) (...)
                 *
                 * Entonces tenemos dar otro paso hacia atras.
                 * Esto es para que next_node quede en C y sepa que ha
                 * agregado a ese lado como forward (En este caso).
                 */

                 /* Lo borramos del network y lo quitamos del stack */
                _network_del_edge(aux_net, current_edge, mode);
                stack_pop(dfs_stack);

                /* Volvemos un paso */
                if (mode == 'f') {
                    next_node = edge_get_first(current_edge);
                } else {
                    next_node = edge_get_second(current_edge);
                }

                /* Nos volvemos otro */
                if (!stack_is_empty(dfs_stack)) {
                    current_edge = (Edge *) stack_head(dfs_stack);
                    if (*edge_get_first(current_edge) == *next_node) {
                        next_node = edge_get_second(current_edge);
                    } else {
                        next_node = edge_get_first(current_edge);
                    }
                }
            }
        }
    }

    /* path es una lista de la forma [(s, a), (a, b), ..., (r, t)]  o []*/
    path = slist_reverse(stack_to_list(dfs_stack));
    stack_free(dfs_stack);

    {
        Flow current_flow = UINT_MAX;
        next_node = data->s;

        result->path = path;
        if (path == NULL) {
            current_flow = 0;
        }
        while (path != NULL) {
            Edge *edge = slist_head_data(path);

            /* Calculamos el valor del flujo del camino en current_flow */
            if(*edge_get_first(edge) == *next_node) {
                /* Arista Forward */
                current_flow = min(current_flow, edge_get_capacity(edge) - edge_get_flow(edge));
                next_node = edge_get_second(edge);
            } else {
                /* Backward */
                current_flow = min(current_flow, edge_get_flow(edge));
                next_node = edge_get_first(edge);
            }
            path = slist_next(path);
        }
        result->flow_value = current_flow;
    }

    if (verbose) {
        flow_pretty_print(data, result);
    }

    /* Postcondiciones */

    assert(result != NULL);
    assert(data->network != NULL);

    return result;
}


dinic_result *dinic(Network *network, Node *s, Node *t, bool verbose) {
    dinic_t data; /* Envoltorio principal que se pasa internamente */
    dinic_result *result = NULL;
    Network *aux_net = NULL;
    DinicFlow *current = NULL;
    bool found_max_flow = false;
    unsigned int na_count = 1; /* Cuenta de los N.A.s (cuando verbose) */
    Flow flow_value = 0, na_flow_value = 0;

    /* Precondiciones */
    assert(network != NULL);
    assert(network_has_node(network, *s));
    assert(network_has_node(network, *t));

    /* Inicializando las estructuras */
    data.network = network;
    data.s = s;
    data.t = t;

    result = calloc(1, sizeof(dinic_result));
    result->flow_value = 0;
    result->max_flow = NULL;
    result->min_cut = NULL;

    /* Pasos Basicos:
     * 1) Crear network auxiliar
     * 2) es un corte minimal?:
     *                         a) No -> Buscar flujo bloqueante.
     *                         b) Si -> terminamos.
     *
     *    a)
     *      i) Buscar camino aumentante.
     *      ii) Actualizar network con el flujo que se manda por el mismo.
     *      iii) Si piden verbose -> imprimir Info.
     *      iv) Actualizar contador de flujo total enviado.
     *      v) Goto 1)
     *
     *
     *    b) Guardar resultados en result y setear flag para salir.
     *
     * 3) liberar estructuras temporales desechables, Goto 1)
     */

    if (verbose) puts(""); /* Detallito, nueva linea antes de empezar */

    while (!found_max_flow) {
        /* Verbose Info: */

        /* 1 */
        aux_net = aux_network_new(&data);
        memory_check(aux_net);

        /* 2 */
        if (network_has_node(aux_net, *t)) {
            /* Buscamos flujo bloqueante */

            if (verbose) printf("N.A %u:\n", na_count);

            /* Inicializamos ciclo buscando el primer camino aumentante */
            current = aux_network_find_flow(&data, aux_net, verbose);
            memory_check(current);

            while (!slist_is_empty(current->path) &&
                  (current->flow_value > 0)) {

                /* 2a */
                /* Actualizamos el flujo enviado por el camino aumentante */
                network_update(data.network, current->path, current->flow_value);

                na_flow_value += current->flow_value;

                slist_free(current->path);
                free(current); current = NULL;

                current = aux_network_find_flow(&data, aux_net, verbose);
                memory_check(current);
            }
            /* eliminamos el ultimo DinicFlow */
            slist_free(current->path);
            free(current); current = NULL;

            /* Tenemos flujo bloqueante en este NA */
            if (verbose) {
                printf("El N.A. %u aumenta el flujo en %u.\n\n",\
                        na_count, na_flow_value);
                na_count++;
            }

            /* Actualizamos la cuenta del flujo global */
            flow_value += na_flow_value;
            na_flow_value = 0;

        } else {
            if (verbose) {
                printf("N.A %u:\n", na_count);
                printf("El N.A. %u aumenta el flujo en %u.\n\n", na_count, 0);
            }

            /* 2b -> Terminamos */
            /* El Corte Minimal son los nodos que quedan en el ultimo NA*/
            assert(aux_net != NULL);

            if(!network_has_node(aux_net, *s)) {
                /* Parche para caso excepcional en nuestra implementacion */
                result->min_cut = slist_prepend(result->min_cut, ((void*) s));
            } else {
                result->min_cut = network_nodes(aux_net);
            }
            result->flow_value = flow_value;
            result->max_flow = network_forward_edges(network);
            found_max_flow = true;

        }
        /* 3 */
        network_destroy(aux_net);
        aux_net = NULL;
    }


    assert(aux_net == NULL); /* deberia estar borrada */


    /* Postcondicion */
    assert(result != NULL);
    assert(result->min_cut != NULL);
    assert(result->max_flow != NULL);

    return result;
}

