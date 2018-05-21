/*
 * Created by Baptiste on 20/05/18.
*/

#ifndef PROJETENSI_GRANDPRIX_DEPTHFINDWAY_H
#define PROJETENSI_GRANDPRIX_DEPTHFINDWAY_H

#include "utils.h"

Point depthGetWay(Graph g, Driver* us, Driver others[]);
int tryThisWay(Graph g, Point position, Vector velocity, int iteration);

#endif //PROJETENSI_GRANDPRIX_DPETHFINDWAY_H
