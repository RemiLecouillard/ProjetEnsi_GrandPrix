//
// Created by lecouillard on 18/04/18.
//


#include <stdlib.h>
#include "driver.h"
#include "raceManagerInterface.h"
#include "racetrack.h"

struct racemanagerinterface {
    Racetrack racetrack;
};


void readUntilEndOfLine();

void init(Racetrack race,int* gasoline) {
    int i;

    (void)fscanf(stdin, "%d %d %d", &race->width, &race->height, gasoline);
    readUntilEndOfLine();

    race->array = malloc(sizeof(char*) * race->height);
    for (i = 0; i < race->height; i++) {
        race->array[i] = malloc(sizeof(char) * race->width);
        (void)fread(race->array[i], sizeof(char),race->width, stdin);
        readUntilEndOfLine();
    }

}

int updatePositions(Driver* myDriver,Driver* otherDriver1,Driver* otherDriver2) {
    fflush(debug);
    Point pos;
    (void)fscanf(stdin, "%d %d\t%d %d\t%d %d", &pos.x, &pos.y, &otherDriver1->position.x,
           &otherDriver1->position.y, &otherDriver2->position.x, &otherDriver2->position.y);
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