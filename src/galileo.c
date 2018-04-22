//
// Created by lecouillard on 18/04/18.
//

#include <limits.h>
#include "galileo.h"


void dijkstra(Graph* graph,Vertex source) {
    int i;
    Vertex currentVertex, neighbourVertex;
    PriorityQueue nodesToExplore;
    Edge* neighbors;
    Edge neighborEdge;
    int size;
    int alternativeDistance;

    vertexSetDistance(source, 0);
    priorityQueueAdd(nodesToExplore, source, 0);

    while(priorityQueueIsNotEmpty(nodesToExplore)) {
        currentVertex = priorityQueuePop(nodesToExplore);
        neighbors = vertexGetNeighbors(currentVertex, &size);
        for (i = 0; i < size; i ++) {
            neighborEdge = neighbors[i];
            neighbourVertex = edgeGetNode(neighborEdge);
            if(!priorityQueueIsIn(nodesToExplore, neighbourVertex)) { /* if not in the queue we add it, could also use an array of explored nodes, or an attribute in the struct */
                vertexSetDistance(&neighbourVertex, INT_MAX);
                priorityQueueAdd(nodesToExplore, neighbourVertex, INT_MAX);
            }
            alternativeDistance = vertexGetDistance(currentVertex) + edgeGetWeight(neighborEdge); /* computes the total distance */
            if (alternativeDistance < vertexGetDistance(neighbourVertex)) { /* if the path is shorter than the previous, changes it */
                vertexSetDistance(neighbourVertex, alternativeDistance);
                vertexSetPrevious(neighbourVertex, currentVertex);
                priorityQueueDecreasePrio(nodesToExplore, neighbourVertex, alternativeDistance);
            }
        }
    }
}


Vector getBestDirection(Racetrack racetrack,Driver myDriver,Driver otherDriver1,Driver otherDriver2) {
    Vector acceleration;
    acceleration.x = 1;
    acceleration.y = 1;
    return acceleration;
}