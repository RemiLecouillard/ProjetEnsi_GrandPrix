/*
 * Created by remi on 30/04/18.
*/

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


