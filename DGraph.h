#ifndef DGRAPH_DGRAPH_H
#define DGRAPH_DGRAPH_H

#include <stddef.h>
#include "Contenedores/List.h"

typedef void * Type;
typedef struct dgraph_struct * DGraph;
typedef enum {FALSE, TRUE} Bool;
typedef int (*fcmp) (Type, Type);

DGraph createDGraph(size_t elementSize, size_t tagSize, fcmp cmpTag, fcmp cmpData);
void addVertex(DGraph, Type data, Type tag);
Bool adjacent(DGraph, Type, Type);
List neighbors(DGraph, Type);
void setVertexData(DGraph, Type, Type);
void addEdge(DGraph, Type src, Type dst, Type tag);

#endif //DGRAPH_DGRAPH_H
