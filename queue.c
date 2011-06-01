/*
 * Queue with GQueue interface.
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "queue.h"
#include "slist.h"

struct _Queue
{
 SList *head;
 SList *tail;
 unsigned int length;
};

Queue* queue_new (void) {
    Queue *new_queue = NULL;
    
    new_queue = calloc(1, sizeof(*new_queue));
    
    new_queue->head = NULL;
    new_queue->tail = NULL;
    new_queue->length = 0;
    
    return new_queue;
}


void queue_free (Queue *queue) {
    
    if (queue->length != 0) {
        slist_free(queue->head);
    }
    free(queue);
}

void queue_clear (Queue *queue) {
    ;
}


bool queue_is_empty (Queue *queue) {
    return queue->length == 0;
}

unsigned int queue_get_length (Queue *queue) {
    return queue->length;
}

void queue_reverse (Queue *queue) {
    ;
}

Queue * queue_copy (Queue *queue) {
    Queue *new_queue = NULL;
    
    new_queue = calloc(1, sizeof(*new_queue));
   
    new_queue->head = slist_copy(queue->head);
    new_queue->tail = slist_last(queue->head);
    new_queue->length = queue->length;
    
    return new_queue;
}


void queue_foreach (Queue *queue, Func(func), void * user_data) {
    slist_foreach(queue->head, func, user_data);
}


SList * queue_find (Queue *queue, const void *data) {
    return slist_find(queue->head, data);
}

SList * queue_find_custom (Queue *queue, const void *data, CompareFunc(func)) {
    return slist_find_custom(queue->head, data, func);
}


void queue_sort (Queue *queue, CompareDataFunc(compare_func),
                 void * user_data) {
    ;
}


void queue_push_head (Queue *queue, void * data) {
    queue->head = slist_prepend (queue->head, data);
    queue->length ++;
}


void queue_push_tail (Queue *queue, void * data) {
    queue->tail = slist_append (queue->tail, data);
    queue->length ++;
}


void queue_push_nth (Queue *queue, void * data, unsigned int n) {
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
    
    if (queue->length == 1) {
        element_data = slist_nth_data(queue->tail, 0);
        slist_free_1(queue->head);
        queue->head = NULL;
        queue->tail = NULL;
    }
    else {
        temp_previous = slist_nth(queue->head, queue->length - 1);
        element_data = slist_nth_data(queue->tail, 0);
        slist_free_1(queue->tail);
        queue->tail = temp_previous;
    }
    
    queue->length --;
        
    return element_data;
}

void * queue_pop_nth (Queue *queue, unsigned int n) {
    return NULL;
}

void * queue_peek_head (Queue *queue) {
    return slist_nth_data(queue->head, 0);
}

void * queue_peek_tail (Queue *queue) {
    return slist_nth_data(queue->tail, 0);
}

void * queue_peek_nth (Queue *queue, unsigned int n) {
        return slist_nth_data(queue->head, n);
}

int queue_index (Queue *queue, const void *data) {
    return slist_index(queue->head, data);
}

void queue_remove (Queue *queue, const void *data) {
    ;
}

void queue_remove_all (Queue *queue, const void *data) {
    ;
}
void queue_insert_before (Queue *queue, SList *sibling, void * data) {
    ;
}

void queue_insert_after (Queue *queue, SList *sibling, void * data) {
    ;
}
void queue_insert_sorted (Queue *queue, void * data, CompareDataFunc(func),
                          void * user_data) {
    ;
}

void queue_push_head_link (Queue *queue, SList *link_) {
    ;
}
void queue_push_tail_link (Queue *queue, SList *link_) {
    ;
}
void queue_push_nth_link (Queue *queue, int n, SList *link_) {
    ;
}
SList* queue_pop_head_link (Queue *queue) {
    return NULL;
}
SList* queue_pop_tail_link (Queue *queue) {
    return NULL;
}
SList* queue_pop_nth_link (Queue *queue, unsigned int n) {
    return NULL;
}
SList* queue_peek_head_link (Queue *queue) {
    return NULL;
}
SList* queue_peek_tail_link (Queue *queue) {
    return NULL;
}
SList* queue_peek_nth_link (Queue *queue, unsigned int n) {
    return NULL;
}
int queue_link_index (Queue *queue, SList *link_) {
    return 0;
}
void queue_unlink (Queue *queue, SList *link_) {
    ;
}
void queue_delete_link (Queue *queue, SList *link_) {
    ;
}
