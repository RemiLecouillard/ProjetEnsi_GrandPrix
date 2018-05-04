/*
 * Created by remi on 30/04/18.
*/

#ifndef PROJETENSI_GRANDPRIX_DRIVER_H
#define PROJETENSI_GRANDPRIX_DRIVER_H

#include "utils.h"

typedef struct driver Driver;

struct driver{
    Point position;
    Vector velocity;
    int gasoline;
};

/**
 *
 * @param driver
 * @param point
 * @return
 */
Vector driverGetNeededAcceleration(Driver *driver, Point point);

Driver createDriver(int gasoline);

void driverAddAcceleration(Driver *driver, Vector acceleration);

#endif //PROJETENSI_GRANDPRIX_DRIVER_H
