/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
*/

/**
 * @author Rémi Lecouillard
 * @author Baptiste Dromard
 * @date 30/04/18
*/

#include <math.h>

#include "racetrack.h"

#define ARRIVAL '='
#define SAND '~'
#define OUTSIDE '.'
#define ROAD '#'

/**
 * @brief Structure pour le position d'un element sur la map avec des
 * coordonnees entieres
 */
typedef struct {
    int x;
    int y;
} pos2Dint;

/**
 * @brief Structure pour une position 2D avec des coordonnees flottantes
 */
typedef struct {
    float x;
    float y;
} pos2Dfloat;

/**
 * @brief Structure permettant de definir un segment et de se deplacer le long
 */
typedef struct {
    pos2Dint posStart;    /*< Debut du segment */
    pos2Dint posEnd;      /*< Fin du segment */
    pos2Dfloat posActual; /*< Position Actuel sur le segment */
    pos2Dfloat delta;     /*< Variation en x et en y pour le deplacement */
    int len;              /*< Longueur suivant l'axe principale */
    int pos;              /*< Position de reference sur la ligne */
} infoLine;

/**
 * @brief Definition d'un segment entre deux points
 */
void line(int x1, int y1, int x2, int y2, infoLine *data);

/**
 * @brief Deplacement sur un segment dans un sens donne (+1 vers la fin, et -1
 * vers le debut)
 */
int nextPoint(infoLine *data, pos2Dint *point, int sens);

void line(int x1, int y1, int x2, int y2, infoLine *data) {
    data->posStart.x = x1;
    data->posStart.y = y1;
    data->posActual.x = x1 + 0.5;
    data->posActual.y = y1 + 0.5;
    data->posEnd.x = x2;
    data->posEnd.y = y2;
    int adxi, adyi, dxi, dyi;
    adxi = dxi = x2 - x1;
    adyi = dyi = y2 - y1;
    if (adxi < 0)
        adxi = -dxi;
    if (adyi < 0)
        adyi = -dyi;
    data->pos = 0;
    data->len = adxi;
    if (adyi > adxi)
        data->len = adyi;
    data->delta.x = ((float)dxi) / data->len;
    data->delta.y = ((float)dyi) / data->len;
}

int nextPoint(infoLine *data, pos2Dint *point, int sens) {
    if (sens > 0) {
        if (data->pos == data->len) {
            point->x = data->posEnd.x;
            point->y = data->posEnd.y;
            return -1; // La fin de la ligne est atteinte
        }
        data->posActual.x += data->delta.x;
        data->posActual.y += data->delta.y;
        point->x = ((int)data->posActual.x);
        point->y = ((int)data->posActual.y);
        data->pos++;
        return 1; // un nouveau point est déterminé.
    }
    if (sens < 0) {
        if (data->pos == 0) {
            point->x = data->posStart.x;
            point->y = data->posStart.y;
            return -1; // La fin de la ligne est atteinte
        }
        data->posActual.x -= data->delta.x;
        data->posActual.y -= data->delta.y;
        point->x = ((int)data->posActual.x);
        point->y = ((int)data->posActual.y);
        data->pos--;

        return 1; // un nouveau point est déterminé.
    }
    return 1; // Avec sens==0, il n'y a pas de déplacement
}


Racetrack newRacetrack() {
    return malloc(sizeof(struct race));
}

LinkedList raceGetArrival(Racetrack racetrack) {
    int i, j;
    LinkedList list;

    list = newLinkedList();

    for (i = 0; i < racetrack->height; i ++) {
        for (j = 0; j < racetrack->width; j ++) {
            if (racetrack->array[i][j] == ARRIVAL) {
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
                if (racetrack->array[point.y][point.x] == SAND && vectorNorm(createVector(i,j)) > 1.) {
                    continue;
                }
                if (racetrack->array[y][x] != OUTSIDE) {
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
        cost -= 5;
    }

    if (race->array[to.y][to.x] == SAND) {
        cost += 20;
    }

    return cost;
}

int raceIsValidPosition(Racetrack race, Point coord) {
    if (coord.x < 0 || coord.y < 0 || coord.x >= race->width || coord.y >= race->height) {
        return 0;
    }

    return race->array[coord.y][coord.x] != OUTSIDE;
}

int raceIsArrival(Racetrack race, Point coord) {
    if (!raceIsValidPosition(race, coord)) return 1;
    return race->array[coord.y][coord.x] == ARRIVAL;
}

int raceGasolineCost(Racetrack race, Point coord, Vector velocity, Vector acceleration) {
    int value = acceleration.x * acceleration.x + acceleration.y * acceleration.y;
    value += (int)(sqrt(velocity.x * velocity.x + velocity.y * velocity.y) * 3.0 / 2.0);
    if (race->array[coord.y][coord.x] == SAND)
        value += 1;
    return value;
}

int raceIsPathPosssible(Racetrack race, Point from, Point to) {
    infoLine vline;
    pos2Dint p;
    float norm;

    if(!raceIsValidPosition(race, to)) {
        return 0;
    }

    norm = vectorNorm(createVector(to.x - from.x, to.y - from.y));

    if (norm > 5.) {
        return 0;
    }

    if (race->array[from.y][from.x] == SAND && norm > 1.) {
        return 0;
    }

    line(from.x, from.y, to.x, to.y, &vline);
    while (nextPoint(&vline, &p, +1) > 0) {
        if (p.x == from.x && p.y == from.y)
            continue;
        if (!raceIsValidPosition(race, createPoint(p.x, p.y))) {
            return 0;
        }
    }

    return 1;
}

int raceNoCollision(Point from, Driver walls[], Point to) {
    infoLine vline;
    pos2Dint p;
    Point tmp;
    line(from.x, from.y, to.x, to.y, &vline);
    while (nextPoint(&vline, &p, +1) > 0) {
        if (p.x == from.x && p.y == from.y)
            continue;
        tmp = createPoint(p.x, p.y);
        if (PointEquals(walls[0].position, tmp) || PointEquals(walls[1].position, tmp)) {
            return 0;
        }
    }
    return 1;
}