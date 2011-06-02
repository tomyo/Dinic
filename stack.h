#ifndef STACK_H
#define STACK_H

typedef Stack Queue;

#define stack_new() queue_new()
#define stack_free(stack) queue_free(stack)
#define stack_is_empty(stack) queue_is_empty(stack)
#define stack_push(stack, elem) queue_push_head(stack, elem)
#define stack_pop(stack) queue_pop_head(stack)

#endif /* STACK_H */
