#ifndef STACK_H
#define STACK_H

typedef Stack Queue;

#define stack_new() queue_new()
#define stack_destroy(stack) queue_free(stack)
#define stack_push(stack, elem) queue_push_head(stack, elem)
#define stack_pop(stack) queue_pop_head(stack)

#endif /* STACK_H */
