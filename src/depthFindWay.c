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

Point depthGetWay(Graph g, Driver* us, Driver others[]) {
   
   Point nextPosition; 
   Point bestWay;
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
   
   while (LinkedListMoveCurrentNext(accessibleNeighbours)) {
      distance = tryThisWay(g, ((EdgeVelocity) LinkedListGetCurrent(accessibleNeighbours))->to, vectorAdd(velocity,((EdgeVelocity) LinkedListGetCurrent(accessibleNeighbours))->acceleration) , iteration-1);

      if(distance < lowerDistance) {
	 lowerDistance = distance; 
      }      
   }
   return lowerDistance;
}
