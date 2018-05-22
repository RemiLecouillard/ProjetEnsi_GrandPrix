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

#include <stdlib.h>
#include "driver.h"
#include "raceManagerInterface.h"

struct racemanagerinterface {
    Racetrack racetrack;
};


void readUntilEndOfLine();

void init(Racetrack race,int* gasoline) {
    int i;
    int ignored __attribute__((unused));

    if (fscanf(stdin, "%d %d %d", &race->width, &race->height, gasoline) == EOF)
        exit(1);
    readUntilEndOfLine();

    race->array = malloc(sizeof(char*) * race->height);
    for (i = 0; i < race->height; i++) {
        race->array[i] = malloc(sizeof(char) * race->width);
        ignored = fread(race->array[i], sizeof(char),race->width, stdin);
        readUntilEndOfLine();
    }

}

int updatePositions(Driver* myDriver,Driver* otherDriver1,Driver* otherDriver2) {
    fflush(debug);
    Point pos;
    if (fscanf(stdin, "%d %d\t%d %d\t%d %d", &pos.x, &pos.y, &otherDriver1->position.x,
           &otherDriver1->position.y, &otherDriver2->position.x, &otherDriver2->position.y) == EOF)
        exit(1);
    readUntilEndOfLine();
    if (!PointEquals(createPoint(-1, -1), myDriver->position))
        myDriver->velocity = createVector(pos.x - myDriver->position.x, pos.y - myDriver->position.y);
    myDriver->position = pos;
    return 0;
}

void sendOrder(Vector acceleration) {
    fprintf(stdout, "%d %d", acceleration.x, acceleration.y);
    fflush(stdout);
}

void readUntilEndOfLine() {
    char trash;
    while (fread(&trash, sizeof(char), 1, stdin) == 1 && trash != '\n');
}