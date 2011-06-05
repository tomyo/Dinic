#ifndef STACK_H
#define STACK_H

typedef Stack Queue;

#define stack_new() queue_new()
#define stack_free(stack) queue_free(stack)
#define stack_is_empty(stack) queue_is_empty(stack)
#define stack_push(stack, elem) queue_push_head(stack, elem)
#define stack_pop(stack) queue_pop_head(stack)
#define stack_head(stack) queue_peek_head(stack)
#define stack_to_list(stack) queue_list(stack)

#endif /* STACK_H */
