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

#define DEPTH 6

Point depthGetWay(Graph g, Driver* us, Driver others[]) {
   
   Point nextPosition; 
   Point bestWay = createPoint(-1, -1);
   int distance;
   int lowerDistance;
   Vector nextVelocity;
   LinkedList accessibleNeighbours;
   
   lowerDistance = INT_MAX;
   accessibleNeighbours = graphVertexVelocityGetNeighbors(g, us->position, us->velocity);
   LinkedListResetCurrent(accessibleNeighbours);

   while (LinkedListMoveCurrentNext(accessibleNeighbours)) {
      
      nextPosition = ((EdgeVelocity) LinkedListGetCurrent(accessibleNeighbours))->to;

      if(raceNoCollision(us->position, others, nextPosition)) {
          nextVelocity = vectorAdd(us->velocity, ((EdgeVelocity) LinkedListGetCurrent(accessibleNeighbours))->acceleration);
          distance = tryThisWay(g, nextPosition, nextVelocity, DEPTH);

          if(distance < lowerDistance) {
              lowerDistance = distance;
              bestWay = nextPosition;
          }
      }
   }

   return bestWay;
}

int tryThisWay(Graph g, Point position, Vector velocity, int iteration) {
   int distance;
   int lowerDistance;
   LinkedList accessibleNeighbours;
   lowerDistance = INT_MAX;
   accessibleNeighbours = graphVertexVelocityGetNeighbors(g, position, velocity);

   if(!iteration) {
       return graphVertexGetDistance(g, position);
   }

   if (raceIsArrival(g->racetrack, position)) {
       return 0-iteration;
   }

   LinkedListResetCurrent(accessibleNeighbours);
   while (LinkedListMoveCurrentNext(accessibleNeighbours)) {
       distance = tryThisWay(g, ((EdgeVelocity) LinkedListGetCurrent(accessibleNeighbours))->to, vectorAdd(velocity,((EdgeVelocity) LinkedListGetCurrent(accessibleNeighbours))->acceleration) , iteration - 1);

       if(distance < lowerDistance) {
          lowerDistance = distance;
       }
   }

   return lowerDistance;
}
