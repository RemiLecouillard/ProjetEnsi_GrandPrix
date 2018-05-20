#include <stdio.h>
#include <stdlib.h>
#include <raceManager.h>
#include <zconf.h>
#include <signal.h>
#include "utils.h"


int main() {
    debug = fopen("debugperso.log", "w+");
    RaceManager raceManager;
    (void)freopen("error.log", "w+", stderr);

    raceManager = newRaceManager();

    RaceManagerMainLoop(raceManager);

    return EXIT_SUCCESS;
}
