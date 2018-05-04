//
// Created by lecouillard on 18/04/18.
//

#include <limits.h>

#include <util.h>
#include <unistd.h>
#include <PriorityQueue.h>
#include <LinkedList.h>
#include <time.h>
#include <galileo.h>
#include <graph.h>
#include <utils.h>
#include <Stack.h>
#include <racetrack.h>
#include <driver.h>
#include "utils.h"

void updateDistance(Graph graph,PriorityQueue nodesToExplore,Point currentVertex, Edge neighborEdge,
                    Point *neighbourVertex);

Stack createPath(Graph pGraph, Point point, Point source);

int equalsPoint(T param1, T param2);

void display(T value) {
    Point *a;
    a = (Point*)value;
    fprintf(debug, "%d %d", a->x, a->y);
}

Stack dijkstra(Graph graph,Point source,Point destination) {
    Point currentVertex, neighbourVertex;
    PriorityQueue nodesToExplore;
    LinkedList neighbors;
    Edge neighborEdge;

    nodesToExplore = newPriorityQueue();
    graphVertexSetDistance(graph, source, 0);
    PriorityQueueAdd(nodesToExplore, &source, 0);
    graphVertexSetInQueue(graph, source);

    while (!PriorityQueueIsEmpty(nodesToExplore)) {
        /* The Vertex with the min distance */
        currentVertex = *((Point*) PriorityQueuePop(nodesToExplore));
        neighbors = graphVertexGetNeighbors(graph, currentVertex);
        //fprintf(debug, "\n************** %d %d (%d) **************\n", currentVertex.x, currentVertex.y, graphVertexGetDistance(graph, currentVertex));
        if (currentVertex.x == destination.x && currentVertex.y == destination.y) {
            //fprintf(debug, "arrival, create path\n");
            return createPath(graph, currentVertex, source);
        }

        while (LinkedListMoveCurrentNext(neighbors)) {

            neighborEdge = LinkedListGetCurrent(neighbors);
            neighbourVertex = neighborEdge->to;
            //fprintf(debug, "neighbors : %d %d (%d)\n", neighbourVertex.x, neighbourVertex.y, graphVertexGetDistance(graph, neighbourVertex));
            /* Adds Vertex in the queue to be explore */
            if (!graphVertexIsInQueue(graph, neighbourVertex)) {
                //fprintf(debug, "not in queue yet, add it\n");
                graphVertexSetDistance(graph, neighbourVertex, INT_MAX);
                PriorityQueueAdd(nodesToExplore, newPoint(neighbourVertex.x, neighbourVertex.y), INT_MAX);
                graphVertexSetInQueue(graph, neighbourVertex);
            }

            updateDistance(graph, nodesToExplore, currentVertex, neighborEdge, &neighbourVertex);
            //fprintf(debug, "\n******* queue *******\n");

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
    //fprintf(debug, "alternative distance : %d\n", alternativeDistance);
    if (alternativeDistance < graphVertexGetDistance(graph, *neighbourVertex)) { /* if the path is shorter than the previous, changes it */
        //fprintf(debug, "Shorter, changed !\n");
        graphVertexSetDistance(graph, *neighbourVertex, alternativeDistance);
        graphVertexSetPrevious(graph, *neighbourVertex, currentVertex);
        PriorityQueueChangePrioSpecificSearch(nodesToExplore, neighbourVertex, alternativeDistance, equalsPoint);
        /*for (int i = 0; i < graph->racetrack->height; i++) {
            for(int j = 0; j < graph->racetrack->width; j++) {
                if (graph->vertices[i][j])
                    fprintf(debug, "%4d", graphVertexGetDistance(graph, *newPoint(j, i)));
                else
                    fprintf(debug, "%4c", graph->racetrack->array[i][j],j, i);
            }
            fprintf(debug, "\n");
        }
        fprintf(debug, "\n");
        fflush(debug);*/
    }
}


Vector getBestDirection(Racetrack racetrack,Driver *myDriver,Driver *otherDriver1,Driver *otherDriver2) {
    static Stack stack = NULL;
    static Graph graph = NULL;
    Point source, destination, next, *tmp;
    Vector acceleration;
    clock_t start_t, end_t;

    if (!stack) {
        start_t = clock();
        fprintf(debug, "No stack\n");
        fflush(debug);
        graph = newGraph(racetrack);
        source = myDriver->position;
        destination = raceGetArrival(racetrack);
        fprintf(debug, "arrival -> %d %d\n", destination.x, destination.y);
        fflush(debug);
        stack = dijkstra(graph, source, destination);
        fprintf(debug, "dijkstra finished path -> %p\n", stack);
        fflush(debug);
        end_t = clock();
        fprintf(debug, "Time taken start %ld end %ld time : %lf sec\n", start_t, end_t, ((double)(end_t - start_t) / CLOCKS_PER_SEC));
        fflush(debug);

        /*while (!StackIsEmpty(stack)) {
            Point *tmp = StackPop(stack);
            racetrack->array[tmp->y][tmp->x] = 'O';
        }

        for (int i = 0; i < racetrack->height; i++) {
            for(int j = 0; j < racetrack->width; j++) {
                fprintf(debug, "%c", racetrack->array[i][j]);
            }
            fprintf(debug, "\n");
        }

        fflush(debug);*/
    }
    tmp = StackPop(stack);
    next = *tmp;
    free(tmp);
    acceleration = driverGetNeededAcceleration(myDriver, next);
    driverAddAcceleration(myDriver, acceleration);
    fprintf(debug, "driver vel %d %d\n", myDriver->velocity.x, myDriver->velocity.y);
    fprintf(debug, "***********Action*************\nPoint %d %d\nacceleration -> %d %d\n\n", next.x, next.y, acceleration.x, acceleration.y);
    fflush(debug);
    return acceleration;
}

int equalsPoint(T param1, T param2) {
    Point *a,*b;
    a = (Point*)param1;
    b = (Point*)param2;
    return (a->y == b->y && a->x == b->x);
}