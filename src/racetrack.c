/*
 * Created by remi on 30/04/18.
*/

#include <math.h>

#include "racetrack.h"

Racetrack newRacetrack() {
    return malloc(sizeof(struct race));
}

LinkedList raceGetArrival(Racetrack racetrack) {
    int i, j;
    LinkedList list;

    list = newLinkedList();

    for (i = 0; i < racetrack->height; i ++) {
        for (j = 0; j < racetrack->width; j ++) {
            if (racetrack->array[i][j] == '=') {
                LinkedListAddFirst(list, newPoint(j, i));
            }
        }
    }
    return list;
}

LinkedList raceGetPossibleDestination(Racetrack racetrack, Point point) {
    int i, j;
    int x, y;
    LinkedList list;

    list = newLinkedList();

    for (i = -1; i < 2; i ++) {
        for (j = -1; j < 2; j ++) {
            x = j + point.x;
            y = i + point.y;
            if (x >= 0 && x < racetrack->width && y >= 0 && y < racetrack->height
                    && (x != point.x || y != point.y)) {
                if (racetrack->array[y][x] != '.') {
                    LinkedListAddFirst(list, newPoint(x, y));
                }
            }
        }
    }

    return list;
}

int raceGetNodeCost(Racetrack race, Point from, Point to) {
    int cost;
    cost = 10;
    if (from.x != to.x && from.y != to.y) {
        cost += 3;
    }

    if (race->array[to.y][to.x] == '~') {
        cost += 40;
    }

    return cost;
}

int raceIsValidPosition(Racetrack race, Point coord) {
    if (coord.x < 0 || coord.y < 0 || coord.x >= race->width || coord.y >= race->height) {
        return 0;
    }

    return race->array[coord.y][coord.x] != '.';
}

int raceIsArrival(Racetrack race, Point coord) {
    if (!raceIsValidPosition(race, coord)) return 1;
    return race->array[coord.y][coord.x] == '=';
}

int raceGasolineCost(Racetrack race, Point coord, Vector velocity, Vector acceleration) {
    int value = acceleration.x * acceleration.x + acceleration.y * acceleration.y;
    value += (int)(sqrt(velocity.x * velocity.x + velocity.y * velocity.y) * 3.0 / 2.0);
    if (race->array[coord.y][coord.x] == '~')
        value += 1;
    return value;
}
