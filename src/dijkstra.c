/*
 * Created by remi on 04/05/18.
*/

#include <limits.h>

#include <Stack.h>
#include <PriorityQueue.h>

#include <utils.h>
#include <graph.h>

#include "dijkstra.h"

#define _graph this->graph
#define _source this->source
#define _destinations this->destinations
#define _nodesToExplore this->nodesToExplore
#define _neighbors this->neighbors
#define _currentVertex this->currentVertex
#define _neighborVertex this->neighbourVertex
#define _neighborEdge this->neighborEdge

struct dijkstra {
    Graph graph;
    Point source;
    LinkedList destinations; /* points */
    PriorityQueue nodesToExplore;
    LinkedList neighbors; /* points */
    Point currentVertex;
    Point neighbourVertex;
    Edge neighborEdge;
};

void updateDistance(Graph graph,PriorityQueue nodesToExplore,Point currentVertex, Edge neighborEdge,
                    Point *neighbourVertex);

Stack createPath(Graph pGraph, Point point, Point source);

int equalsPoint(T param1, T param2);

struct dijkstra *newDijkstra(Graph graph,Point source,LinkedList destination) {
    struct dijkstra *this;
    this = malloc(sizeof(struct dijkstra));
    _graph = graph;
    _source = source;
    _destinations = destination;
    _nodesToExplore = newPriorityQueue();
    _neighbors = NULL;
    _neighborEdge = NULL;

    graphVertexSetDistance(graph, _source, 0);
    PriorityQueueAdd(_nodesToExplore, &_source, 0);
    graphVertexSetInQueue(graph, _source);

    return this;
}

Stack dijkstraFindShortestPath(struct dijkstra *this) {
    while (!PriorityQueueIsEmpty(_nodesToExplore)) {

        _currentVertex = *((Point*) PriorityQueuePop(_nodesToExplore));
        _neighbors = graphVertexGetNeighbors(_graph, _currentVertex);

        if (pointIsIn(_currentVertex, _destinations)) {
            return createPath(_graph, _currentVertex, _source);
        }

        while (LinkedListMoveCurrentNext(_neighbors)) {

            _neighborEdge = LinkedListGetCurrent(_neighbors);
            _neighborVertex = _neighborEdge->to;

            /* Adds Vertex in the queue to be explore */
            if (!graphVertexIsInQueue(_graph, _neighborVertex)) {
                graphVertexSetDistance(_graph, _neighborVertex, INT_MAX);
                PriorityQueueAdd(_nodesToExplore, newPoint(_neighborVertex.x, _neighborVertex.y), INT_MAX);
                graphVertexSetInQueue(_graph, _neighborVertex);
            }

            updateDistance(_graph, _nodesToExplore, _currentVertex, _neighborEdge, &_neighborVertex);

        }
        LinkedListResetCurrent(_neighbors);

    }

    return NULL;
}

void dijkstraDelete(struct dijkstra *this) {
    LinkedListDelete(_destinations);
    PriorityQueueDelete(_nodesToExplore);
    free(this);
}

Stack createPath(Graph graph, Point point, Point source) {
    Point *current;
    Point tmp;
    Stack stack;

    current = malloc(sizeof(Point));
    *current = point;
    stack = newStack();

    while(current->x != source.x || current->y != source.y) {
        StackAdd(stack, current);
        graphVertexSetDijkstraPath(graph, *current, 1),
        tmp = graphVertexGetPrevious(graph, *current);
        current = malloc(sizeof(Point));
        *current = tmp;
    }

    return stack;
}

void updateDistance(Graph graph,PriorityQueue nodesToExplore,Point currentVertex,Edge neighborEdge,
                    Point *neighbourVertex) {
    int alternativeDistance;

    alternativeDistance = graphVertexGetDistance(graph, currentVertex) + neighborEdge->weight; /* computes the total distance */
    if (alternativeDistance < graphVertexGetDistance(graph, *neighbourVertex)) { /* if the path is shorter than the previous, changes it */

        graphVertexSetDistance(graph, *neighbourVertex, alternativeDistance);
        graphVertexSetPrevious(graph, *neighbourVertex, currentVertex);
        PriorityQueueChangePrioSpecificSearch(nodesToExplore, neighbourVertex, alternativeDistance, equalsPoint);
    }
}

int equalsPoint(T param1, T param2) {
    Point *a,*b;
    a = (Point*)param1;
    b = (Point*)param2;
    return (a->y == b->y && a->x == b->x);
}