#include "DGraph.h"
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
    new->vertex = listCreate(sizeof(struct vertex_struct));
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
    if (vertexExists(DG, tag)) return;  // Prevent from adding duplicates

    Vertex new = malloc(sizeof(struct vertex_struct));
    if (!new) return;

    new->neighbours = listCreate(sizeof(struct vertex_struct));

    // Copy data to new directions
    Type tempData = malloc(DG->elementSize);
    memcpy(tempData, data, DG->elementSize);
    new->data = tempData;

    Type tempTag = malloc(DG->tagSize);
    memcpy(tempTag, tag, DG->tagSize);
    new->tag = tempTag;

    listAdd(DG->vertex, new);
    DG->size++;
}


//Retorna la direccion del vector cuando la comparacion es exitosa
Vertex getVertex(List vert, fcmp cmpTag, Type d1, unsigned int size){
    Vertex current = NULL;
    for(int i=0; i<size; i++){
        current = listGet(vert, i);
        if(cmpTag(current->tag, d1) == 0){
            return current;
        }
    }
    return current;
}


Bool adjacent(DGraph dgraph, Type tag1, Type tag2) {
    if (dgraph != NULL) {
        Vertex current, current2, current3;
        current = getVertex(dgraph->vertex, dgraph->cmpTag, tag1, dgraph->size);
        current2 = getVertex(dgraph->vertex, dgraph->cmpTag, tag2, dgraph->size);
        if (current) {
            current3 = getVertex(current->neighbours, dgraph->cmpTag, tag2, listSize(current->neighbours));
            if (current3) {
                return TRUE;
            } else if (current2) {
                current3 = getVertex(current->neighbours, dgraph->cmpTag, tag2, listSize(current->neighbours));
                if (current3) {
                    return TRUE;
                }
            }
        }
        return FALSE;
    }
}

List neighbors(DGraph dgraph, Type tag){
    //Solo falta retornarse a si mismo. Pense en añadirlo a la lista de neighbors, pero no sé...
    if(dgraph != NULL){
        Vertex current;
        current = getVertex(dgraph->vertex,dgraph->cmpTag,tag,dgraph->size);
        if(current){
            return current->neighbours;
        }
    }
    return NULL;
}

void setVertexData(DGraph dgraph, Type tag, Type data){
    if(dgraph != NULL){
        Vertex current;
        current = getVertex(dgraph->vertex, dgraph->cmpTag, tag, dgraph->size);
        if(current){
            Type d = malloc(sizeof(dgraph->elementSize));
            memcpy(d,data,dgraph->elementSize);
            current->data = d;
        }
    }
}

Type getVertexData(DGraph dgraph, Type tag){
    if(dgraph != NULL){
        Vertex  current;
        current = getVertex(dgraph->vertex, dgraph->cmpTag, tag, dgraph->size);
        if(current){
            return current->data;
        }
    }
    return NULL;
}






