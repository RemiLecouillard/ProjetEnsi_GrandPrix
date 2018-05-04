/*
 * Created by remi on 28/04/18.
*/

#include <malloc.h>
#include "utils.h"


Point *newPoint(int x, int y) {
    Point *res;
    res = malloc(sizeof(Point));
    res->y = y;
    res->x = x;
    return res;
}

Vector *newVector(int x, int y) {
    Vector *res;
    res = malloc(sizeof(Vector));
    res->y = y;
    res->x = x;
    return res;
}