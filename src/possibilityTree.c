/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
*/

/**
 * @author Rémi Lecouillard
 * @author Baptiste Dromard
 * @date 20/05/18.
*/

#include "possibilityTree.h"
#include <PriorityQueue.h>
#include <dijkstra.h>
#include <limits.h>
#include <time.h>
#include <math.h>
#include <assert.h>

/**
 *
 * @param graph
 * @param from
 * @param vel
 * @param gasoline
 * @return
 */
Stack rec(Graph graph, Point from, Vector vel, int gasoline, int hops, float norm);

int nb;

float getNorm(Racetrack race, Stack path,int gasoline,Driver start) {
    Point last, *curr;
    Vector lastV, currV, acc;
    int cost, i, totalNorm;

    last = start.position;
    lastV = start.velocity;
    cost = 0;
    i = 0;
    totalNorm = 0;

    while (!StackIsEmpty(path)) {
        curr = StackPop(path);
        currV = createVector(curr->x - last.x, curr->y - last.y);
        acc = createVector(currV.x - lastV.x, currV.y - lastV.y);
        cost += raceGasolineCost(race, last, lastV, acc);

        totalNorm += vectorNorm(lastV);
        i++;

        lastV = currV;
        last = *curr;

        free(curr);
    }

    fprintf(debug, " %d / %d * 6/2 = %f\n", gasoline, cost, (float)gasoline/cost * ((6.f/2.f) * 1.f/((float)totalNorm/(float)i)));

    return (float)gasoline/cost * ((6.f/2.f) * 1.f/((float)totalNorm/(float)i));
}

Stack shortestPath(Graph graph, Driver start, Driver* walls) {
    Dijkstra dijkstra;
    Stack path = NULL;
    clock_t begin, end;
    float norm;

    begin = clock();
    fprintf(debug, "TREE !!!!!!!!!!!!!!!!!!!\n");
    fflush(debug);
    //dijkstra = newDijkstra(graph, start.position, raceGetArrival(graph->racetrack), walls);
    //path = dijkstraFindShortestPath(dijkstra);
    dijkstraDelete(dijkstra);

    norm = getNorm(graph->racetrack, path, start.gasoline, start);

    nb = 0;
    path = rec(graph, start.position, start.velocity, start.gasoline, 0, norm);
    end = clock();
    fprintf(debug, "Time taken %1.10f s in %d \n", (float) (end - begin )/CLOCKS_PER_SEC, nb);
    return path;
}

Stack rec(Graph graph, Point from, Vector vel, int gasoline, int hops, float normPref) {
    LinkedList dest = NULL;
    EdgeVelocity edge = NULL;
    PriorityQueue queue = NULL;
    Stack path = NULL;
    int cost, gasolineCost;
    float norm;
    nb ++;

    if (gasoline <= 0) {
        return NULL;
    }

    /*for (int i = 0; i < hops; i ++) {
        fprintf(debug, "   ");
    }
    fprintf(debug, "%d %d (%d %d) (%d)\n", from.x, from.y, vel.x, vel.y, hops);
    fflush(debug);
*/
    if (raceIsArrival(graph->racetrack, from)) {
        fprintf(debug, "PATH FOUND !!!!!\n");
        path = newStack();
        StackAdd(path, newPoint(from.x, from.y));
        return path;
    }

    queue = newPriorityQueue();
    dest = graphVertexVelocityGetNeighbors(graph, from, vel);

    LinkedListResetCurrent(dest);
    assert(dest != NULL);
    //fprintf(debug, "List lenght %d\n", LinkedListGetLength(dest));
    //fflush(debug);
    while (LinkedListMoveCurrentNext(dest)) {
        cost = INT_MAX;
        edge = LinkedListGetCurrent(dest);
        if (graphVertexIsDijkstraPath(graph, edge->to)) {
            cost -= 10;
            norm = vectorNorm(vectorAdd(edge->acceleration, vel));
            if (norm < normPref) {
                cost -= (int)norm*10.;
            }
            PriorityQueueAdd(queue, edge, cost);
        }

    }

    while (!PriorityQueueIsEmpty(queue)) {
        edge = PriorityQueuePop(queue);
        //gasolineCost =  raceGasolineCost(graph->racetrack, from, vel, edge->acceleration);
        path = rec(graph, edge->to, vectorAdd(vel, edge->acceleration), gasoline - edge->gasolineCost, hops + 1, normPref);
        if (path != NULL) {
            StackAdd(path, newPoint(from.x, from.y));
            return path;
        }
    }
    PriorityQueueDelete(queue);

    return NULL;
}