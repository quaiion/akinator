#ifndef AKINATOR_ACTIVE
#define AKINATOR_ACTIVE

#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include <string.h>

constexpr int STRINGS_EQUAL = 0;
constexpr int NO_OFFSET = 0;

struct node_t {

    char *data;
    node_t *pos;
    node_t *neg;
};

enum AK_MODE {GAME = 1, DEFINE = 2, COMPARE = 3, VISUALIZE = 4};
enum AK_MEM_MODE {EXISTING = 'y', NEW = 'n'};
enum AK_SAVE_MODE {SAVE = 'y', LEAVE = 'n'};
enum AK_USER_ANSWER {YES = 'y', NO = 'n'};

constexpr MAX_OBJ_NAME = 16;       // Константы классные конечно, но в спецификаторах между процентом и s константу не впихнешь
constexpr MAX_OBJ_CHAR = 32;



struct stack_t {

    node_t **data;
    ssize_t capacity;
    ssize_t min_capacity /* >= 1 */;
    ssize_t size;
};

constexpr unsigned char BROKEN_BYTE = 0xFF;
constexpr int OS_RESERVED_ADDRESS = 1;

void stack_ctor (stack_t *stack, ssize_t min_capacity = 8);
void stack_dtor (stack_t *stack);
void stack_push (stack_t *stack, int value);
node_t stack_pop (stack_t *stack);
node_t stack_shadow_pop (stack_t *stack);

#endif

/*

Протестировать абсурдный ввод номера режима
См. строку 54 akinator.cpp
Запрет свойств на "не"
Мб есть смысл вписать возможность выхода и перевыбора режима
Доработать визуализатор (если потребуется)
Ну мб есть смысл допилить буферизацию выгрузки базы
Ассерты

*/
