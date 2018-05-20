/*
 * Created by remi on 30/04/18.
*/

#ifndef PROJETENSI_GRANDPRIX_RACETRACK_H
#define PROJETENSI_GRANDPRIX_RACETRACK_H

#include <LinkedList.h>
#include "utils.h"
#include "driver.h"

typedef struct race *Racetrack;

struct race {
    int width;
    int height;
    char** array;
};

Racetrack newRacetrack();

LinkedList raceGetArrival(Racetrack racetrack);

LinkedList raceGetPossibleDestination(Racetrack racetrack, Point point);

int raceGetNodeCost(Racetrack, Point, Point);

int raceIsValidPosition(Racetrack, Point);

int raceIsArrival(Racetrack, Point);

int raceGasolineCost(Racetrack race, Point coord, Vector velocity, Vector acceleration);

int raceIsPathPosssible(Racetrack race, Point from, Point to);

int raceNoCollision(Point from, Driver walls[], Point to);

#endif //PROJETENSI_GRANDPRIX_RACETRACK_H
