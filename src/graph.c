//
// Created by remi on 22/04/18.
//

#include <utils.h>
#include <stdlib.h>
#include <limits.h>
#include <racetrack.h>
#include <antColony.h>
#include <assert.h>
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
        for (int j = 0; j < racetrack->width; j ++) {
            graph->vertices[i][j] = NULL;
        }
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

void graphInitDijkstra(Graph this) {
    int i, j;

    for (i = 0; i < this->racetrack->height; i ++) {
        for (j = 0; j < this->racetrack->width; j ++) {

            if (this->vertices[i][j]) {
                this->vertices[i][j]->distanceFromSource = INT_MAX;
                this->vertices[i][j]->isSetToExplore = 0;
                this->vertices[i][j]->previousVertex = createPoint(-1, -1);
                this->vertices[i][j]->isDijkstraPath = 0;
            }
        }
    }
}

LinkedList graphVertexVelocityGetNeighbors(Graph this, Point from, Vector velocity) {
    return getVertexVelocity(this, from, velocity)->possibleDestination;
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
#ifdef ANT
        (*vertex)->pheromone = MIN_PHEROMONE;
#endif
        (*vertex)->possibleDestination = newLinkedList();

        for (i = -1; i <= 1; i ++) {
            for (j = -1; j <= 1; j ++) {
                destination = createPoint(velocity.x + coord.x + i, velocity.y + coord.y + j );
                if (!PointEquals(destination, coord)) {
                    if (raceIsPathPosssible(graph->racetrack, coord, destination)) {
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
        graph->vertices[coord.y][coord.x]->previousVertex = createPoint(-1, -1);
        graph->vertices[coord.y][coord.x]->isDijkstraPath = 0;

        for (int i = 0; i < 11; i ++) {
            for (int j = 0; j < 11; j ++) {
                graph->vertices[coord.y][coord.x]->inPutVelocity[i][j] = NULL;
            }
        }

    }

    return graph->vertices[coord.y][coord.x];
}

int graphVertexIsInQueue(Graph graph,Point coord) {
    return getVertex(graph, coord)->isSetToExplore;
}

void graphVertexSetInQueue(Graph graph,Point coord) {
    getVertex(graph, coord)->isSetToExplore = 1;
}

#ifdef ANT
Vector graphGetDirectionWithMostPheromone(Graph this, Point from, Vector velocity, Point *to) {
    VertexVelocity fromVertex = getVertexVelocity(this, from, velocity);
    VertexVelocity neighbour;
    EdgeVelocity edgeVelocity, selectedEdge = NULL;
    int maxPheromone;

    maxPheromone = 0;

    LinkedListResetCurrent(fromVertex->possibleDestination);
    while (LinkedListMoveCurrentNext(fromVertex->possibleDestination)) {
        edgeVelocity = LinkedListGetCurrent(fromVertex->possibleDestination);
        Vector a =vectorAdd(velocity, edgeVelocity->acceleration);
        neighbour = getVertexVelocity(this, edgeVelocity->to, vectorAdd(velocity, edgeVelocity->acceleration));
        fprintf(debug, " -> to %d %d with %d %d, pheromone %d \n", edgeVelocity->to.x, edgeVelocity->to.y, edgeVelocity->acceleration.x,
        edgeVelocity->acceleration.y,neighbour->pheromone);
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

int graphVertexVelocityGetPheromone(Graph this, Point from, Vector velocity) {
    return getVertexVelocity(this, from, velocity)->pheromone;
}

void graphVertexVelocitySetPheromone(Graph this, Point from, Vector velocity, int pheromone) {
    getVertexVelocity(this, from, velocity)->pheromone = pheromone;
}
#endif