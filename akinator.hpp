#ifndef AKINATOR_ACTIVE
#define AKINATOR_ACTIVE

#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include <string.h>
#include "bin_tree.hpp"

#define OS_RESERVED_ADDRESS 1
#define STRINGS_EQUAL 0
#define NO_OFFSET 0

#define EXIT 'e'
enum AK_MODE {GAME = '1', DEFINE = '2', COMPARE = '3', VISUALIZE = '4'};
enum AK_MEM_MODE {EXISTING = 'y', NEW = 'n'};
enum AK_SAVE_MODE {SAVE = 'y', LEAVE = 'n'};
enum AK_USER_ANSWER {YES = 'y', NO = 'n'};

enum LOAD_DATABASE_RESULT {DATABASE_EXITED, DATABASE_ENTERED};

constexpr int MAX_OBJ_NAME = 16;       // Константы классные конечно, но в спецификаторах между % и s константу не впихнешь
constexpr int MAX_OBJ_CHAR = 32;

void ak_game_init ();
void ak_def_init ();
void ak_comp_init ();
void ak_vis_init ();
bool restart ();

void ak_game (bin_tree_t *tree);
void ak_def (bin_tree_t *tree, char *obj_name);
void ak_comp (bin_tree_t *tree, char *obj_name_1, char *obj_name_2);
void ak_vis (bin_tree_t *tree);

LOAD_DATABASE_RESULT load_database (bin_tree_t *tree);
void save_database (bin_tree_t *tree);
void make_ak_tree (bin_tree_t *tree, char *data_buffer);
char *download_data_buffer (FILE *database_file);
void upload_tree_as_database (bin_tree_t *tree, FILE *database_file);

#endif

/*

Она пока что умеет искать свойства свойств (убрать или оставить?)
Надо бы про буфер ввода зашарить нормально, а то проблему-то я зафиксил но не очень понял как
Есть че подредить касательно ввода слов и многозначных номеров
Запрет свойств на "не"
Коды исполнения (возможно, у некоторых функций надо поменять возвращение и так обработанного параметра на что-то более полезное)
В definition не работает e
Многословные свойства
Вопрос про сохранение базы без ее изменения
Через пару дней я понял, что в dump_node () в целом можно было просто использовать их адреса в памяти как идентификаторы - переписывать?

*/
