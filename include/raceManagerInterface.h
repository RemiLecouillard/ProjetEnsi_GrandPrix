//
// Created by lecouillard on 18/04/18.
//

#ifndef PROJETENSI_GRANDPRIX_RACEMANAGERINTERFACE_H
#define PROJETENSI_GRANDPRIX_RACEMANAGERINTERFACE_H

#include "utils.h"
#include "racetrack.h"

typedef struct racemanagerinterface *RaceManagerInterface;

/**
 *
 * @param race
 * @param gasoline
 */
void init(Racetrack race,int* gasoline);

/**
 *
 * @param myDriver
 * @param otherDriver1
 * @param otherDriver2
 * @return
 */
int updatePositions(Driver* myDriver,Driver* otherDriver1,Driver* otherDriver2);

/**
 *
 * @param acceleration
 */
void sendOrder(Vector acceleration);



#endif //PROJETENSI_GRANDPRIX_RACEMANAGERINTERFACE_H
