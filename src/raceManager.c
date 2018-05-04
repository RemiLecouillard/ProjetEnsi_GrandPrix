/*
 * Created by remi on 30/04/18.
*/
#include <utils.h>
#include <driver.h>
#include <racetrack.h>
#include <raceManagerInterface.h>
#include <galileo.h>
#include <signal.h>
#include <unistd.h>

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

void mainLoop() {
    Vector newAcceleration;
    Driver myDriver, otherDriver1, otherDriver2;
    Racetrack racetrack;
    int gasoline, isFinished;

    racetrack = newRacetrack();

    init(racetrack, &gasoline);
    myDriver = createDriver(gasoline);
    otherDriver1 = createDriver(gasoline);
    otherDriver2 = createDriver(gasoline);

    displayDebug(racetrack, gasoline);
    do {
        isFinished = updatePositions(&myDriver, &otherDriver1, &otherDriver2);
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
        newAcceleration = getBestDirection(racetrack, &myDriver, &otherDriver1, &otherDriver2);
        sendOrder(newAcceleration);
    } while(!isFinished);
    fclose(debug);

}