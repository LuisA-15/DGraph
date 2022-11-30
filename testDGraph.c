#include <stdio.h>
#include <stdlib.h>
#include "DGraph.h"

struct student_struct {
    int id;
    int age;
};
typedef struct student_struct * Student;

int * newInt(int);
Student newStudent(int, int);
int cmpInt(Type, Type);
void printStudent(Student);
void printTag(Type);
void printNeighbours(List);

int main() {
    // Test data for later usage
    Student s[5] = {
            newStudent(0, 18),
            newStudent(1, 19),
            newStudent(2, 20),
            newStudent(3, 19),
            newStudent(4, 22)
    };
    int *tags[5] = {
            newInt(10),
            newInt(11),
            newInt(12),
            newInt(13),
            newInt(14)

    };

    DGraph DG = createDGraph(sizeof(struct student_struct), sizeof(int), cmpInt);

    printf("**Case 1 Add 1 element**\n\n");
    addVertex(DG, s[0], &s[0]->id);
    printStudent(getVertexData(DG, &s[0]->id));
    printf("--------------------------------\n");

    printf("**Case 2 Add more elements, 1 has a duplicate tag**\n\n");
    addVertex(DG, s[1], &s[1]->id);
    addVertex(DG, s[2], &s[2]->id);
    addVertex(DG, s[3], &s[3]->id);

    addVertex(DG, s[4], &s[2]->id);

    for(int i = 0; i < DGraphSize(DG); i++) {
        printStudent(getVertexData(DG, &s[i]->id));
    }
    printf("--------------------------------\n");

    printf("**Case 3 Add edges, one has a duplicated tag**\n\n");
    addEdge(DG, &s[0]->id, &s[1]->id, tags[0]);
    addEdge(DG, &s[0]->id, &s[2]->id, tags[1]);
    addEdge(DG, &s[2]->id, &s[3]->id, tags[2]);

    addEdge(DG, &s[1]->id, &s[2]->id, tags[0]);

    printTag(getEdgeTag(DG, &s[0]->id, &s[1]->id));
    printTag(getEdgeTag(DG, &s[0]->id, &s[2]->id));
    printTag(getEdgeTag(DG, &s[2]->id, &s[3]->id));

    printTag(getEdgeTag(DG, &s[1]->id, &s[2]->id));

    printf("--------------------------------\n");

    printf("**Case 4 Set new data for Vertex and Edge**\n\n");
    setVertexData(DG, &s[0]->id, s[4]);
    setEdgeTag(DG, &s[0]->id, &s[1]->id, tags[3]);
    for(int i = 0; i < DGraphSize(DG); i++) {
        printStudent(getVertexData(DG, &s[i]->id));
    }
    printTag(getEdgeTag(DG, &s[0]->id, &s[1]->id));
    printf("--------------------------------\n");

    printf("**Case 5 Test neighbours and adjacent**\n\n");
    printf("Vertex 3 - Vertex 2: %d\n", adjacent(DG, &s[3]->id, &s[2]->id));
    printf("Vertex 2 - Vertex 1: %d\n", adjacent(DG, &s[2]->id, &s[1]->id));
    printf("\nVertex 0\n");
    printNeighbours(neighbours(DG, &s[0]->id));
    printf("--------------------------------\n");

    printf("**Case 6 Remove data**\n\n");
    removeVertex(DG, &s[1]->id);
    removeEdge(DG, &s[2]->id, &s[3]->id);
    printf("Vertex 3 - Vertex 2: %d\n", adjacent(DG, &s[3]->id, &s[2]->id));
    printf("\nVertex 0\n");
    printNeighbours(neighbours(DG, &s[0]->id));
    printf("--------------------------------\n");

    destroyDG(DG);

    return 0;
}

int * newInt(int x) {
    int *new = malloc(sizeof(int));
    *new = x;
    return new;
}

Student newStudent(int id, int age) {
    Student new = malloc(sizeof(struct student_struct));
    new->id = id;
    new->age = age;
    return new;
}

int cmpInt(Type d1, Type d2) {
    int *x = d1;
    int *y = d2;

    return *x - *y;
}

void printStudent(Student s) {
    if (s)
        printf("id: %d\tage:%d\n", s->id, s->age);
    else
        printf("id: NULL\tage: NULL\n");
}

void printTag(Type data) {
    int *tag = data;
    if (tag)
        printf("Tag: %d\n", *tag);
    else
        printf("Tag: NULL\n");
}

void printNeighbours(List neighbours) {
    for (int i = 0; i < listSize(neighbours); i++) {
        printStudent(listGet(neighbours, i));
    }
}