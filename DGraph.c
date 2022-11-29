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

Vertex getVertex(List vert, fcmp cmpTag, Type tag, unsigned int size);

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

Bool edgeExists(DGraph DG, Type src, Type dst) {
    // Find vertex that has the src tag
    Vertex vSrc = getVertex(DG->vertex, DG->cmpTag, src, DG->size);

    if (listSize(vSrc->neighbours) == 0) return FALSE;
    // Return False if no other neighbour has the dst tag
    Vertex current;
    for (int i = 0; i < listSize(vSrc->neighbours); i++) {
        current = listGet(vSrc->neighbours, i);
        if (DG->cmpTag(current->tag, dst) == 0) return TRUE;
    }
    return FALSE;
}

void addEdge(DGraph DG, Type src, Type dst, Type tag) {
    Vertex vSrc = getVertex(DG->vertex, DG->cmpTag, src, DG->size);
    if (!vSrc || !vertexExists(DG, dst)) return;
    if (edgeExists(DG, src, dst)) return;

    Edge new = malloc(sizeof(struct edge_struct));
    if (!new) return;

    Type tempSrc = malloc(DG->tagSize);
    Type tempDst = malloc(DG->tagSize);
    Type tempTag = malloc(DG->tagSize);
    memcpy(tempSrc, src, DG->tagSize);
    memcpy(tempDst, dst, DG->tagSize);
    memcpy(tempTag, tag, DG->tagSize);
    new->source = tempSrc;
    new->destination = tempDst;
    new->tag = tempTag;

    listAdd(DG->edge, new);
    listAdd(vSrc->neighbours, new);
}



//Retorna la direccion del vector cuando la comparacion es exitosa
Vertex getVertex(List vert, fcmp cmpTag, Type tag, unsigned int size) {
    Vertex current = NULL;
    for(int i = 0; i < size; i++){
        current = listGet(vert, i);
        if(cmpTag(current->tag, tag) == 0){
            return current;
        }
    }
    return NULL;
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
            }
        }
        else if (current2) {
                current3 = getVertex(current2->neighbours, dgraph->cmpTag, tag1, listSize(current->neighbours));
                if (current3) {
                    return TRUE;
                }
            }
        }
        return FALSE;
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






