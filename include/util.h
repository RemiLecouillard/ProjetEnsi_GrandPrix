//
// Created by lecouillard on 18/04/18.
//

#ifndef PROJETENSI_GRANDPRIX_UTIL_H
#define PROJETENSI_GRANDPRIX_UTIL_H

typedef struct race Racetrack;

typedef struct driver Driver;

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

struct race{
    int width;
    int height;
    char** array;
};

struct driver{
    Point position;
    Vector velocity;
    int gasoline;
};

#endif //PROJETENSI_GRANDPRIX_UTIL_H
