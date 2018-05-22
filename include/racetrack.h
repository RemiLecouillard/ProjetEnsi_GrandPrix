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

#ifndef PROJETENSI_GRANDPRIX_RACETRACK_H
#define PROJETENSI_GRANDPRIX_RACETRACK_H

#include <LinkedList.h>
#include "utils.h"
#include "driver.h"

typedef struct race *Racetrack;

struct race {
    int width;
    int height;
    char** array;
};

Racetrack newRacetrack();

LinkedList raceGetArrival(Racetrack racetrack);

LinkedList raceGetPossibleDestination(Racetrack racetrack, Point point);

int raceGetNodeCost(Racetrack, Point, Point);

int raceIsValidPosition(Racetrack, Point);

int raceIsArrival(Racetrack, Point);

int raceGasolineCost(Racetrack race, Point coord, Vector velocity, Vector acceleration);

int raceIsPathPosssible(Racetrack race, Point from, Point to);

int raceNoCollision(Point from, Driver walls[], Point to);

#endif //PROJETENSI_GRANDPRIX_RACETRACK_H
