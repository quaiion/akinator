#ifndef BIN_TREE_ACTIVE
#define BIN_TREE_ACTIVE

#define BIN_TREE_AUTO_QUICK_VERIFICATION

#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <string.h>
#include "bin_node.hpp"
#include "node_stack.hpp"
#include "common.hpp"

#define BINTR_STRINGS_EQUAL 0
#define BINTR_OS_RESERVED_ADDRESS 1

constexpr size_t DEFAULT_NODE_TBL_SIZE = 16;

struct bin_tree_t {

    bin_node_t *root;
    bin_node_t *keyhole;
    bool keybranch;
};

void bin_tree_ctor (bin_tree_t *tree);
void bin_tree_dtor (bin_tree_t *tree);

bin_node_t *bin_tree_move_keyhole_to (bin_tree_t *tree, bool (*walk_node) (const bin_node_t *));        // Это единственная функция, меняющая keyhole
bin_node_t *bin_tree_bt_track_path (bin_tree_t *tree, char *target_name,
                                    void (*spot_pos_node) (const bin_node_t *), void (*spot_neg_node) (const bin_node_t *));
bin_node_t *bin_tree_tb_stack_path (bin_tree_t *tree, char *target_name, node_stack_t *stack);

enum VERIFICATION_CODES {DEFAULT, CYCLED, BRANCH_FAULT, NULL_DATA};

bin_node_t *bin_tree_verify_qck (bin_tree_t *tree, VERIFICATION_CODES *ver_code = NULL);
bin_node_t *bin_tree_verify_slw (bin_tree_t *tree, VERIFICATION_CODES *ver_code = NULL);
void bin_tree_vis_dump (bin_tree_t *tree, char *file_name);

bin_node_t *bin_tree_create_leaf (char *data);
bin_node_t *bin_tree_insert_fork (bin_tree_t *tree, bin_node_t *par_node, bin_node_t *pos_node, bin_node_t *neg_node);
bin_node_t *UNSAFE_bin_tree_place_fork (bin_tree_t *tree, bin_node_t *par_node, bin_node_t *pos_node, bin_node_t *neg_node);
bin_node_t *bin_tree_split_leaf (bin_tree_t *tree, char *obj_data, char *char_data);
bin_node_t *bin_tree_add_leaf (bin_tree_t *tree, char *data);
bin_node_t *UNSAFE_bin_tree_place_leaf (bin_tree_t *tree, char *data);

#endif
