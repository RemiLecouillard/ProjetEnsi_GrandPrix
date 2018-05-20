/*
 * Created by remi on 28/04/18.
*/

#include <malloc.h>
#include <math.h>
#include "utils.h"


Point *newPoint(int x, int y) {
    Point *res;
    res = malloc(sizeof(Point));
    res->y = y;
    res->x = x;
    return res;
}

Point createPoint(int x, int y) {
    Point point;
    point.x = x;
    point.y = y;
    return point;
}

Vector *newVector(int x, int y) {
    Vector *res;
    res = malloc(sizeof(Vector));
    res->y = y;
    res->x = x;
    return res;
}


Vector createVector(int x, int y) {
    Vector vec;
    vec.x = x;
    vec.y = y;
    return vec;
}

int pointIsIn(Point point, LinkedList list) {
    Point *tmp;
    LinkedListResetCurrent(list);
    while(LinkedListMoveCurrentNext(list)) {
        tmp = LinkedListGetCurrent(list);
        if (tmp->y == point.y && tmp->x == point.x) {
            return 1;
        }
    }
    return 0;
}

int PointEquals(Point a, Point b) {
    return a.x == b.x && a.y == b.y;
}

Vector vectorAdd(Vector a, Vector b) {
    Vector c;

    c.y = a.y + b.y;
    c.x = a.x + b.x;

    return c;
}

float vectorNorm(Vector a) {
    return sqrtf(a.x*a.x + a.y*a.y);
}