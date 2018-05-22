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
 * @date 28/04/18.
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