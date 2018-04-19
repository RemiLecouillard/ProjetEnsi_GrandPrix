#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "util.h"
#include "raceManagerInterface.h"
#include "galileo.h"

FILE *debug;

void initDriver(Driver* driver,int gasoline) {
    driver->gasoline = gasoline;
}


void displayDebug(Racetrack racetrack,int gasoline) {
    fprintf(debug, " === >Carte< === \n");
    fprintf(debug, "taille %d x %d\n", racetrack.width, racetrack.height);
    fprintf(debug, "Carburant de départ %d \n\n", gasoline);

    for (int i = 0; i < racetrack.height; i++) {
        for(int j = 0; j < racetrack.width; j++) {
            fprintf(debug, "%c", racetrack.array[i][j]);
        }
        fprintf(debug, "\n");
    }

    fflush(debug);
}

void mainLoop() {
    Vector newAcceleration;
    Driver myDriver, otherDriver1, otherDriver2;
    Racetrack racetrack;
    int gasoline, isFinished;

    init(&racetrack, &gasoline);
    initDriver(&myDriver, gasoline);
    initDriver(&otherDriver1, gasoline);
    initDriver(&otherDriver2, gasoline);

    displayDebug(racetrack, gasoline);
    fclose(debug);
    do {
        isFinished = updatePositions(&myDriver, &otherDriver1, &otherDriver2);
        newAcceleration = getBestDirection(racetrack, myDriver, otherDriver1, otherDriver2);
        sendOrder(newAcceleration);
    }while(!isFinished);

}

int main() {
    debug = fopen("debugperso.log", "w+");

    mainLoop();

    return EXIT_SUCCESS;
}
