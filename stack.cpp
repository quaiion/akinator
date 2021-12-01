#include "akinator.hpp"
               
static void stack_resize_up (stack_t *stack);
static void stack_resize_down (stack_t *stack);

void stack_ctor (stack_t *stack, ssize_t min_capacity /* = 8 */) {

    stack->size = 0;
    stack->min_capacity = min_capacity;
    stack->capacity = min_capacity;
    stack->data = (node_t **) calloc (min_capacity, sizeof (node_t *));
}

void stack_dtor (stack_t *stack) {

    memset (stack->data, BROKEN_BYTE, stack->capacity * sizeof (node_t *));
    free (stack->data);

    stack->data = (node_t **) OS_RESERVED_ADDRESS;
    stack->size = SIZE_MAX;
    stack->capacity = SIZE_MAX;
    stack->min_capacity = SIZE_MAX;
}

void stack_push (stack_t *stack, node_t *value) {

    if (stack->size == stack->capacity) stack_resize_up (stack);

    stack->data [stack->size ++] = value;
}

node_t *stack_pop (stack_t *stack) {

    if (stack->size < 1) {

        return NULL;
    }

    node_t *value = stack->data [-- stack->size];

    if (stack->size == stack->capacity / 4 && stack->size >= stack->min_capacity) stack_resize_down (stack);

    return value;
}

node_t *stack_shadow_pop (stack_t *stack) {

    if (stack->size < 1) {

        return NULL;
    }

    return stack->data [stack->size - 1];
}

static void stack_resize_up (stack_t *stack) {

    stack->data = (node_t **) realloc (stack->data, (stack->capacity = stack->capacity * 2) * sizeof (node_t *));
}

static void stack_resize_down (stack_t *stack) {

    stack->data = (node_t **) realloc (stack->data, (stack->capacity = stack->capacity / 2) * sizeof (node_t *));
}
