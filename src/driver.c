/*
 * Created by remi on 30/04/18.
*/

#include "driver.h"


Vector driverGetNeededAcceleration(Driver *driver, Point point) {
    Vector acceleration;

    acceleration = createVector(point.x - driver->position.x - driver->velocity.x,
                                point.y - driver->position.y - driver->velocity.y);

    return acceleration;
}

Driver createDriver(int gasoline) {
    Driver driver;
    driver.velocity= createVector(0, 0);
    driver.gasoline = gasoline;
    return driver;
}

void driverAddAcceleration(Driver *driver, Vector acceleration) {
    driver->velocity.x = driver->velocity.x + acceleration.x;
    driver->velocity.y = driver->velocity.y + acceleration.y;
}