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

struct racemanager {
    Racetrack racetrack;
    Graph graph;
    Driver ourDriver;
    Driver otherDrivers[2];
    RaceManagerInterface interface;
    AntColony antColony;
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
    //_antColony = newAntColony(_graph);

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

/*
Vector getBestDirection(RaceManager this) {
    Point nextPosition;
    Vector neededAcceleration;

    ACsearchPath(_antColony, _ourDriver, _otherDrivers);

    nextPosition = graphGetVertexWithMostPheromon(_graph, _ourDriver.position);

    neededAcceleration = driverGetNeededAcceleration(&_ourDriver, nextPosition);

    driverAddAcceleration(&_ourDriver, neededAcceleration);

    return neededAcceleration;
}*/

Vector getBestDirection(RaceManager this) {
    static Stack stack = NULL;
    static Graph graph = NULL;
    Point source, next, *tmp;
    Vector acceleration;
    LinkedList arrivals;
    clock_t start_t, end_t;
    Dijkstra dijkstra;

    if (!stack) {
        start_t = clock();
        fflush(debug);
        graph = newGraph(_race);
        source = _ourDriver.position;
        arrivals = raceGetArrival(_race);
        dijkstra = newDijkstra(graph, source, arrivals);
        stack = dijkstraFindShortestPath(dijkstra);
        end_t = clock();
        fprintf(debug, "Time taken start %ld end %ld time : %lf sec\n%p path\n", start_t, end_t, ((double)(end_t - start_t) / CLOCKS_PER_SEC), stack);
        fflush(debug);
        displayDebug(_race, 0);
        dijkstraDelete(dijkstra);
    }
    tmp = StackPop(stack);
    next = *tmp;
    free(tmp);
    acceleration = driverGetNeededAcceleration(&_ourDriver, next);
    fprintf(debug, "***********Action*************\ndriver coord %d %d\ndriver velocity %d %d\n", _ourDriver.position.x, _ourDriver.position.y,_ourDriver.velocity.x, _ourDriver.velocity.y);
    fprintf(debug, "Point %d %d\nacceleration -> %d %d\n\n", next.x, next.y, acceleration.x, acceleration.y);
    fflush(debug);
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