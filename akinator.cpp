#include "akinator.hpp"

void ak_game_init () {

    printf ("\nUse the exising database?\nprint y if yes, n if no\n");

    char mem_mode = 0;
    while (true) {

        printf ("\nUse the exising database?\nprint y if yes, n if no\n");
        scanf ("%c", &mem_mode);
        if (mem_mode == EXISTING || mem_mode == NEW) {

            break;
        }

        printf ("\nWrong input format, try again\n");
        while (getchar () != '\n') ;
    }

    switch (mem_mode) {

        case EXISTING: {

            node_t *root = load_database ();

            node_t *new_root = NULL;
            bool round = true;

            while (round) {

                new_root = ak_game (root);
                round = restart ();
            }

            save_database (new_root);
            free_tree (new_root);

            break;
        }

        case NEW: {

            node_t *root = (node_t *) calloc (1, sizeof (node_t));
            root->data = "nobody";
            node_t *new_root = NULL;
            bool round = true;

            while (round) {

                new_root = ak_game (root);
                round = restart ();
            }

            save_database (new_root);
            free_tree (new_root);

            break;
        }
    }
}

void ak_def_init () {

    node_t *root = load_database ();
    bool round = true;

    while (round) {

        char *obj_name = (char *) calloc (MAX_OBJ_NAME + 1, sizeof (char));
        printf ("\nDefinition of which object would you like to see?");
        scanf ("%16s", obj_name);

        ak_def (root, obj_name);
        free (obj_name);
        round = restart ();
    }

    free_tree (root);
}

void ak_comp_init () {

    node_t *root = load_database ();
    bool round = true;

    while (round) {

        char *obj_name_1 = (char *) calloc (MAX_OBJ_NAME + 1, sizeof (char)), *obj_name_2 = (char *) calloc (MAX_OBJ_NAME + 1, sizeof (char));
        printf ("\nEnter the name of the first object you want to be compared");
        scanf ("%32s", obj_name_1);
        printf ("\nEnter the name of the second object you want to be compared");
        scanf ("%32s", obj_name_2);

        ak_comp (root, obj_name_1, obj_name_2);
        free (obj_name_1);
        free (obj_name_2);
        round = restart ();
    }

    free_tree (root);
}

void ak_vis_init () {

    node_t *root = load_database ();

    ak_vis (root);
    free_tree (root);
}

node_t *load_database () {

    char file_name = (char *) calloc (_MAX_FNAME + 1, sizeof (char));
    FILE *database_file = NULL;
    while (true) {

        printf ("\nInsert the name of a database file");
        scanf ("%s", file_name);

        database_file = fopen (file_name, "r");
        if (database_file) {

            break;
        }

        printf ("\nNo such file in the directory, try again\n");
        while (getchar () != '\n') ;
    }
    free (file_name);

    char *data_buffer = dload_database (database_file);
    fclose (database_file);

    node_t *root = make_tree (data_buffer);
    free (data_buffer);

    return root;
}

void save_database (node_t *root) {

    char save_mode = 0;
    while (true) {

        printf ("\nSave the new database?\n*Print \"y\" if yes, \"n\" if no*");
        scanf ("%c", &save_mode);
        if (save_mode == SAVE || save_mode == LEAVE) {

            break;
        }

        printf ("\nWrong input format, try again\n");
        while (getchar () != '\n') ;
    }
    
    if (save_mode == SAVE) {

        char file_name = (char *) calloc (_MAX_FNAME + 1, sizeof (char));

        printf ("\nEnter the name of the new database file");
        scanf ("%256s", file_name);
        FILE *database_file = fopen (file_name, "w");
        free (file_name);

        uload_database (database_file, root);
        fclose (database_file);
    }
}

node_t *ak_game (node_t *root) {            //  Технически тут можно развалить функцию на 2 подфункции; философский вопрос - а надо?

    node_t *cur = root, *pre_cur = NULL;
    bool branch = true;
    while (cur->pos) {

        char ans = 0;
        while (true) {

            printf ("\nIs this object %s?", cur->data);
            scanf ("%c", &ans);
            if (ans = YES || ans = NO) {

                break;
            }

            printf ("\nWrong input format, try again\n");
            while (getchar () != '\n') ;
        }

        switch (ans) {

            case YES: {

                pre_cur = cur;
                branch = true;
                cur = cur->pos;

                break;
            }

            case NO: {

                pre_cur = cur;
                branch = false;
                cur = cur->neg;

                break;
            }
        }
    }

    char ans = 0;
    while (true) {

        printf ("\nIs this object a %s?", cur->data);
        scanf ("%c", &ans);
        if (ans = YES || ans = NO) {

            break;
        }

        printf ("\nWrong input format, try again\n");
        while (getchar () != '\n') ;
    }

    switch (ans) {

        case YES: {

            printf ("\nThat was ez lol\n");
            break;
        }

        case NO: {

            node_t *obj_node = (node_t *) calloc (1, sizeof (node_t));
            obj_node->data = (char *) calloc (MAX_OBJ_NAME + 1, sizeof (char));
            printf ("\nOh.. Alright, I can do with this\n\nWhat object were you thinking about, my dude?");
            scanf ("%16s", obj_node->data);

            node_t *char_node = (node_t *) calloc (1, sizeof (node_t));
            char_node->pos = obj_node;
            char_node->neg = cur;
            char_node->data = (char *) calloc (MAX_OBJ_CHAR + 1, sizeof (char));
            printf ("\nWhat makes it different from the one I suggested?");
            scanf ("%32s", char_node->data);

            if (pre_cur) {

                switch (branch) {

                    case true:
                        pre_cur->pos = char_node;

                    case false:
                        pre_cur->neg = char_node;
                }
            } else {

                root = char_node;
            }

            printf ("\nOk I remembered it\n");

            break;
        }
    }

    return root;
}

void ak_def (node_t *root, char *obj_name) {        // В этой версии определение выводится "снизу вверх", т.к. "сверху вниз" требует стека и неясно вообще нафига

    bool round = true;

    printf ("%s is, as far as I know");
    if (seek_obj (root, obj_name) == true) {
            
        printf ("\n");

    } else {

        printf (", not mentioned in the database\n");
    }
}

bool seek_obj (node_t *node, char *obj_name) {

    if (node == NULL) {

        return false;
    }

    if (strcmp (node->data, obj_name) == STRINGS_EQUAL) {

        return true;
    }

    if (seek_obj (node->pos) == true) {

        printf (", %s", node->data);
        return true;
    }

    if (seek_obj (node->neg) == true) {

        printf (", not %s", node->data);
        return true;
    }

    return false;
}

void ak_comp (node_t *root, char *obj_name_1, char *obj_name_2) {       // Не ловит (вроде как и не должно) случаи, когда одинаковые свойства дублируются в разных бранчах

    stack_t stk_1 = {};
    stack_t *stk_1_p = &stk_1;
    stack_ctor (stk_1_p);
    if (track_obj (root, obj_name_1, stk_1_p) == false) {

        printf ("\n%s is, as far as I know, not mentioned in the database\n", obj_name_1);
        stack_dtor (stk_1_p);
        return;
    }

    if (strcmp (obj_name_1, obj_name_2) == STRINGS_EQUAL) {

        node_t *cur = stack_pop (stk_1_p);
        node_t *next = stack_shadow_pop (stk_1_p);
        if (next != NULL) {

            printf ("\n%s and %s are both", obj_name_1, obj_name_2);
            do {

                if (next == cur->pos) {

                    printf (" %s,", cur->data);

                } else {

                    printf (" not %s,", cur->data);
                }

                cur = stack_pop (stk_1_p);
                next = stack_shadow_pop (stk_1_p);
                
            } while (next != NULL);

        } else {

            printf ("\nNo characteristics of these objects found\n");
        }

        stack_dtor (stk_1_p);
        return;
    } 

    stack_t stk_2 = {};
    stack_t *stk_2_p = &stk_2;
    stack_ctor (stk_2_p);
    if (track_obj (root, obj_name_2, stk_2_p) == false) {

        printf ("\n%s is, as far as I know, not mentioned in the database\n", obj_name_2);
        stack_dtor (stk_1_p);
        stack_dtor (stk_2_p);
        return;
    }

    node_t *cur_1 = stack_pop (stk_1_p);
    node_t *cur_2 = stack_pop (stk_2_p);
    node_t *next_1 = stack_shadow_pop (stk_1_p);
    node_t *next_2 = stack_shadow_pop (stk_2_p);

    if (next_1 == next_2) {

        printf ("\n%s and %s are both", obj_name_1, obj_name_2);
        do {

            if (next_1 == cur_1->pos) {

                printf (" %s,", cur_1->data);

            } else {

                printf (" not %s,", cur_1->data);
            }

            cur_1 = stack_pop (stk_1_p);
            cur_2 = stack_pop (stk_2_p);
            next_1 = stack_shadow_pop (stk_1_p);
            next_2 = stack_shadow_pop (stk_2_p);

        } while (next_1 == next_2);

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

        cur_1 = stack_pop (stk_1_p);
        next_1 = stack_shadow_pop (stk_1_p);

    } while (next_1 != NULL);

    printf (" and %s is", obj_name_2);
    do {

        if (next_2 == cur_2->pos) {

            printf (" %s,", cur_2->data);

        } else {

            printf (" not %s,", cur_2->data);
        }

        cur_2 = stack_pop (stk_2_p);
        next_2 = stack_shadow_pop (stk_2_p);

    } while (next_2 != NULL);

    printf (" respectively\n");

    stack_dtor (stk_1_p);
    stack_dtor (stk_2_p);
}

bool track_obj (node_t *node, char *obj_name, stack_t *stack) {

    if (node == NULL) {

        return false;
    }

    if (strcmp (node->data, obj_name) == STRINGS_EQUAL) {

        stack_push (stack, node);
        return true;
    }

    if (track_obj (node->pos) == true || track_obj (node->neg) == true) {

        stack_push (stack, node);
        return true;
    }

    return false;
}

void ak_vis (node_t *root) {            // В этой версии отсутствует как явление рукодельная буферизация (может, ее стоит добавить?)

    FILE *instr_file = fopen ("vis_instr.gv", "w");
    fprintf (instr_file, "digraph vis{\n\trankdir=TB;");
    print_node (root, instr_file);
    fprintf (instr_file, "\n}");
    fclose (instr_file);

    char *pic_file_name = calloc (_MAX_FNAME, sizeof (char));
    printf ("\nEnter the name of the file to place the scheme");
    scanf ("%256s", pic_file_name);

    /*
    char *cmd = (char *) calloc (_MAX_FNAME + 30, sizeof (char));
    sprintf (cmd, "dot -Tpng vis_instr.gv -o %s", pic_file_name);
    system (cmd);
    free (cmd);
    */
}

int print_node (node_t *node, FILE *instr_file, int ident /* = 0 */) {      // Функция получает на вход НОМЕР текущего шага рекурсии (т.е. НАИМЕНЬШИЙ незанятый идентификатор), возвращает НАИБОЛЬШИЙ занятый идентификатор

    if (node->pos == NULL) {

        fprintf (instr_file, "\n\t%d [shape=record,label=\" <dat> %s | { <neg> NULL | <pos> NULL } \"];", ident, node->data);
        return ident;
    }

    fprintf (instr_file, "\n\t%d [shape=record,label=\" <dat> %s | { <neg> neg | <pos> pos } \"];", ident, node->data);

    int ident_2 = print_node (node->neg, instr_file, ident   + 1);
    int ident_3 = print_node (node->pos, instr_file, ident_2 + 1);
    fprintf (instr_file, "\n\t%d:<neg> -> %d:<dat>;", ident, ident   + 1);
    fprintf (instr_file, "\n\t%d:<neg> -> %d:<dat>;", ident, ident_2 + 1);

    return ident_3;
}

bool restart () {

    char restart_ans = 0;
    while (true) {

        printf ("\nWanna repeat, my dude?");
        scanf ("%c", &restart_ans);
        if (restart_ans = YES || restart_ans = NO) {

            break;
        }

        printf ("\nWrong input format, try again\n");
        while (getchar () != '\n') ;
    }

    if (restart_ans == NO) {

        return false;

    } else {

        return true;
    }
}

node_t *make_tree (char *data_buffer) {

    return make_node ()
}

node_t *make_node (char *data) {


}

char *dload_database (FILE *database_file) {

    size_t file_size = get_file_size (database_file);
    char *data_buffer = (char *) calloc (file_size + 1, sizeof (char));

    fread (data_buffer, sizeof (char), file_size, database_file);
    return data_buffer;
}

void uload_database (FILE *database_file, node_t *root) {


}

void free_tree (node_t *root) {


}

size_t get_file_size (FILE* const file_in) {

    assert (file_in);

    fseek (file_in, NO_OFFSET, SEEK_END);

    size_t filesize = (size_t) ftell (file_in);

    rewind (file_in);
    return filesize;
}
