#include "List.h"
#include <string.h>

struct node_struct {
    struct node_struct * prior;
    struct node_struct * next;
    Type data;
};
typedef struct node_struct * Node;

struct list_struct {
    Node first;
    Node last;
    unsigned int size;
    size_t elementSize;
    fcmp cmp;
};

List listCreate(size_t bytes, fcmp cmp) {
    List new = malloc(sizeof(struct list_struct));
    if (!new) return NULL;

    new->first = NULL;
    new->last = NULL;
    new->size = 0;
    new->elementSize = bytes;
    new->cmp = cmp;

    return new;
}

Node newNode(Type data, size_t bytes) {
    Node new = malloc(sizeof(struct node_struct));
    if (!new) return NULL;

    new->prior = NULL;
    new->next = NULL;

    Type temp = malloc(bytes);
    if (!temp) return NULL;
    mempcpy(temp, data, bytes);

    new->data = temp;

    return new;
}

void listAdd(List list, Type data) {
    if (!list) return;

    Node new = newNode(data, list->elementSize);
    if (!new) return;

    new->prior = list->last;
    if (new->prior) {
        list->last->next = new;
    } else {
        list->first = new;
    }
    list->last = new;

    list->size++;
}

unsigned int listSize(List list) {
    if (!list) return -1;
    return list->size;
}

void listDestroy(List list) {
    Type temp;
    while(list->size) {
        temp = listRemove(list, 0);
        free(temp);
    }
    free(list);
}

Type listGet(List list, int index) {
    if (!list) return NULL;
    if (index < 0 || index >= list->size) return NULL;

    Node current = list->first;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }

    Type temp = malloc(list->elementSize);
    if (!temp) return NULL;
    mempcpy(temp, current->data, list->elementSize);

    return temp;
}

void listSet(List list, int index, Type data) {
    if (!list || !list->size) return;
    if (index < 0 || index >= list->size) return;

    Node current = list->first;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }
    mempcpy(current->data, data, list->elementSize);
}

Type listRemove(List list, int index) {
    if(!list) return NULL;
    if (index < 0 || index >= list->size) return NULL;

    Node current = list->first;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }

    if (index == 0) {   // Is the first
        list->first = current->next;
        if (current->next)  // Is only the first
            list->first->prior = NULL;
        else    // Is also the last
            list->last = NULL;

    } else if (index == list->size - 1) {   // Is only the last
        list->last = current->prior;
        list->last->next = NULL;

    } else {    // Is in the middle
        current->next->prior = current->prior;
        current->prior->next = current->next;
    }
    list->size--;

    Type temp = malloc(list->elementSize);
    if (!temp) return NULL;
    mempcpy(temp, current->data, list->elementSize);

    free(current->data);
    free(current);

    return temp;
}

void listInsert(List list, int index, Type data) {
    if(!list) return;
    if (!list->size && index == 0) {
        listAdd(list, data);
        return;
    }
    if (index < 0 || index >= list->size) return;

    Node current = list->first;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }

    Node new = newNode(data, list->elementSize);
    if (!new) return;

    new->next = current;
    new->prior = current->prior;
    current->prior = new;

    if (new->prior)
        new->prior->next = new;
    else
        list->first = new;

    list->size++;
}

int listIndexOf(List list, Type data) {
    if (!list) return -1;

    Node current = list->first;
    for (int i = 0; i < list->size; i++) {
        char *d1 = (char *) current->data;
        char *d2 = (char *) data;

        if (list->cmp(d1, d2) == 0) {
            return i;
        }
        current = current->next;
    }
    return -1;
}