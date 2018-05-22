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

/**
 *
 * @return
 */
Racetrack newRacetrack();

/**
 *
 * @param racetrack
 * @return
 */
LinkedList raceGetArrival(Racetrack racetrack);

/**
 *
 * @param racetrack
 * @param point
 * @return
 */
LinkedList raceGetPossibleDestination(Racetrack racetrack, Point point);

/**
 *
 * @param race
 * @param from
 * @param to
 * @return
 */
int raceGetNodeCost(Racetrack race, Point from, Point to);

/**
 *
 * @param race
 * @param p
 * @return
 */
int raceIsValidPosition(Racetrack race, Point p);

/**
 *
 * @param race
 * @return
 */
int raceIsArrival(Racetrack race, Point);

/**
 *
 * @param race
 * @param coord
 * @param velocity
 * @param acceleration
 * @return
 */
int raceGasolineCost(Racetrack race, Point coord, Vector velocity, Vector acceleration);

/**
 *
 * @param race
 * @param from
 * @param to
 * @return
 */
int raceIsPathPosssible(Racetrack race, Point from, Point to);

/**
 *
 * @param from
 * @param walls
 * @param to
 * @return
 */
int raceNoCollision(Point from, Driver walls[], Point to);

#endif //PROJETENSI_GRANDPRIX_RACETRACK_H
