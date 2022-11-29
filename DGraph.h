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
void addEdge(DGraph, Type src, Type dst, Type tag);
void setVertexData(DGraph, Type tag, Type data);
Type getVertexData(DGraph dgraph, Type tag);
void setEdgeTag(DGraph, Type src, Type dst, Type newTag);
Type getEdgeTag(DGraph, Type src, Type dst);
Bool adjacent(DGraph, Type tagX, Type tagY);
List neighbors(DGraph, Type tag);
int DGraphSize(DGraph);
void removeVertex(DGraph, Type tag);
void removeEdge(DGraph, Type src, Type dst);
void destroyDG(DGraph);

#endif //DGRAPH_DGRAPH_H
