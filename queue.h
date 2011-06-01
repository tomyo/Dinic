#ifndef QUEUE_H
#define QUEUE_H

#include "slist.h"

/*
Grupo x (?) Discreta 2011, una peque#a implementacion de queues, implementadas
con las
*/

typedef struct _Queue Queue;

struct _Queue
{
 SList *head;
 SList *tail;
 unsigned int length;
};

/*
Interfaz de queues "similar" a la de la GLIB, PELIGRO: muchas funciones no van
a estar implementadas.
*/

/*
Creates a new Queue.
*/
Queue* queue_new (void);

/*
Frees the memory allocated for the Queue. If queue elements contain
dynamically-allocated memory, they should be freed first.
*/
void queue_free (Queue *queue);

/*
Removes all the elements in queue. If queue elements contain
dynamically-allocated memory, they should be freed first.
*/
void queue_clear (Queue *queue);

/*
Returns true if the queue is empty.
*/
bool queue_is_empty (Queue *queue);

/*
Returns the number of items in queue.
*/
unsigned int queue_get_length (Queue *queue);

/*
Reverses the order of the items in queue.
*/
void queue_reverse (Queue *queue);

/*
Copies a queue. Note that is a shallow copy. If the elements in the queue
consist of pointers to data, the pointers are copied, but the actual data is not
*/
Queue * queue_copy (Queue *queue);

/*
Calls func for each element in the queue passing user_data to the function.
queue : a Queue
func : the function to call for each element's data
user_data : user data to pass to func
*/
void queue_foreach (Queue *queue, Func(func), void * user_data);

/*
Finds the first link in queue which contains data.
queue : a Queue
data : data to find
Returns : The first link in queue which contains data.
*/
SList * queue_find (Queue *queue, const void *data);

/*
Finds an element in a Queue, using a supplied function to find the desired element. It iterates over the queue, calling the given function which should return 0 when the desired element is found. The function takes two const void *arguments, the Queue element's data as the first argument and the given user data as the second argument.
queue : a Queue
data : user data passed to func
func : a CompareFunc to call for each element. It should return 0 when the
desired element is found
Returns : The found link, or NULL if it wasn't found
*/
SList * queue_find_custom (Queue *queue, const void *data, CompareFunc(func));

/*
Sorts queue using compare_func.
queue : a Queue
compare_func : the CompareDataFunc used to sort queue. This function is passed
two elements of the queue and should return 0 if they are equal, a negative
value if the first comes before the second, and a positive value if the second
comes before the first.
user_data : user data passed to compare_func
*/
void queue_sort (Queue *queue, CompareDataFunc(compare_func), void * user_data);

/*
Adds a new element at the head of the queue.
*/
void queue_push_head (Queue *queue, void * data);

/*
Adds a new element at the tail of the queue.
*/
void queue_push_tail (Queue *queue, void * data);

/*
Inserts a new element into queue at the given position
*/
void queue_push_nth (Queue *queue, void * data, unsigned int n);

/*
Removes the first element of the queue.
*/
void * queue_pop_head (Queue *queue);

/*
Removes the last element of the queue. Lineal order.
*/
void * queue_pop_tail (Queue *queue);

/*
Removes the n'th element of queue.
*/
void * queue_pop_nth (Queue *queue, unsigned int n);

/*
Returns the first element of the queue.
*/
void * queue_peek_head (Queue *queue);

/*
Returns the last element of the queue. Lineal order.
*/
void * queue_peek_tail (Queue *queue);

/*
Returns the n'th element of queue.
*/
void * queue_peek_nth (Queue *queue, unsigned int n);

/*
Returns the position of the first element in queue which contains data.
*/
int queue_index (Queue *queue, const void *data);

/*
Removes the first element in queue that contains data.
*/
void queue_remove (Queue *queue, const void *data);

/*
Remove all elements whose data equals data from queue.
*/
void queue_remove_all (Queue *queue, const void *data);

/*
Inserts data into queue before sibling.
sibling must be part of queue.
queue : a Queue
sibling : a SList link that must be part of queue
data : the data to insert
*/
void queue_insert_before (Queue *queue, SList *sibling, void * data);

/*
*/
void queue_insert_after (Queue *queue, SList *sibling, void * data);
void queue_insert_sorted (Queue *queue, void * data, CompareDataFunc(func), void * user_data);

void queue_push_head_link (Queue *queue, SList *link_);
void queue_push_tail_link (Queue *queue, SList *link_);
void queue_push_nth_link (Queue *queue, int n, SList *link_);
SList* queue_pop_head_link (Queue *queue);
SList* queue_pop_tail_link (Queue *queue);
SList* queue_pop_nth_link (Queue *queue, unsigned int n);
SList* queue_peek_head_link (Queue *queue);
SList* queue_peek_tail_link (Queue *queue);
SList* queue_peek_nth_link (Queue *queue, unsigned int n);
int queue_link_index (Queue *queue, SList *link_);
void queue_unlink (Queue *queue, SList *link_);
void queue_delete_link (Queue *queue, SList *link_);

#endif

