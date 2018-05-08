/*
 * Created by remi on 30/04/18.
*/

#ifndef PROJETENSI_GRANDPRIX_RACEMANAGER_H
#define PROJETENSI_GRANDPRIX_RACEMANAGER_H

#include "racetrack.h"
#include "driver.h"

typedef struct racemanager *RaceManager;

RaceManager newRaceManager();

void RaceManagerMainLoop(RaceManager this);

#endif //PROJETENSI_GRANDPRIX_RACEMANAGER_H
