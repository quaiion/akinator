#include "node_stack.hpp"
               
static void stack_resize_up (node_stack_t *stack);
static void stack_resize_down (node_stack_t *stack);

void node_stack_ctor (node_stack_t *stack, ssize_t min_capacity /* = 8 */) {

    assert (stack);
    assert (min_capacity);

    stack->size = 0;
    stack->min_capacity = min_capacity;
    stack->capacity = min_capacity;
    stack->data = (bin_node_t **) calloc (min_capacity, sizeof (bin_node_t *));
}

void node_stack_dtor (node_stack_t *stack) {

    assert (stack);

#ifdef NODE_STACK_AUTO_VERIFICATION

    if (node_stack_verify (stack) != NODST_NO_FLAWS) {

        printf ("\nNodestack: dtor autoverification failed, operation terminated\n");
        return;
    }

#endif

    memset (stack->data, NODST_BROKEN_BYTE, stack->capacity * sizeof (bin_node_t *));
    free (stack->data);

    stack->data = (bin_node_t **) NODST_OS_RESERVED_ADDRESS;
    stack->size = -1;
    stack->capacity = -1;
    stack->min_capacity = -1;
}

void node_stack_push (node_stack_t *stack, bin_node_t *value) {

#ifdef NODE_STACK_AUTO_VERIFICATION

    if (node_stack_verify (stack) != NODST_NO_FLAWS) {

        printf ("\nNodestack: push autoverification failed, operation terminated\n");
        return;
    }

#endif

    assert (stack);
    assert (value);

    if (stack->size == stack->capacity) stack_resize_up (stack);

    stack->data [stack->size ++] = value;
}

bin_node_t *node_stack_pop (node_stack_t *stack) {

#ifdef NODE_STACK_AUTO_VERIFICATION

    if (node_stack_verify (stack) != NODST_NO_FLAWS) {

        printf ("\nNodestack: pop autoverification failed, operation terminated\n");
        return NULL;
    }

#endif

    assert (stack);

    if (stack->size < 1) {

        return NULL;
    }

    bin_node_t *value = stack->data [-- stack->size];

    if (stack->size == stack->capacity / 4 && stack->size >= stack->min_capacity) stack_resize_down (stack);

    return value;
}

bin_node_t *node_stack_shadow_pop (node_stack_t *stack) {

#ifdef NODE_STACK_AUTO_VERIFICATION

    if (node_stack_verify (stack) != NODST_NO_FLAWS) {

        printf ("\nNodestack: shadow pop autoverification failed, operation terminated\n");
        return NULL;
    }

#endif

    assert (stack);

    if (stack->size < 1) {

        return NULL;
    }

    return stack->data [stack->size - 1];
}

static void stack_resize_up (node_stack_t *stack) {

    assert (stack);

    stack->data = (bin_node_t **) realloc (stack->data, (stack->capacity = stack->capacity * 2) * sizeof (bin_node_t *));
}

static void stack_resize_down (node_stack_t *stack) {

    assert (stack);

    stack->data = (bin_node_t **) realloc (stack->data, (stack->capacity = stack->capacity / 2) * sizeof (bin_node_t *));
}

NODE_STACK_VERIFICATION_CODE node_stack_verify (node_stack_t *stack) {

    if (stack->min_capacity < 1) {

        return NODST_LOW_MIN_CAP;
    }

    if (stack->capacity < 1) {

        return NODST_LOW_CAP;
    }

    if (stack->size < 0) {

        return NODST_LOW_SIZE;
    }

    if (stack->size > stack->capacity) {

        return NODST_OVERFLOW;
    }

    if (stack->capacity < stack->min_capacity) {

        return NODST_UNDERFLOW;
    }

    if (stack->data == NULL) {

        return NODST_DATA_LOSS;
    }

    if (stack->data == (bin_node_t **) NODST_OS_RESERVED_ADDRESS) {

        return NODST_DATA_DISTRUCTED;
    }

    return NODST_NO_FLAWS;
}
