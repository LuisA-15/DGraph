#ifndef LIST_LIST_H
#define LIST_LIST_H

#include <stdlib.h>

typedef void * Type;
typedef struct list_struct * List;
typedef int (*fcmp) (Type, Type);

List listCreate(size_t, fcmp);
void listAdd(List, Type);
unsigned int listSize(List);
void listDestroy(List);
Type listGet(List, int);
void listSet(List, int, Type);
Type listRemove(List, int);
void listInsert(List, int, Type);
int listIndexOf(List, Type);

#endif //LIST_LIST_H
