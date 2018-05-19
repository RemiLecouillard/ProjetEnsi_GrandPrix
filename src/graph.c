//
// Created by remi on 22/04/18.
//

#include <utils.h>
#include <stdlib.h>
#include <limits.h>
#include <racetrack.h>
#include <antColony.h>
#include "graph.h"
#include "utils.h"

Vertex getVertex(Graph graph,Point coord);

VertexVelocity getVertexVelocity(Graph graph, Point coord, Vector velocity);

Graph newGraph(Racetrack racetrack) {
    int i;
    Graph graph;

    graph = malloc(sizeof(struct graph_t));
    graph->racetrack = racetrack;
    graph->vertices = malloc(racetrack->height * sizeof(Vertex*));
    for (i = 0; i < racetrack->height; i ++) {
        graph->vertices[i] = malloc(racetrack->width * sizeof(Vertex));
    }

    return graph;
}

void graphVertexSetDistance(Graph graph,Point coord,int dist) {
    getVertex(graph, coord)->distanceFromSource = dist;
}

int graphVertexGetDistance(Graph graph,Point coord) {
    return getVertex(graph, coord)->distanceFromSource;
}

void graphVertexSetPrevious(Graph graph,Point coord,Point prevCoord) {
    getVertex(graph, coord)->previousVertex = prevCoord;
}

Point graphVertexGetPrevious(Graph graph,Point coord) {
    return getVertex(graph, coord)->previousVertex;
}

LinkedList graphVertexGetNeighbors(Graph graph,Point point) {
    return getVertex(graph, point)->neighbors;
}

void graphVertexSetDijkstraPath(Graph graph, Point point, int isPath) {
    getVertex(graph, point)->isDijkstraPath = isPath;
}

int graphVertexIsDijkstraPath(Graph graph, Point point) {
    return getVertex(graph, point)->isDijkstraPath;
}

Vector graphGetDirectionWithMostPheromone(Graph this, Point from, Vector velocity, Point *to) {
    VertexVelocity vertexVelocity = getVertexVelocity(this, from, velocity);
    VertexVelocity neighbour;
    EdgeVelocity edgeVelocity, selectedEdge = NULL;
    int maxPheromone;

    maxPheromone = 0;

    LinkedListResetCurrent(vertexVelocity->possibleDestination);

    while (LinkedListMoveCurrentNext(vertexVelocity->possibleDestination)) {
        edgeVelocity = LinkedListGetCurrent(vertexVelocity->possibleDestination);
        neighbour = getVertexVelocity(this, edgeVelocity->to, vectorAdd(velocity, edgeVelocity->acceleration));
        if (neighbour->pheromone > maxPheromone) {
            maxPheromone = neighbour->pheromone;
            selectedEdge = edgeVelocity;
        }

    }
    if (selectedEdge) {
        *to = selectedEdge->to;
        return selectedEdge->acceleration;
    }
    return createVector(0, 0);
}

LinkedList graphVertexVelocityGetNeighbors(Graph this, Point from, Vector velocity) {
    return getVertexVelocity(this, from, velocity)->possibleDestination;
}

int graphVertexVelocityGetPheromone(Graph this, Point from, Vector velocity) {
    return getVertexVelocity(this, from, velocity)->pheromone;
}

void graphVertexVelocitySetPheromone(Graph this, Point from, Vector velocity, int pheromone) {
    getVertexVelocity(this, from, velocity)->pheromone = pheromone;
}

void graphVertexVelocityForEach(Graph this, void (*foreach)(VertexVelocity)) {
    int i, j, k, l;

    for (i = 0; i < this->racetrack->height; i ++) {
        for (j = 0; j < this->racetrack->width; j ++) {

            if (this->vertices[i][j]) {

                for (k = 0; k < 11; k ++) {
                    for (l = 0; l < 11; l ++) {

                        if (this->vertices[i][j]->inPutVelocity[k][l]) {
                            foreach(this->vertices[i][j]->inPutVelocity[k][l]);
                        }
                    }
                }
            }
        }
    }
}

VertexVelocity getVertexVelocity(Graph graph, Point coord, Vector velocity) {
    VertexVelocity *vertex;
    Point destination;
    EdgeVelocity newEdge;
    int i, j;
    vertex = &getVertex(graph, coord)->inPutVelocity[velocity.y+5][velocity.x+5];

    if (!*vertex) {
        *vertex = malloc(sizeof(struct vertexvelocity));
        (*vertex)->pheromone = MIN_PHEROMONE;
        (*vertex)->possibleDestination = newLinkedList();

        for (i = -1; i <= 1; i ++) {
            for (j = -1; j <= 1; j ++) {
                destination = createPoint(velocity.x + coord.x + i, velocity.y + coord.y + j );
                if (!PointEquals(destination, coord)) {
                    if (raceIsValidPosition(graph->racetrack, destination)) {
                        newEdge = malloc(sizeof(struct edgevelocity));
                        newEdge->to = destination;
                        newEdge->acceleration = createVector(i, j);
                        newEdge->gasolineCost = raceGasolineCost(graph->racetrack, coord, velocity, newEdge->acceleration);
                        newEdge->boostCost = 0;
                        LinkedListAddLast((*vertex)->possibleDestination, newEdge);
                    }
                }
            }
        }

    }

    return *vertex;
}

Vertex getVertex(Graph graph,Point coord) {

    if(!graph->vertices[coord.y][coord.x]) {
        LinkedList neighbors;
        LinkedList tmp;

        neighbors = newLinkedList();
        tmp = raceGetPossibleDestination(graph->racetrack, coord);

        while (LinkedListMoveCurrentNext(tmp)) {
            Edge newEdge = malloc(sizeof(struct edge_t));
            newEdge->to = *((Point*) LinkedListGetCurrent(tmp));
            newEdge->weight = raceGetNodeCost(graph->racetrack, coord,newEdge->to);
            LinkedListAddFirst(neighbors, newEdge);
        }

        LinkedListDelete(tmp);

        graph->vertices[coord.y][coord.x] = malloc(sizeof(struct vertex_t));
        graph->vertices[coord.y][coord.x]->neighbors = neighbors;
        graph->vertices[coord.y][coord.x]->distanceFromSource = INT_MAX;
        graph->vertices[coord.y][coord.x]->isSetToExplore = 0;
        graph->vertices[coord.y][coord.x]->previousVertex = *newPoint(-1, -1);
        graph->vertices[coord.y][coord.x]->isDijkstraPath = 0;

    }

    return graph->vertices[coord.y][coord.x];
}

int graphVertexIsInQueue(Graph graph,Point coord) {
    return getVertex(graph, coord)->isSetToExplore;
}

void graphVertexSetInQueue(Graph graph,Point coord) {
    getVertex(graph, coord)->isSetToExplore = 1;
}