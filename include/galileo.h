//
// Created by lecouillard on 18/04/18.
//

#ifndef PROJETENSI_GRANDPRIX_GALILEO_H
#define PROJETENSI_GRANDPRIX_GALILEO_H

#include <stdio.h>
#include "utils.h"
#include "racetrack.h"
#include "driver.h"

/**
 * Computes the best acceleration to do.
 * @param racetrack
 * @param myDriver
 * @param otherDriver1
 * @param otherDriver2
 * @return a Vector of acceleration.
 */
Vector getBestDirection(Racetrack racetrack,Driver *myDriver,Driver *otherDriver1,Driver *otherDriver2);

#endif //PROJETENSI_GRANDPRIX_GALILEO_H
