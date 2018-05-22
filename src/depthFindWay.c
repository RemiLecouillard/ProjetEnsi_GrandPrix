/*
 * Created by Baptiste on 20/05/18.
*/

#include "possibilityTree.h"
#include "depthFindWay.h"
#include <PriorityQueue.h>
#include <dijkstra.h>
#include <limits.h>
#include <time.h>
#include <math.h>
#include <assert.h>

#define DEPTH 7

#define _to ((EdgeVelocity) LinkedListGetCurrent(accessibleNeighbours))->to
#define _acceleration ((EdgeVelocity) LinkedListGetCurrent(accessibleNeighbours))->acceleration
#define _newVelocity vectorAdd(velocity, _acceleration)
#define _newGasoline (gasoline - raceGasolineCost(g->racetrack, position, velocity, _acceleration))

struct possibility {
    int distance;
    int gasoline;
};

Possibility createPossibility(int distance,int gasoline);

int isBetter(Possibility a, Possibility b);

float getSegmentValue(int distance,int gasoline);

float raceValue;

int startDistance;

int startGasoline;

Point depthGetWay(Graph g, Driver* us, Driver others[]) {
    Point nextPosition;
    Point bestWay = createPoint(-1, -1);
    Vector nextVelocity;
    LinkedList accessibleNeighbours;
    Possibility possibility, bestPossibility;
    int gasolineCost;
    raceValue = TotalDistance/TotalGasoline;
    bestPossibility = createPossibility(INT_MAX, INT_MAX);
    accessibleNeighbours = graphVertexVelocityGetNeighbors(g, us->position, us->velocity);

    startDistance = graphVertexGetDistance(g, us->position);
    startGasoline = us->gasoline;
    LinkedListResetCurrent(accessibleNeighbours);
    while (LinkedListMoveCurrentNext(accessibleNeighbours)) {
        nextPosition = ((EdgeVelocity) LinkedListGetCurrent(accessibleNeighbours))->to;

        if(raceNoCollision(us->position, others, nextPosition)) {

            nextVelocity = vectorAdd(us->velocity, _acceleration);
            if (graphVertexGetDistance(g, _to) >= graphVertexGetDistance(g, us->position))
                continue;
            gasolineCost = us->gasoline - raceGasolineCost(g->racetrack, us->position, us->velocity, _acceleration);
            possibility = tryThisWay(g, nextPosition, nextVelocity, DEPTH, gasolineCost);

            if(isBetter(possibility, bestPossibility)) {
                bestPossibility = possibility;
                bestWay = nextPosition;
            }
        }
    }

    return bestWay;
}

Possibility tryThisWay(Graph g, Point position, Vector velocity, int iteration, int gasoline) {
   LinkedList accessibleNeighbours;
   Possibility bestPossibility, possibility;

   accessibleNeighbours = graphVertexVelocityGetNeighbors(g, position, velocity);
   bestPossibility = createPossibility(INT_MAX, gasoline);

   if (gasoline <= 0) {
       return createPossibility(INT_MAX, INT_MAX);
   }

    if (raceIsArrival(g->racetrack, position)) {
        return createPossibility(0-iteration, gasoline);
    }

   if(!iteration) {
       if (getSegmentValue(graphVertexGetDistance(g, position), gasoline) < raceValue*0.9)
           return createPossibility(INT_MAX, INT_MAX);
       return createPossibility(graphVertexGetDistance(g, position), gasoline);
   }

   LinkedListResetCurrent(accessibleNeighbours);
   while (LinkedListMoveCurrentNext(accessibleNeighbours)) {

       if (graphVertexGetDistance(g, _to) >= graphVertexGetDistance(g, position))
           continue;

       possibility = tryThisWay(g, _to, _newVelocity , iteration - 1, _newGasoline);

       if(isBetter(possibility, bestPossibility)) {
          bestPossibility = possibility;
       }
   }

   return bestPossibility;
}


Possibility createPossibility(int distance,int gasoline) {
    Possibility new;
    new.gasoline = gasoline;
    new.distance = distance;
    return new;
}

int isBetter(Possibility a, Possibility b) {
    if (a.distance < b.distance) {
        return 1;
    } else if (a.distance > b.distance) {
        return 0;
    }

    if (a.gasoline > b.gasoline) {
        return 1;
    } else if (a.gasoline < b.gasoline) {
        return 0;
    }
    return 1;
}

float getSegmentValue(int distance,int gasoline) {
    return (float) (startDistance-distance)/(startGasoline-gasoline);
}