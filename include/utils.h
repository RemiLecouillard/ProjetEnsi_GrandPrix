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
 * @date 18/04/18
*/

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
