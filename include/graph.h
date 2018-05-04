//
// Created by remi on 22/04/18.
//

#ifndef PROJETENSI_GRANDPRIX_GRAPH_H
#define PROJETENSI_GRANDPRIX_GRAPH_H


#include <LinkedList.h>

typedef struct graph_t *Graph;

typedef struct vertex_t *Vertex;

typedef struct edge_t *Edge;

struct graph_t {
    Racetrack racetrack;
    Vertex** vertices;
};

struct vertex_t {
    int distanceFromSource;
    int isSetToExplore;
    Point previousVertex;
    LinkedList neighbors; /* edges */
};

struct edge_t {
    Point to;
    int weight;
};

Graph newGraph(Racetrack racetrack);

void graphVertexSetDistance(Graph graph,Point coord,int dist);

int graphVertexGetDistance(Graph graph,Point coord);

void graphVertexSetPrevious(Graph graph,Point coord,Point prevCoord);

Point graphVertexGetPrevious(Graph graph,Point coord);

LinkedList graphVertexGetNeighbors(Graph graph,Point point);

int graphVertexIsInQueue(Graph,Point);

void graphVertexSetInQueue(Graph,Point);

#endif //PROJETENSI_GRANDPRIX_GRAPH_H
