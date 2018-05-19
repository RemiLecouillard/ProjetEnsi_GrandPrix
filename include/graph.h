//
// Created by remi on 22/04/18.
//

#ifndef PROJETENSI_GRANDPRIX_GRAPH_H
#define PROJETENSI_GRANDPRIX_GRAPH_H


#include <LinkedList.h>

#include <racetrack.h>

typedef struct graph_t *Graph;

typedef struct vertex_t *Vertex;

typedef struct edge_t *Edge;

typedef struct vertexvelocity *VertexVelocity;

typedef struct edgevelocity *EdgeVelocity;

struct graph_t {
    Racetrack racetrack;
    Vertex** vertices;
};

struct vertex_t {
    int distanceFromSource;
    int isSetToExplore;
    int isDijkstraPath;
    Point previousVertex;
    LinkedList neighbors; /* edges */
    VertexVelocity inPutVelocity[11][11];
};

struct edge_t {
    Point to;
    int weight;
};

struct vertexvelocity {
    LinkedList possibleDestination;
    int pheromone;
};

struct edgevelocity {
    Point to;
    Vector acceleration; // x e [-2; 2] y  e [-2; 2]
    int gasolineCost;
    int boostCost;
};

Graph newGraph(Racetrack racetrack);

void graphVertexSetDistance(Graph graph,Point coord,int dist);

int graphVertexGetDistance(Graph graph,Point coord);

void graphVertexSetPrevious(Graph graph,Point coord,Point prevCoord);

Point graphVertexGetPrevious(Graph graph,Point coord);

LinkedList graphVertexGetNeighbors(Graph graph,Point point);

int graphVertexIsInQueue(Graph,Point);

void graphVertexSetInQueue(Graph,Point);

void graphVertexSetDijkstraPath(Graph, Point, int);

int graphVertexIsDijkstraPath(Graph, Point);

Vector graphGetDirectionWithMostPheromone(Graph this, Point from, Vector velocity, Point *to);

LinkedList graphVertexVelocityGetNeighbors(Graph this, Point from, Vector velocity);

int graphVertexVelocityGetPheromone(Graph this, Point from, Vector velocity);

void graphVertexVelocitySetPheromone(Graph this, Point from, Vector velocity, int pheromone);

void graphVertexVelocityForEach(Graph this, void (*foreach)(VertexVelocity));

#endif //PROJETENSI_GRANDPRIX_GRAPH_H
