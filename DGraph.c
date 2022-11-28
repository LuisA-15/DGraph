#include "DGraph.h"
#include "Contenedores/List.h"
#include <stdlib.h>

struct dgraph_struct {
    List vertex;
    List edge;
    int size;
    size_t elementSize;
    fcmp cmpTag;
    fcmp cmpData;
};

struct vertex_struct {
    List neighbours;
    Type data;
    Type tag;
};
typedef struct node_struct * Vertex;

struct edge_struct {
    Vertex source;
    Vertex destination;
    Type tag;
};
typedef struct edge_struct * Edge;

DGraph createDGraph(size_t elementSize, fcmp cmpTag, fcmp cmpData) {
    DGraph new = malloc(sizeof(struct dgraph_struct));
    new->vertex = listCreate(sizeof(Vertex));
    new->edge = listCreate(sizeof(Edge));
    new->size = 0;
    new->elementSize = elementSize;
    new->cmpTag = cmpTag;
    new->cmpData = cmpData;

    return new;
}