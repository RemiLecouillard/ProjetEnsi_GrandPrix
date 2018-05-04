/*
 * Created by remi on 30/04/18.
*/

#ifndef PROJETENSI_GRANDPRIX_RACEMANAGER_H
#define PROJETENSI_GRANDPRIX_RACEMANAGER_H

#include "racetrack.h"
#include "driver.h"

void initDriver(Driver* driver, int gasoline);

void displayDebug(Racetrack racetrack,int gasoline);

void mainLoop();

#endif //PROJETENSI_GRANDPRIX_RACEMANAGER_H
