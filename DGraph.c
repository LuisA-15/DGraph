#include "DGraph.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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
    Type source;
    Type destination;
    Type tag;
};
typedef struct edge_struct * Edge;

Vertex getVertex(List vert, fcmp cmpTag, Type tag, unsigned int size);

DGraph createDGraph(size_t elementSize, size_t tagSize, fcmp cmpTag, fcmp cmpData) {
    DGraph new = malloc(sizeof(struct dgraph_struct));
    new->vertex = listCreate(sizeof(struct vertex_struct));
    new->edge = listCreate(sizeof(struct edge_struct));
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
    Vertex vDst = getVertex(DG->vertex, DG->cmpTag, dst, DG->size);

    if (!vSrc || !vDst) return;
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
    listAdd(vSrc->neighbours, vDst);
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

Edge getEdge(List edges, fcmp cmpTag, Type src, Type dst) {
    Edge current = NULL;
    for (int i = 0; i < listSize(edges); i++) {
        current = listGet(edges, i);
        if (cmpTag(current->source, src) == 0 && cmpTag(current->destination, dst) == 0)
            return current;
    }
    return NULL;
}

Bool adjacent(DGraph dgraph, Type tagX, Type tagY) {
    if (!dgraph) return FALSE;

    Vertex x = getVertex(dgraph->vertex, dgraph->cmpTag, tagX, dgraph->size);
    Vertex y = getVertex(dgraph->vertex, dgraph->cmpTag, tagY, dgraph->size);

    if (x && y) {
        if (getVertex(x->neighbours, dgraph->cmpTag, tagY, listSize(x->neighbours)))
            return TRUE;
        if (getVertex(y->neighbours, dgraph->cmpTag, tagX, listSize(y->neighbours)))
            return TRUE;
    }
    return FALSE;
}


List neighbors(DGraph dgraph, Type tag) {
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

void setVertexData(DGraph dgraph, Type tag, Type data) {
    if(dgraph != NULL){
        Vertex current;
        current = getVertex(dgraph->vertex, dgraph->cmpTag, tag, dgraph->size);
        if(current){
            memcpy(current->data, data, dgraph->elementSize);
        }
    }
}

Type getVertexData(DGraph dgraph, Type tag) {
    if(dgraph != NULL){
        Vertex  current;
        current = getVertex(dgraph->vertex, dgraph->cmpTag, tag, dgraph->size);
        if(current){
            return current->data;
        }
    }
    return NULL;
}

void setEdgeTag(DGraph DG, Type src, Type dst, Type newTag) {
    Edge e = getEdge(DG->edge, DG->cmpTag, src, dst);
    if (!e) return;

    memcpy(e->tag, newTag, DG->tagSize);
}

Type getEdgeTag(DGraph DG, Type src, Type dst) {
    Edge e = getEdge(DG->edge, DG->cmpTag, src, dst);
    if (!e) return NULL;

    return e->tag;
}

int DGraphSize(DGraph DG) {
    return DG->size;
}

void removeVertex(DGraph DG, Type tag) {
    Vertex x = getVertex(DG->vertex, DG->cmpTag, tag, DG->size);
    if (!x) return;

    // Erase from neighbours lists

    unsigned int edgeCount = listSize(DG->edge);
    Edge currentEdge;
    Vertex v;
    for (int i = 0; i < edgeCount; i++) {
        currentEdge = listGet(DG->edge, i);
        if (DG->cmpTag(tag, currentEdge->destination)) {
            v = getVertex(DG->vertex, DG->cmpTag, currentEdge->destination, DG->size);
            Vertex currentVertex;
            for (int j = 0; j < listSize(v->neighbours); j++) {
                currentVertex = listGet(v->neighbours, j);
                if (DG->cmpTag(tag, currentVertex->tag) == 0) {
                    listRemove(v->neighbours, j);
                    break;
                }
            }
            // Remove from edge list
            listRemove(DG->edge, i);
            edgeCount--;
        } else if (DG->cmpTag(tag, currentEdge->source)) {
            free(currentEdge->source);
            free(currentEdge->destination);
            free(currentEdge->tag);
            listRemove(DG->edge, i);
            edgeCount--;
        }
    }

    listDestroy(x->neighbours);
    free(x->tag);
    free(x->data);
    free(x);
    DG->size--;
}

void removeEdge(DGraph DG, Type src, Type dst) {
    Vertex vSrc = getVertex(DG->vertex, DG->cmpTag, src, DG->size);
    Vertex vDst = getVertex(DG->vertex, DG->cmpTag, dst, DG->size);
    if (!vSrc || !vDst) return;

    // Remove from edge list
    Edge current;
    for (int i = 0; i < listSize(DG->edge); i++) {
        current = listGet(DG->edge, i);
        if (DG->cmpTag(current->source, src) == 0 && DG->cmpTag(current->destination, dst) == 0) {
            free(current->destination);
            free(current->source);
            free(current->tag);
            listRemove(DG->edge, i);
            break;
        }
    }

    // Remove link in neighbours list
    Vertex currentV;
    for (int i = 0; i < listSize(vSrc->neighbours); i++) {
        currentV = listGet(vSrc->neighbours, i);
        if (DG->cmpTag(currentV->tag, dst)) {
            listRemove(vSrc->neighbours, i);
            break;
        }
    }
}

void destroyDG(DGraph DG) {
    Vertex currentV;
    for (int i = 0; i < listSize(DG->vertex); i++) {
        currentV = listGet(DG->vertex, 0);
        free(currentV->tag);
        free(currentV->data);
        listDestroy(currentV->neighbours);
        listRemove(DG->vertex, 0);
    }
    free(DG->vertex);
    Edge currentE;
    for (int i = 0; i < listSize(DG->edge); i++) {
        currentE = listGet(DG->edge, 0);
        free(currentE->source);
        free(currentE->destination);
        free(currentE->tag);
        listRemove(DG->edge, 0);
    }
    free(DG->edge);

    free(DG);
}