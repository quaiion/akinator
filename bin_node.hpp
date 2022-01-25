#ifndef BIN_NODE_ACTIVE
#define BIN_NODE_ACTIVE

#include "common.hpp"

struct bin_node_t {

    char *data;
    bin_node_t *pos;
    bin_node_t *neg;
};

#endif
