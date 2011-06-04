#ifndef QUEUE_H
#define QUEUE_H

#include "slist.h"

/**
 * @author Grupo xxx.
 * @brief Queue implementationImplementacion de colas
 *
 * Interfaz de queues "similar" a la de la GLIB.
 * PELIGRO: muchas funciones no van a estar implementadas.
 */

typedef struct _Queue Queue;

struct _Queue
{
    SList *head;
    SList *tail;
    unsigned int length;
};

/**
 * @brief Creates a new Queue.
 */
Queue *queue_new (void);

/**
 * @brief Frees the memory allocated for the Queue. If queue elements contain
 * dynamically-allocated memory, they should be freed first.
 */
void queue_free (Queue *queue);

/**
 * @brief Returns true if the queue is empty.
 */
bool queue_is_empty (Queue *queue);

/**
 * @brief Returns the number of items in queue.
 */
unsigned int queue_get_length (Queue *queue);

/**
 * @brief Copies a queue. Note that is a shallow copy. If the elements in the queue
 * consist of pointers to data, the pointers are copied, but the actual data is not
 */
Queue *queue_copy (Queue *queue);

/**
 * @brief Calls func for each element in the queue passing user_data to the function.
 * @param queue : a Queue
 * @param func : the function to call for each element's data
 * @param user_data : user data to pass to func
 */
void queue_foreach (Queue *queue, UserFunc(func), void *user_data);

/**
 * @brief Finds the first link in queue which contains data.
 * @param queue : a Queue
 * @param data : data to find
 * @param Returns : The first link in queue which contains data or NULL.
 */
SList *queue_find (Queue *queue, const void *data);

/**
 * @brief Finds an element in a Queue, using a supplied function to find the
 * desired element. It iterates over the queue, calling the given function
 * which should return 0 when the desired element is found. The function
 * takes two const void *arguments, the Queue element's data as the first
 * argument and the given user data as the second argument.
 * @param queue : a Queue
 * @param data : user data passed to func
 * @param func : a CompareFunc to call for each element. It should return 0 when the
 * @param desired element is found
 * @returns The found link, or NULL if it wasn't found
 */
SList *queue_find_custom (Queue *queue, const void *data, CompareFunc(func));

/**
 * @brief Adds a new element at the head of the queue.
 */
void queue_push_head (Queue *queue, void *data);

/**
 * @brief Adds a new element at the tail of the queue.
 */
void queue_push_tail (Queue *queue, void *data);

/**
 * @brief Inserts a new element into queue at the given position
 */
void queue_push_nth (Queue *queue, void *data, unsigned int n);

/**
 * @removes the first element of the queue.
 */
void *queue_pop_head (Queue *queue);

/**
 * @removes the last element of the queue. Lineal order.
 */
void *queue_pop_tail (Queue *queue);

/**
 * @returns the first element of the queue.
 */
void *queue_peek_head (Queue *queue);

/**
 * @returns the last element of the queue. Lineal order.
 */
void *queue_peek_tail (Queue *queue);

/**
 * @returns the n'th element of queue.
 */
void *queue_peek_nth (Queue *queue, unsigned int n);

/**
 * @returns the position of the first element in queue which contains data.
 */
int queue_index (Queue *queue, const void *data);

#endif
