#ifndef DGRAPH_DGRAPH_H
#define DGRAPH_DGRAPH_H

#include <stddef.h>

typedef void * Type;
typedef struct dgraph_struct * DGraph;
typedef enum {FALSE, TRUE} Bool;
typedef int (*fcmp) (Type, Type);

DGraph createDGraph(size_t elementSize, fcmp cmpTag, fcmp cmpData);

#endif //DGRAPH_DGRAPH_H
