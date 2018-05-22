/*
 * Created by Baptiste on 20/05/18.
*/

#ifndef PROJETENSI_GRANDPRIX_DEPTHFINDWAY_H
#define PROJETENSI_GRANDPRIX_DEPTHFINDWAY_H

#include "utils.h"

typedef struct possibility Possibility;

float TotalGasoline, TotalDistance;

Point depthGetWay(Graph g, Driver* us, Driver others[]);
Possibility tryThisWay(Graph g, Point position, Vector velocity, int iteration, int gasoline);
#endif //PROJETENSI_GRANDPRIX_DPETHFINDWAY_H
