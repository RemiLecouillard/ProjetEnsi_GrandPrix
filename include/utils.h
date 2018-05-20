//
// Created by lecouillard on 18/04/18.
//

#ifndef PROJETENSI_GRANDPRIX_UTIL_H
#define PROJETENSI_GRANDPRIX_UTIL_H

#include <stdio.h>
#include <LinkedList.h>

typedef struct point Point;

typedef struct vector Vector;

struct point {
    int x;
    int y;
};

struct vector {
    int x;
    int y;
};

FILE *debug;

Point *newPoint(int x, int y);

Point createPoint(int x, int y);

Vector *newVector(int x, int y);

Vector createVector(int x, int y);

int PointEquals(Point, Point);

int pointIsIn(Point point, LinkedList list);

Vector vectorAdd(Vector a, Vector b);

float vectorNorm(Vector a);

#endif //PROJETENSI_GRANDPRIX_UTIL_H
