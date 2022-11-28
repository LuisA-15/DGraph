#include "DGraph.h"
#include "Contenedores/List.h"
#include <stdlib.h>
#include <string.h>

struct dgraph_struct {
    List vertex;
    List edge;
    int size;
    size_t elementSize;
    size_t tagSize;
    fcmp cmpTag;
    fcmp cmpData;
};

struct vertex_struct {
    List neighbours;
    Type data;
    Type tag;
};
typedef struct vertex_struct * Vertex;

struct edge_struct {
    Vertex source;
    Vertex destination;
    Type tag;
};
typedef struct edge_struct * Edge;

DGraph createDGraph(size_t elementSize, size_t tagSize, fcmp cmpTag, fcmp cmpData) {
    DGraph new = malloc(sizeof(struct dgraph_struct));
    new->vertex = listCreate(sizeof(struct vertex_struct)); // testing
    new->edge = listCreate(sizeof(Edge));
    new->size = 0;
    new->elementSize = elementSize;
    new->tagSize = tagSize;
    new->cmpTag = cmpTag;
    new->cmpData = cmpData;

    return new;
}

Bool vertexExists(DGraph DG, Type tag) {
    Vertex current;

    for (int i = 0; i < DG->size; i++) {
        current = listGet(DG->vertex, i);
        if (DG->cmpTag(tag, current->tag) == 0)
            return TRUE;
    }
    return FALSE;
}

void addVertex(DGraph DG, Type data, Type tag) {
    if (vertexExists(DG, tag)) return;

    Vertex new = malloc(sizeof(struct vertex_struct));
    if (!new) return;

    new->neighbours = listCreate(sizeof(struct vertex_struct));

    Type tempData = malloc(DG->elementSize);
    memcpy(tempData, data, DG->elementSize);
    new->data = tempData;

    Type tempTag = malloc(DG->tagSize);
    memcpy(tempTag, tag, DG->tagSize);
    new->tag = tempTag;

    listAdd(DG->vertex, new);
    DG->size++;
}