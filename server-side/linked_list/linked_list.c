//
// Created by mikulec on 12/27/23.
//

#include <string.h>
#include "linked_list.h"

void ls_create(LINKED_LIST* ls, const size_t item_size)
{
    ls->size = 0;
    ls->item_size = item_size;
    ls->first = NULL;
    ls->last = NULL;
}

void ls_destroy(LINKED_LIST* ls)
{
    if (ls->size > 0) {
        NODE* tmp = ls->first;
        do {
            free(tmp->item);
            NODE* help = tmp;
            tmp = help->next;
            free(help);
        } while (tmp != NULL);
    }
    ls->first = NULL;
    ls->last = NULL;
    ls->size = 0;
    ls->item_size = 0;
}

unsigned int ls_get_size(const LINKED_LIST* ls)
{
    return ls != NULL ? ls->size : 0;
}

_Bool ls_push(LINKED_LIST* ls, const void* item)
{
    if (ls == NULL || item == NULL)
    {
        return false;
    }

    NODE* node = malloc(sizeof(NODE));
    node->item = malloc(ls->item_size);
    memcpy(node->item, item, ls->item_size);
    node->next = NULL;

    if (ls->first == NULL)
    {
        ls->first = node;
        ls->last = node;
        ls->size++;
        return true;
    }

    ls->last->next = node;
    ls->last = node;
    ls->size++;
    return true;
}

_Bool ls_push_index(LINKED_LIST* ls, const void* item, const unsigned int index)
{
    if (ls == NULL || item == NULL || index > ls->size)
    {
        return false;
    }

    if (ls->size == index)
    {
        return ls_push(ls, item);
    }
    NODE* node = malloc(sizeof(NODE));
    node->item = malloc(ls->item_size);
    memcpy(node->item, item, ls->item_size);

    if (index == 0) {
        node->next = ls->first;
        ls->first = node;
        ls->size++;
        return true;
    }

    NODE* tmp = ls_get_node(ls, index - 1);

    node->next = tmp->next;
    tmp->next = node;
    ls->size++;
    return true;
}

bool ls_pop(LINKED_LIST* ls, void* item)
{
    if (ls == NULL || ls->size == 0)
    {
        return false;
    }

    if (ls->size == 1)
    {
        memcpy(item, ls->first->item, ls->item_size);
        free(ls->first->item);
        free(ls->first);
        ls->first = NULL;
        ls->last = NULL;
        ls->size--;
        return true;
    }
    memcpy(item, ls->last->item, ls->item_size);
    free(ls->last->item);
    free(ls->last);
    ls->last = ls_get_node(ls, ls->size - 2);
    ls->last->next = NULL;
    ls->size--;
    return true;
}

bool ls_pop_index(LINKED_LIST* ls, void* item, const unsigned int index)
{
    if (ls == NULL || index >= ls->size)
    {
        return false;
    }

    if (index == ls->size - 1)
    {
        return ls_pop(ls, item);
    }

    NODE* previous = ls_get_node(ls, index - 1);
    NODE* popped = previous->next;
    previous->next = popped->next;

    memcpy(item, popped->item, ls->item_size);
    free(popped->item);
    popped->next = NULL;
    free(popped);
    ls->size--;
    return true;
}

bool ls_get(const LINKED_LIST* ls, void* item, const unsigned int index)
{
    if (ls == NULL || ls->size == 0)
    {
        return false;
    }
    memcpy(item ,ls_get_node(ls, index)->item, ls->item_size);
    return true;
}

NODE* ls_get_node(const LINKED_LIST* ls, const unsigned int index)
{
    if (index == 0)
    {
        return ls->first;
    }
    if (index == ls->size - 1)
    {
        return ls->last;
    }
    NODE* tmp = ls->first;
    for (int i = 0; i < index; i++)
    {
        tmp = tmp->next;
    }
    return tmp;
}

void ls_iterator_init(LINKED_LIST_ITERATOR* self, LINKED_LIST* list)
{
    self->list = list;
    self->item = self->list->first;
    self->index = 0;
}

void ls_iterator_destroy(LINKED_LIST_ITERATOR* self)
{
    self->list = NULL;
    self->item = NULL;
    self->index = 0;
}

bool ls_iterator_has_next(LINKED_LIST_ITERATOR* self)
{
    return self->index < self->list->size;
}

void* ls_iterator_move_next(LINKED_LIST_ITERATOR* self)
{
    void* item = self->item->item;
    self->item = self->item->next;
    self->index++;
    return item;
}

void ls_access_at(LINKED_LIST *self, void **item, unsigned int index) {
    if (index > self->size)
    {
        return;
    }
    *item = ls_get_node(self, index)->item;
}

void ls_run_function(LINKED_LIST *self, void (*function)(void *)) {
    LINKED_LIST_ITERATOR iterator;
    ls_iterator_init(&iterator, self);
    while (ls_iterator_has_next(&iterator)) {
        void* item = ls_iterator_move_next(&iterator);
        function((void**)item);
    }
}
