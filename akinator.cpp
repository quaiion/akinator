#include "akinator.hpp"

static bin_node_t *scan_node (char **data);
static void upload_node (bin_node_t *node, FILE *database_file);
static void comp_print_similar_path (node_stack_t *path_stack, char *obj_name_1, char *obj_name_2);
static void comp_print_path_comparison (node_stack_t *path_stack_1, node_stack_t *path_stack_2, char *obj_name_1, char *obj_name_2);
static bool ask_char (const bin_node_t *node);
static void def_print_pos_char (const bin_node_t *char_node);
static void def_print_neg_char (const bin_node_t *char_node);

void ak_game_init () {

    char mem_mode = 0;
    while (true) {

        printf ("\nUse the exising database?\n*type \"y\" if yes, \"n\" if no, \"e\" if you want to exit to the menu*\n");
        scanf ("%c", &mem_mode);
        flush_input_buffer (mem_mode);
        if (mem_mode == EXISTING || mem_mode == NEW || mem_mode == EXIT) {

            break;
        }

        printf ("\nWrong input format, try again\n");
    }

    if (mem_mode == EXIT) {

        return;
    }

    bin_tree_t tree_ = {};
    bin_tree_t *tree = &tree_;
    bin_tree_ctor (tree);

    if (mem_mode == EXISTING) {

        if (load_database (tree) == DATABASE_EXITED) {

            bin_tree_dtor (tree);
            return;
        }

        bool round = true;
        printf ("\nNah boi now u won't exit till the game ends\n");
        while (round) {

            ak_game (tree);
            round = restart ();
        }
    } else if (mem_mode == NEW) {

        char *nobody_str = (char *) calloc (7, sizeof (char));
        strcpy (nobody_str, "nobody");                          // Тут костылина, ее бы поправить
        bin_tree_add_leaf (tree, nobody_str);

        bool round = true;
        printf ("\nNah boi now u won't exit till the game ends\n");
        while (round) {

            ak_game (tree);
            round = restart ();
        }
    }

    save_database (tree);
    bin_tree_dtor (tree);
}

void ak_def_init () {

    bin_tree_t tree_ = {};
    bin_tree_t *tree = &tree_;
    bin_tree_ctor (tree);
    if (load_database (tree) == DATABASE_EXITED) {

        bin_tree_dtor (tree);
        return;
    }
    
    bool round = true;
    while (round) {

        char *obj_name = (char *) calloc (MAX_OBJ_NAME + 1, sizeof (char));
        printf ("\nDefinition of which object would you like to see? (you still can type \"e\" and rescue me from this nightmare)\n");
        scanf ("%16s", obj_name);
        flush_input_buffer (obj_name [0]);
        if (obj_name [0] == 'e' && obj_name [1] == '\0') return;

        ak_def (tree, obj_name);
        free (obj_name);
        round = restart ();
    }

    bin_tree_dtor (tree);
}

void ak_comp_init () {

    bin_tree_t tree_ = {};
    bin_tree_t *tree = &tree_;
    bin_tree_ctor (tree);
    if (load_database (tree) == DATABASE_EXITED) {

        bin_tree_dtor (tree);
        return;
    }
    
    bool round = true;
    while (round) {

        char *obj_name_1 = (char *) calloc (MAX_OBJ_NAME + 1, sizeof (char));
        char *obj_name_2 = (char *) calloc (MAX_OBJ_NAME + 1, sizeof (char));

        printf ("\nEnter the name of the first object you want to be compared or type \"e\" if you want to exit\n");
        scanf ("%32s", obj_name_1);
        flush_input_buffer (obj_name_1 [0]);
        if (obj_name_1 [0] == 'e' && obj_name_1 [1] == '\0') return;

        printf ("\nEnter the name of the second object you want to be compared or type \"e\" if you want to exit\n");
        scanf ("%32s", obj_name_2);
        flush_input_buffer (obj_name_2 [0]);
        if (obj_name_1 [0] == 'e' && obj_name_1 [1] == '\0') return;

        ak_comp (tree, obj_name_1, obj_name_2);
        free (obj_name_1);
        free (obj_name_2);
        round = restart ();
    }

    bin_tree_dtor (tree);
}

void ak_vis_init () {

    bin_tree_t tree_ = {};
    bin_tree_t *tree = &tree_;
    bin_tree_ctor (tree);
    if (load_database (tree) == DATABASE_EXITED) {

        bin_tree_dtor (tree);
        return;
    }
    ak_vis (tree);

    bin_tree_dtor (tree);
}

LOAD_DATABASE_RESULT load_database (bin_tree_t *tree) {

    char *file_name = (char *) calloc (MAX_FILE_NAME + 1, sizeof (char));
    FILE *database_file = NULL;
    while (true) {

        printf ("\nEnter the name of a database file (with .akr) or just \"e\"\n");
        scanf ("%64s", file_name);
        flush_input_buffer (file_name [0]);
        if (file_name [0] == 'e' && file_name [1] == '\0') {
            
            free (file_name);
            return DATABASE_EXITED;
        }

        int file_name_len = strlen (file_name);
        if (file_name [file_name_len - 4] != '.' ||
            file_name [file_name_len - 3] != 'a' ||
            file_name [file_name_len - 2] != 'k' ||
            file_name [file_name_len - 1] != 'r' ||
            file_name_len < 4) {

            printf ("\nWrong database name format, try again\n");
            continue;
        }

        database_file = fopen (file_name, "r");
        if (database_file) {

            break;
        }

        printf ("\nNo such file in the directory, try again\n");
    }
    free (file_name);

    char *data_buffer = download_data_buffer (database_file);
    fclose (database_file);

    make_ak_tree (tree, data_buffer);
    free (data_buffer);

    return DATABASE_ENTERED;
}

void save_database (bin_tree_t *tree) {

    char save_mode = 0;
    while (true) {

        printf ("\nSave the new database?\n*Print \"y\" if yes, \"n\" if no*\n");
        scanf ("%c", &save_mode);
        flush_input_buffer (save_mode);
        if (save_mode == SAVE || save_mode == LEAVE) {

            break;
        }

        printf ("\nWrong input format, try again\n");
    }
    
    if (save_mode == SAVE) {

        char *database_name = (char *) calloc (MAX_FILE_NAME + 1, sizeof (char));

        printf ("\nName new database\n");
        scanf ("%61s", database_name);
        flush_input_buffer (database_name [0]);
        FILE *database_file = fopen (strcat (database_name, ".akr"), "w");
        free (database_name);

        upload_tree_as_database (tree, database_file);
        fclose (database_file);
    }
}

void ak_game (bin_tree_t *tree) {

    tree->keyhole = NULL;
    bin_node_t *leaf = bin_tree_move_keyhole_to (tree, ask_char);

    char ans = 0;
    while (true) {

        printf ("\nIs this object a %s?\n", leaf->data);
        scanf ("%c", &ans);
        flush_input_buffer (ans);
        if (ans == YES || ans == NO) {

            break;
        }

        printf ("\nWrong input format, try again\n");
    }

    if (ans == YES) {

        printf ("\nThat was ez lol\n");
        
    } else if  (ans == NO) {                          // На случай непредвиденных багов

        printf ("\nOh.. Alright, I can do with this\n\nWhat object were you thinking about, my dude?\n");
        char *obj_data = (char *) calloc (MAX_OBJ_NAME + 1, sizeof (char));
        scanf ("%16s", obj_data);
        flush_input_buffer (obj_data [0]);

        printf ("\nWhat makes it different from the one I suggested?\n");
        char *char_data = (char *) calloc (MAX_OBJ_CHAR + 1, sizeof (char));
        scanf ("%32s", char_data);
        flush_input_buffer (char_data [0]);

        bin_tree_split_leaf (tree, obj_data, char_data);

        printf ("\nOk I remembered it\n");
    }
}

static bool ask_char (const bin_node_t *node) {

    char ans = 0;
    while (true) {

        printf ("\nIs this object %s?\n", node->data);
        scanf ("%c", &ans);
        flush_input_buffer (ans);
        if (ans == YES || ans == NO) {

            break;
        }

        printf ("\nWrong input format, try again\n");
    }

    return (ans == YES) ? true : false;
}

void ak_def (bin_tree_t *tree, char *obj_name) {        // В этой версии определение выводится "снизу вверх", т.к. "сверху вниз" требует стека и неясно вообще нафига

    printf ("\n%s is, as far as I know", obj_name);
    if (bin_tree_bt_track_path (tree, obj_name, def_print_pos_char, def_print_neg_char)) {
            
        printf ("\n");

    } else {

        printf (", not mentioned in the database\n");
    }
}

static void def_print_pos_char (const bin_node_t *char_node) {

    printf (", %s", char_node->data);
}

static void def_print_neg_char (const bin_node_t *char_node) {

    printf (", not %s", char_node->data);
}

void ak_comp (bin_tree_t *tree, char *obj_name_1, char *obj_name_2) {       // Не ловит (вроде как и не должно) случаи, когда одинаковые свойства дублируются в разных бранчах

    node_stack_t stk_1 = {};
    node_stack_t *stk_1_p = &stk_1;
    node_stack_ctor (stk_1_p);
    if (bin_tree_tb_stack_path (tree, obj_name_1, stk_1_p) == NULL) {

        printf ("\n%s is, as far as I know, not mentioned in the database\n", obj_name_1);
        node_stack_dtor (stk_1_p);
        return;
    }

    if (strcmp (obj_name_1, obj_name_2) == STRINGS_EQUAL) {

        comp_print_similar_path (stk_1_p, obj_name_1, obj_name_2);

        node_stack_dtor (stk_1_p);
        return;
    } 

    node_stack_t stk_2 = {};
    node_stack_t *stk_2_p = &stk_2;
    node_stack_ctor (stk_2_p);
    if (bin_tree_tb_stack_path (tree, obj_name_2, stk_2_p) == NULL) {

        printf ("\n%s is, as far as I know, not mentioned in the database\n", obj_name_2);
        node_stack_dtor (stk_1_p);
        node_stack_dtor (stk_2_p);
        return;
    }

    comp_print_path_comparison (stk_1_p, stk_2_p, obj_name_1, obj_name_2);

    node_stack_dtor (stk_1_p);
    node_stack_dtor (stk_2_p);
}

static void comp_print_similar_path (node_stack_t *path_stack, char *obj_name_1, char *obj_name_2) {

    bin_node_t *cur = node_stack_pop (path_stack);
    bin_node_t *next = node_stack_shadow_pop (path_stack);
    if (next != NULL) {

        printf ("\n%s and %s are both", obj_name_1, obj_name_2);
        do {

            if (next == cur->pos) {

                printf (" %s,", cur->data);

            } else {

                printf (" not %s,", cur->data);
            }

            cur = node_stack_pop (path_stack);
            next = node_stack_shadow_pop (path_stack);
                
        } while (next != NULL);

    } else {

        printf ("\nNo characteristics of these objects found\n");
    }
}

static void comp_print_path_comparison (node_stack_t *path_stack_1, node_stack_t *path_stack_2, char *obj_name_1, char *obj_name_2) {

    bin_node_t *cur_1 = node_stack_pop (path_stack_1);
    bin_node_t *cur_2 = node_stack_pop (path_stack_2);
    bin_node_t *next_1 = node_stack_shadow_pop (path_stack_1);
    bin_node_t *next_2 = node_stack_shadow_pop (path_stack_2);

    if (next_1 == next_2) {

        bool first_common_char = true;
        printf ("\n%s and %s are both", obj_name_1, obj_name_2);
        do {
            
            bin_node_t *preliminary_cur_1 = node_stack_pop (path_stack_1);
            cur_2 = node_stack_pop (path_stack_2);
            bin_node_t *preliminary_next_1 = node_stack_shadow_pop (path_stack_1);
            next_2 = node_stack_shadow_pop (path_stack_2);

            if (!first_common_char) {

                if (next_2 == preliminary_next_1) {

                    printf (",");

                } else {

                    printf (" and");
                }
            }

            if (next_1 == cur_1->pos) {

                printf (" %s", cur_1->data);

            } else {

                printf (" not %s", cur_1->data);
            }

            cur_1 = preliminary_cur_1;
            next_1 = preliminary_next_1;
            first_common_char = false;

        } while (next_1 == next_2);
        printf (",");

        printf (" but %s is", obj_name_1);

    } else {

        printf ("\n%s and %s have nothing in common, %s is", obj_name_1, obj_name_2, obj_name_1);
    }

    do {

        if (next_1 == cur_1->pos) {

            printf (" %s,", cur_1->data);

        } else {

            printf (" not %s,", cur_1->data);
        }

        cur_1 = node_stack_pop (path_stack_1);
        next_1 = node_stack_shadow_pop (path_stack_1);

    } while (next_1 != NULL);

    printf (" and %s is", obj_name_2);
    do {

        if (next_2 == cur_2->pos) {

            printf (" %s,", cur_2->data);

        } else {

            printf (" not %s,", cur_2->data);
        }

        cur_2 = node_stack_pop (path_stack_2);
        next_2 = node_stack_shadow_pop (path_stack_2);

    } while (next_2 != NULL);

    printf (" respectively\n");
}

void ak_vis (bin_tree_t *tree) {

    char *pic_name = (char *) calloc (MAX_FILE_NAME + 1, sizeof (char));
    printf ("\nEnter the name of the scheme\n");
    scanf ("%61s", pic_name);
    flush_input_buffer (pic_name [0]);

    bin_tree_vis_dump (tree, strcat (pic_name, ".png"));
}

bool restart () {

    char restart_ans = 0;
    while (true) {

        printf ("\nWanna repeat, my dude?\n");
        scanf ("%c", &restart_ans);
        flush_input_buffer (restart_ans);
        if (restart_ans == YES || restart_ans == NO) {

            break;
        }

        printf ("\nWrong input format, try again\n");
    }

    if (restart_ans == NO) {

        return false;

    } else {

        return true;
    }
}

void make_ak_tree (bin_tree_t *tree, char *data_buffer) {

    char *data_ptr = data_buffer;
    tree->root = scan_node (&data_ptr);
}

static bin_node_t *scan_node (char **data_ptr) {

    *data_ptr += 2;
    char *line_end = strchr (*data_ptr, '\n');
    *(line_end) = '\0';
    char *data_seg = strdup (*data_ptr);
    *data_ptr = line_end + 1;

    bin_node_t *node = bin_tree_create_leaf (data_seg);

    if (**data_ptr != '}') {

        node->pos = scan_node (data_ptr);
        node->neg = scan_node (data_ptr);
    }
    
    *data_ptr += 2;
    return node;
}

char *download_data_buffer (FILE *database_file) {

    size_t file_size = get_file_size (database_file);
    char *data_buffer = (char *) calloc (file_size + 1, sizeof (char));

    fread (data_buffer, sizeof (char), file_size, database_file);
    return data_buffer;
}

void upload_tree_as_database (bin_tree_t *tree, FILE *database_file) {

    upload_node (tree->root, database_file);
}

static void upload_node (bin_node_t *node, FILE *database_file) {

    fprintf (database_file, "{\n%s\n", node->data);

    if (node->pos != NULL) {

        upload_node (node->pos, database_file);
        upload_node (node->neg, database_file);
    }

    fprintf (database_file, "}\n");
}
