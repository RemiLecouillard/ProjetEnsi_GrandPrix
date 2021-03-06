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
 * @date 30/04/18
*/

#include <utils.h>
#include <driver.h>
#include <racetrack.h>
#include <raceManagerInterface.h>
#include <graph.h>
#include <dijkstra.h>
#include <raceManager.h>
#include "depthFindWay.h"

#define _race this->racetrack
#define _graph this->graph
#define _ourDriver this->ourDriver
#define _otherDrivers this->otherDrivers
#define _antColony this->antColony
#define _turn this->turn

struct racemanager {
    Racetrack racetrack;
    Graph graph;
    Driver ourDriver;
    Driver otherDrivers[2];
#ifdef ANT
    AntColony antColony;
#endif
    int turn;
};

void displayDebug(RaceManager this,int gasoline);

void displayTurn(RaceManager this, Point next, Vector acceleration);

Vector getBestDirection(RaceManager this);

RaceManager newRaceManager() {
    RaceManager this = malloc(sizeof(struct racemanager));
    int gasoline;
    Dijkstra dijkstra;

    _race = newRacetrack();
    init(_race, &gasoline);
    _ourDriver = createDriver(gasoline);
    _otherDrivers[0] = createDriver(gasoline);
    _otherDrivers[1] = createDriver(gasoline);
    _graph = newGraph(_race);
    _turn = 0;

#ifdef ANT
    _antColony = newAntColony(_graph, &_ourDriver, _otherDrivers);
#endif

    dijkstra = newDijkstra(_graph,raceGetArrival(_race),  NULL, NULL);
    dijkstraFindShortestPath(dijkstra);
    dijkstraDelete(dijkstra);
    displayDebug(this, gasoline);
    TotalGasoline = _ourDriver.gasoline;

    return this;
}

void displayDebug(RaceManager this,int gasoline) {
    fprintf(debug, " === >Carte< === \n");
    fprintf(debug, "taille %d x %d\n", _race->width, _race->height);
    fprintf(debug, "Carburant de départ %d \n\n", gasoline);

    for (int i = 0; i < _race->height; i++) {
        for(int j = 0; j < _race->width; j++) {
            if (_race->array[i][j] == '#')
                fprintf(debug, "%4d", graphVertexGetDistance(_graph, createPoint(j, i)));
            else
                fprintf(debug, "%4c", _race->array[i][j]);
        }
        fprintf(debug, "\n");
    }

    fflush(debug);
}

void displayTurn(RaceManager this, Point next, Vector acceleration) {
    fprintf(debug, "\n============ Turn %d ============\n", ++_turn);
    fprintf(debug, "%d %d %d %d\ngasoline left %d\n", _otherDrivers[0].position.x,
            _otherDrivers[0].position.y, _otherDrivers[1].position.x, _otherDrivers[1].position.y, _ourDriver.gasoline);
    fprintf(debug, "==== Action ====\nfrom %d %d (%d %d) to %d %d with %d %d", _ourDriver.position.x, _ourDriver.position.y,
            _ourDriver.velocity.x, _ourDriver.velocity.y, next.x, next.y, acceleration.x, acceleration.y);

}

void RaceManagerMainLoop(RaceManager this) {
    Vector newAcceleration;
    int isFinished;
    TotalDistance = -1;
    do {
        isFinished = updatePositions(&_ourDriver, &_otherDrivers[0], &_otherDrivers[1]);
        if (TotalDistance == -1) {
            TotalDistance = graphVertexGetDistance(_graph, _ourDriver.position);
        }
        newAcceleration = getBestDirection(this);
        sendOrder(newAcceleration);
    } while(!isFinished);
    fclose(debug);

}

#ifndef ANT
Vector getBestDirection(RaceManager this) {
    Point next;
    Vector acceleration;

    next = depthGetWay(_graph, &_ourDriver, _otherDrivers);

    if (PointEquals(createPoint(-1, -1), next))
        acceleration = createVector(0, 0);
    else
        acceleration = driverGetNeededAcceleration(&_ourDriver, next);

    _ourDriver.gasoline -= raceGasolineCost(_race, _ourDriver.position, _ourDriver.velocity, acceleration);

    //displayTurn(this, next, acceleration);

    return acceleration;
}
#endif

#ifdef ANT
Vector getBestDirection(RaceManager this) {
    static int isDone = 0;
    Point point = createPoint(-10, -10);
    Vector neededAcceleration;
    Dijkstra dijkstra;

    _turn++;

    if (!isDone) {
        dijkstra = newDijkstra(_graph, _ourDriver.position, raceGetArrival(_graph->racetrack));
        dijkstraFindShortestPath(dijkstra);
        isDone = 1;
        dijkstraDelete(dijkstra);
        ACsearchPath(_antColony);
    }



    neededAcceleration = graphGetDirectionWithMostPheromone(_graph, _ourDriver.position, _ourDriver.velocity, &point);
    _ourDriver.gasoline -= raceGasolineCost(_graph->racetrack, _ourDriver.position, _ourDriver.velocity, neededAcceleration);

    assert(point.x != -10);

    assert(raceIsValidPosition(_graph->racetrack, point));
    driverAddAcceleration(&_ourDriver, neededAcceleration);


    //printDebug(this, neededAcceleration, point);


    return neededAcceleration;
}
#endif
