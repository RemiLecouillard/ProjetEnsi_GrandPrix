/*
 * Created by remi on 04/05/18.
*/

#include <limits.h>

#include <Stack.h>
#include <PriorityQueue.h>

#include <utils.h>
#include <graph.h>

#include "dijkstra.h"

void updateDistance(Graph graph,PriorityQueue nodesToExplore,Point currentVertex, Edge neighborEdge,
                    Point *neighbourVertex);

Stack createPath(Graph pGraph, Point point, Point source);

int equalsPoint(T param1, T param2);

Stack dijkstra(Graph graph,Point source,LinkedList destination) {
    Point currentVertex, neighbourVertex;
    PriorityQueue nodesToExplore;
    LinkedList neighbors;
    Edge neighborEdge;

    nodesToExplore = newPriorityQueue();
    graphVertexSetDistance(graph, source, 0);
    PriorityQueueAdd(nodesToExplore, &source, 0);
    graphVertexSetInQueue(graph, source);

    while (!PriorityQueueIsEmpty(nodesToExplore)) {

        currentVertex = *((Point*) PriorityQueuePop(nodesToExplore));
        neighbors = graphVertexGetNeighbors(graph, currentVertex);

        if (pointIsIn(currentVertex, destination)) {
            return createPath(graph, currentVertex, source);
        }

        while (LinkedListMoveCurrentNext(neighbors)) {

            neighborEdge = LinkedListGetCurrent(neighbors);
            neighbourVertex = neighborEdge->to;

            /* Adds Vertex in the queue to be explore */
            if (!graphVertexIsInQueue(graph, neighbourVertex)) {
                graphVertexSetDistance(graph, neighbourVertex, INT_MAX);
                PriorityQueueAdd(nodesToExplore, newPoint(neighbourVertex.x, neighbourVertex.y), INT_MAX);
                graphVertexSetInQueue(graph, neighbourVertex);
            }

            updateDistance(graph, nodesToExplore, currentVertex, neighborEdge, &neighbourVertex);

        }
        LinkedListResetCurrent(neighbors);

    }

    return NULL;
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
        graph->racetrack->array[current->y][current->x] = 'O';
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