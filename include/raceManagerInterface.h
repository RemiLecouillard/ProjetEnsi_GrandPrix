//
// Created by lecouillard on 18/04/18.
//

#ifndef PROJETENSI_GRANDPRIX_RACEMANAGERINTERFACE_H
#define PROJETENSI_GRANDPRIX_RACEMANAGERINTERFACE_H

#include <stdio.h>
#include "util.h"

void init(Racetrack* race,int* gasoline);

int updatePositions(Driver* myDriver,Driver* otherDriver1,Driver* otherDriver2);

void sendOrder(Vector acceleration);



#endif //PROJETENSI_GRANDPRIX_RACEMANAGERINTERFACE_H
