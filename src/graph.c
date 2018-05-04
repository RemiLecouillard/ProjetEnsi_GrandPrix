//
// Created by remi on 22/04/18.
//

#include <utils.h>
#include <stdlib.h>
#include <limits.h>
#include <racetrack.h>
#include "graph.h"
#include "utils.h"

Vertex getVertex(Graph graph,Point coord);

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

    }

    return graph->vertices[coord.y][coord.x];
}

int graphVertexIsInQueue(Graph graph,Point coord) {
    return getVertex(graph, coord)->isSetToExplore;
}

void graphVertexSetInQueue(Graph graph,Point coord) {
    getVertex(graph, coord)->isSetToExplore = 1;
}