/*
 * Queue with GQueue interface.
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "queue.h"
#include "slist.h"

Queue* queue_new (void) {
    Queue *new_queue = NULL;

    new_queue = calloc(1, sizeof(*new_queue));

    if (new_queue != NULL) {
        new_queue->head = NULL;
        new_queue->tail = NULL;
        new_queue->length = 0;
    }

    return new_queue;
}


void queue_free (Queue *queue) {
    /* Precondition */
    assert(queue != NULL);

    slist_free(queue->head);
    free(queue);
}

void queue_clear (Queue *queue) {
    /* Not implemented Yet */
    assert(0);
}


bool queue_is_empty (Queue *queue) {
    /* Precondition */
    assert(queue != NULL);

    return queue->length == 0;
}

unsigned int queue_get_length (Queue *queue) {
    /* Precondition */
    assert(queue != NULL);

    return queue->length;
}

void queue_reverse (Queue *queue) {
    /* Not implemented Yet */
    assert(0);
}

Queue * queue_copy (Queue *queue) {
    Queue *new_queue = NULL;

    /* Precondition */
    assert(queue != NULL);

    new_queue = calloc(1, sizeof(*new_queue));

    if (new_queue != NULL) {
        new_queue->head = slist_copy(queue->head);
        new_queue->tail = slist_last(queue->head);
        new_queue->length = queue->length;
    }

    return new_queue;
}


void queue_foreach (Queue *queue, Func(func), void * user_data) {
    /* Precondition */
    assert(queue != NULL);

    slist_foreach(queue->head, func, user_data);
}


SList * queue_find (Queue *queue, const void *data) {
    /* Precondition */
    assert(queue != NULL);

    return slist_find(queue->head, data);
}

SList * queue_find_custom (Queue *queue, const void *data, CompareFunc(func)) {
    /* Precondition */
    assert(queue != NULL);

    return slist_find_custom(queue->head, data, func);
}


void queue_sort (Queue *queue, CompareDataFunc(compare_func),
                 void * user_data) {
    /* Not Implemented Yet*/
    assert(0);
}


void queue_push_head (Queue *queue, void * data) {
    /* Precondition */
    assert(queue != NULL);
    assert(queue->length != 0);

    queue->head = slist_prepend (queue->head, data);
    queue->length ++;
}


void queue_push_tail (Queue *queue, void * data) {
    /* Precondition */
    assert(queue != NULL);
    assert(queue->length != 0);

    queue->tail = slist_append (queue->tail, data);
    queue->length ++;
}


void queue_push_nth (Queue *queue, void * data, unsigned int n) {
    /* Precondition */
    assert(queue != NULL);
    assert(queue->length != 0);
    assert(n <= queue->length);

    if (n == 0) {
       queue_push_head (queue, data);
    }
    else if (n == queue->length){
        queue_push_tail (queue, data);
    }
    else {
        slist_insert(queue->head, data, n);
        queue->length ++;
    }
}


void * queue_pop_head (Queue *queue) {
    void *element_data = NULL;
    SList *temp_element = NULL;

    /* Precondition */
    assert(queue != NULL);
    assert(queue->length != 0);

    element_data = slist_nth_data(queue->head, 0);
    temp_element = slist_next(queue->head);
    slist_free_1(queue->head);
    queue->head = temp_element;
    queue->length --;
    if (queue->length == 0) {
        queue->tail = NULL;
    }
    return element_data;
}


void * queue_pop_tail (Queue *queue) {
    void *element_data = NULL;
    SList *temp_previous = NULL;

    /* Precondition */
    assert(queue != NULL);
    assert(queue->length != 0);

    if (queue->length == 1) {
        element_data = slist_nth_data(queue->tail, 0);
        slist_free_1(queue->tail);
        queue->head = NULL;
        queue->tail = NULL;
    }
    else {
        /* Minus 2, because list starts at pos 0 */
        temp_previous = slist_nth(queue->head, queue->length - 2);
        element_data = slist_nth_data(queue->tail, 0);
        slist_free_1(queue->tail);
        temp_previous->next = NULL;
        queue->tail = temp_previous;
    }

    queue->length --;

    return element_data;
}

void * queue_pop_nth (Queue *queue, unsigned int n) {
    /* Not implemented Yet */
    assert(0);
}

void * queue_peek_head (Queue *queue) {
    /* Precondition */
    assert(queue != NULL);
    assert(queue->length > 0);

    return slist_nth_data(queue->head, 0);
}

void * queue_peek_tail (Queue *queue) {
    /* Precondition */
    assert(queue != NULL);
    assert(queue->length > 0);

    return slist_nth_data(queue->tail, 0);
}

void * queue_peek_nth (Queue *queue, unsigned int n) {
    /* Precondition */
    assert(queue != NULL);
    assert(queue->length > 0);
    assert(n < queue->length);

    return slist_nth_data(queue->head, n);
}

int queue_index (Queue *queue, const void *data) {
    /* Precondition */
    assert(queue != NULL);

    return slist_index(queue->head, data);
}

void queue_remove (Queue *queue, const void *data) {
    /* Not Implemented Yet*/
    assert(0);
}

void queue_remove_all (Queue *queue, const void *data) {
    /* Not Implemented Yet*/
    assert(0);
}
void queue_insert_before (Queue *queue, SList *sibling, void * data) {
    /* Not Implemented Yet*/
    assert(0);
}

void queue_insert_after (Queue *queue, SList *sibling, void * data) {
    /* Not Implemented Yet*/
    assert(0);
}
void queue_insert_sorted (Queue *queue, void * data, CompareDataFunc(func),
                          void * user_data) {
    /* Not Implemented Yet*/
    assert(0);
}

void queue_push_head_link (Queue *queue, SList *link_) {
    /* Not Implemented Yet*/
    assert(0);
}
void queue_push_tail_link (Queue *queue, SList *link_) {
    /* Not Implemented Yet*/
    assert(0);
}
void queue_push_nth_link (Queue *queue, int n, SList *link_) {
    /* Not Implemented Yet*/
    assert(0);
}
SList* queue_pop_head_link (Queue *queue) {
    /* Not Implemented Yet*/
    assert(0);
    return NULL;
}
SList* queue_pop_tail_link (Queue *queue) {
    /* Not Implemented Yet*/
    assert(0);
    return NULL;
}
SList* queue_pop_nth_link (Queue *queue, unsigned int n) {
    /* Not Implemented Yet*/
    assert(0);
    return NULL;
}
SList* queue_peek_head_link (Queue *queue) {
    /* Not Implemented Yet*/
    assert(0);
    return NULL;
}
SList* queue_peek_tail_link (Queue *queue) {
    /* Not Implemented Yet*/
    assert(0);
    return NULL;
}
SList* queue_peek_nth_link (Queue *queue, unsigned int n) {
    /* Not Implemented Yet*/
    assert(0);
    return NULL;
}
int queue_link_index (Queue *queue, SList *link_) {
    /* Not Implemented Yet*/
    assert(0);
    return 0;
}
void queue_unlink (Queue *queue, SList *link_) {
    /* Not Implemented Yet*/
    assert(0);
}
void queue_delete_link (Queue *queue, SList *link_) {
    /* Not Implemented Yet*/
    assert(0);
}

