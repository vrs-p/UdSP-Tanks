//
// Created by mikulec on 12/27/23.
//

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef SERVER_SIDE_LINKED_LIST_H
#define SERVER_SIDE_LINKED_LIST_H

typedef struct node
{
    void* item;
    struct node* next;
} NODE;

typedef struct linked_list
{
    size_t size;
    size_t item_size;
    struct node* first;
    struct node* last;
} LINKED_LIST;

typedef struct linked_list_iterator
{
    LINKED_LIST* list;
    int index;
    NODE* item;
} LINKED_LIST_ITERATOR;

void ls_create(LINKED_LIST* ls, size_t item_size);
void ls_destroy(LINKED_LIST* ls);

unsigned int ls_get_size(const LINKED_LIST* ls);
bool ls_push(LINKED_LIST* ls, const void* item);
bool ls_push_index(LINKED_LIST* ls, const void* item, unsigned int index);
bool ls_pop(LINKED_LIST* ls, void* item);
bool ls_pop_index(LINKED_LIST* ls, void* item, unsigned int index);

bool ls_get(const LINKED_LIST* ls, void* item, unsigned int index);
NODE* ls_get_node(const LINKED_LIST* ls, unsigned int index);
void ls_access_at(LINKED_LIST* self, void** item, unsigned int index);
void ls_run_function(LINKED_LIST* self, void (*function)(void*));

void ls_iterator_init(LINKED_LIST_ITERATOR* self, LINKED_LIST* list);
void ls_iterator_destroy(LINKED_LIST_ITERATOR* self);
_Bool ls_iterator_has_next(LINKED_LIST_ITERATOR* self);
void* ls_iterator_move_next(LINKED_LIST_ITERATOR* self);
void ls_iterator_reset(LINKED_LIST_ITERATOR* self);

#endif //SERVER_SIDE_LINKED_LIST_H
