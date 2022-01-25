#ifndef NODE_STACK_ACTIVE
#define NODE_STACK_ACTIVE

#define NODE_STACK_AUTO_VERIFICATION

#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <string.h>
#include "bin_node.hpp"
#include "common.hpp"

#define NODST_OS_RESERVED_ADDRESS 1

struct node_stack_t {

    bin_node_t **data;
    ssize_t capacity;
    ssize_t min_capacity /* >= 1 */;
    ssize_t size;
};

constexpr unsigned char NODST_BROKEN_BYTE = 0xFF;

void node_stack_ctor (node_stack_t *stack, ssize_t min_capacity = 8);
void node_stack_dtor (node_stack_t *stack);
void node_stack_push (node_stack_t *stack, bin_node_t *value);
bin_node_t *node_stack_pop (node_stack_t *stack);
bin_node_t *node_stack_shadow_pop (node_stack_t *stack);

enum NODE_STACK_VERIFICATION_CODE {NODST_NO_FLAWS, NODST_LOW_MIN_CAP, NODST_LOW_CAP,
                                   NODST_LOW_SIZE, NODST_OVERFLOW, NODST_UNDERFLOW,
                                   NODST_DATA_LOSS, NODST_DATA_DISTRUCTED};
NODE_STACK_VERIFICATION_CODE node_stack_verify (node_stack_t *stack);

#endif
