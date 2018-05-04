/*
 * Created by remi on 30/04/18.
*/

#include "driver.h"


Vector driverGetNeededAcceleration(Driver *driver, Point point) {
    Vector acceleration;

    acceleration.y = point.y - driver->position.y - driver->velocity.y;
    acceleration.x = point.x - driver->position.x - driver->velocity.x;

    return acceleration;
}

Driver createDriver(int gasoline) {
    Driver driver;
    driver.velocity.x = 0;
    driver.velocity.y = 0;
    driver.gasoline = gasoline;
    return driver;
}

void driverAddAcceleration(Driver *driver, Vector acceleration) {
    driver->velocity.x += acceleration.x;
    driver->velocity.y += acceleration.y;
}