/*
 * Created by remi on 30/04/18.
*/
#include <utils.h>
#include <driver.h>
#include <racetrack.h>
#include <raceManagerInterface.h>
#include <signal.h>
#include <unistd.h>
#include <graph.h>
#include <Stack.h>
#include <dijkstra.h>
#include <time.h>
#include <raceManager.h>
#include <antColony.h>
#include <assert.h>

#define _race this->racetrack
#define _graph this->graph
#define _ourDriver this->ourDriver
#define _otherDrivers this->otherDrivers
#define _antColony this->antColony
#define _turn this->turn
#define _path this->path

struct racemanager {
    Racetrack racetrack;
    Graph graph;
    Driver ourDriver;
    Driver otherDrivers[2];
    Stack path;
#ifdef ANT
    AntColony antColony;
#endif
    int turn;
};

void displayDebug(Racetrack racetrack,int gasoline);

void displayTurn(RaceManager this, Point next, Vector acceleration);

Vector getBestDirection(RaceManager this);

RaceManager newRaceManager() {
    RaceManager this = malloc(sizeof(struct racemanager));
    int gasoline;

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

    displayDebug(_race, gasoline);

    return this;
}

void displayDebug(Racetrack racetrack,int gasoline) {
    fprintf(debug, " === >Carte< === \n");
    fprintf(debug, "taille %d x %d\n", racetrack->width, racetrack->height);
    fprintf(debug, "Carburant de départ %d \n\n", gasoline);

    for (int i = 0; i < racetrack->height; i++) {
        for(int j = 0; j < racetrack->width; j++) {
            fprintf(debug, "%c", racetrack->array[i][j]);
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

    do {
        isFinished = updatePositions(&_ourDriver, &_otherDrivers[0], &_otherDrivers[1]);
        newAcceleration = getBestDirection(this);
        sendOrder(newAcceleration);
    } while(!isFinished);
    fclose(debug);

}

#ifndef ANT
Vector getBestDirection(RaceManager this) {
    Dijkstra dijkstra;
    Point *tmp, next;
    Vector acceleration;

    if (!_path) {
        dijkstra = newDijkstra(_graph, _ourDriver.position, raceGetArrival(_race), NULL);
        _path = dijkstraFindShortestPath(dijkstra);
        dijkstraDelete(dijkstra);
    }

    tmp = StackPop(_path);
    next = *tmp;
    free(tmp);

    if (!raceNoCollision(_ourDriver.position, _otherDrivers, next)) {
        StackDelete(_path);
        dijkstra = newDijkstra(_graph, _ourDriver.position, raceGetArrival(_race), _otherDrivers);
        _path = dijkstraFindShortestPath(dijkstra);
        dijkstraDelete(dijkstra);

        tmp = StackPop(_path);
        next = *tmp;
        free(tmp);
    }

    acceleration = driverGetNeededAcceleration(&_ourDriver, next);

    displayTurn(this, next, acceleration);

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
