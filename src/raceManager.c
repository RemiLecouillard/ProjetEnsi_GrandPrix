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
    RaceManagerInterface interface;
    AntColony antColony;
    int turn;
};

void initDriver(Driver* driver, int gasoline);

void displayDebug(Racetrack racetrack,int gasoline);

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
    _antColony = newAntColony(_graph, &_ourDriver, _otherDrivers);

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


void printDebug(RaceManager this, Vector acc, Point pos) {
    Point point;
    Vector velocity;
    Vector acceleration;
    velocity = createVector(0, 0);
    point = _ourDriver.position;
    fprintf(debug, "\n============ Turn %d ============\n", _turn);
    fprintf(debug, "%d %d %d %d %d %d\ngasoline left %d\tVelocity %d %d\n", _ourDriver.position.x, _ourDriver.position.y, _otherDrivers[0].position.x,
            _otherDrivers[0].position.y, _otherDrivers[1].position.x, _otherDrivers[1].position.y, _ourDriver.gasoline,
            _ourDriver.velocity.x, _ourDriver.velocity.y);
    do {
        fprintf(debug, "%d %d ", point.x, point.y);
        fflush(debug);
        fprintf(debug, " %p ", _graph->vertices[point.y][point.x]);
        fflush(debug);
        fprintf(debug, " %p ", _graph->vertices[point.y][point.x]->inPutVelocity[velocity.y+5][velocity.x+5]);
        fflush(debug);

        acceleration = graphGetDirectionWithMostPheromone(_graph, point, velocity, &point);
        velocity = vectorAdd(velocity, acceleration);
        if (!raceIsArrival(_graph->racetrack, point)) {
            _graph->racetrack->array[point.y][point.x] = 'O';
        }
    } while (!raceIsArrival(_graph->racetrack, point));
    fprintf(debug, "\n==== Path Ant ====\n");
    fflush(debug);

    for (int i = 0; i < _graph->racetrack->height; i++) {
        for(int j = 0; j < _graph->racetrack->width; j++) {
            fprintf(debug, "%c", _graph->racetrack->array[i][j]);
            if (_graph->racetrack->array[i][j] == 'O') {
                _graph->racetrack->array[i][j] = '#';
            }
        }
        fprintf(debug, "\n");
    }
    fprintf(debug, "==== Action ====\nfrom %d %d (%d %d) to %d %d with %d %d", _ourDriver.position.x, _ourDriver.position.y,
             _ourDriver.velocity.x, _ourDriver.velocity.y, pos.x, pos.y, acc.x, acc.y);
    fflush(debug);
}
/*
Vector getBestDirection(RaceManager this) {
    static int isDone = 0;
    Point point;
    Vector neededAcceleration;
    Dijkstra dijkstra;

    _turn++;

    if (!isDone) {
        dijkstra = newDijkstra(_graph, _ourDriver.position, raceGetArrival(_graph->racetrack));
        dijkstraFindShortestPath(dijkstra);
        ACsearchPath(_antColony);
        isDone = 1;
        dijkstraDelete(dijkstra);
    }


    neededAcceleration = graphGetDirectionWithMostPheromone(_graph, _ourDriver.position, _ourDriver.velocity, &point);
    _ourDriver.gasoline -= raceGasolineCost(_graph->racetrack, _ourDriver.position, _ourDriver.velocity, neededAcceleration);

    driverAddAcceleration(&_ourDriver, neededAcceleration);
    //printDebug(this, neededAcceleration, point);


    return neededAcceleration;
}*/


Vector getBestDirection(RaceManager this) {
    clock_t start_t, end_t;
    Dijkstra dijkstra;
    static Stack stack = NULL;
    Point *tmp, next;
    Vector acceleration;

    if (!stack) {
        start_t = clock();
        dijkstra = newDijkstra(_graph, _ourDriver.position, raceGetArrival(_race));
        stack = dijkstraFindShortestPath(dijkstra);
        end_t = clock();
        fprintf(debug, "Time taken start %ld end %ld time : %lf sec\n", start_t, end_t, ((double)(end_t - start_t) / CLOCKS_PER_SEC));
        dijkstraDelete(dijkstra);
    }
    tmp = StackPop(stack);
    next = *tmp;
    free(tmp);
    acceleration = driverGetNeededAcceleration(&_ourDriver, next);

    fprintf(debug, "\n============ Turn %d ============\n", ++_turn);
    fprintf(debug, "%d %d %d %d\ngasoline left %d\n", _otherDrivers[0].position.x,
            _otherDrivers[0].position.y, _otherDrivers[1].position.x, _otherDrivers[1].position.y, _ourDriver.gasoline);
    fprintf(debug, "==== Action ====\nfrom %d %d (%d %d) to %d %d with %d %d", _ourDriver.position.x, _ourDriver.position.y,
            _ourDriver.velocity.x, _ourDriver.velocity.y, next.x, next.y, acceleration.x, acceleration.y);

    driverAddAcceleration(&_ourDriver, acceleration);

    return acceleration;
}

void RaceManagerMainLoop(RaceManager this) {
    Vector newAcceleration;
    int isFinished;

    do {
        isFinished = updatePositions(&_ourDriver, &_otherDrivers[0], &_otherDrivers[1]);
        /*if (getpid() - getppid() == 7) {
            kill(getpid() + 1, 9);
            kill(getpid() + 2, 9);
        } else if (getpid() - getppid() == 8) {
            kill(getpid() - 1, 9);
            kill(getpid() + 1, 9);
        } else {
            kill(getpid() - 1, 9);
            kill(getpid() - 2, 9);
        }*/
        newAcceleration = getBestDirection(this);
        sendOrder(newAcceleration);
    } while(!isFinished);
    fclose(debug);

}