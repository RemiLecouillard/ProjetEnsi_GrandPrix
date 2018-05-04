/*
 * Created by remi on 30/04/18.
*/

#ifndef PROJETENSI_GRANDPRIX_RACETRACK_H
#define PROJETENSI_GRANDPRIX_RACETRACK_H

#include <LinkedList.h>
#include "utils.h"

typedef struct race *Racetrack;

struct race {
    int width;
    int height;
    char** array;
};

Racetrack newRacetrack();

Point raceGetArrival(Racetrack racetrack);

LinkedList raceGetPossibleDestination(Racetrack racetrack, Point point);

int raceGetNodeCost(Racetrack, Point, Point);

#endif //PROJETENSI_GRANDPRIX_RACETRACK_H
