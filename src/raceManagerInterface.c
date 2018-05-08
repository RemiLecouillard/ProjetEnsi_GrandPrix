//
// Created by lecouillard on 18/04/18.
//


#include <stdlib.h>
#include "driver.h"
#include "raceManagerInterface.h"
#include "racetrack.h"

struct racemanagerinterface {
    FILE *in;
    FILE *out;
    Racetrack racetrack;
};


void readUntilEndOfLine();

void init(Racetrack race,int* gasoline) {
    int i;

    fscanf(stdin, "%d %d %d", &race->width, &race->height, gasoline);
    readUntilEndOfLine();

    race->array = malloc(sizeof(char*) * race->height);
    for (i = 0; i < race->height; i++) {
        race->array[i] = malloc(sizeof(char) * race->width);
        fread(race->array[i], sizeof(char),race->width, stdin);
        readUntilEndOfLine();
    }

}

int updatePositions(Driver* myDriver,Driver* otherDriver1,Driver* otherDriver2) {
    fflush(debug);
    fscanf(stdin, "%d %d\t%d %d\t%d %d", &myDriver->position.x, &myDriver->position.y, &otherDriver1->position.x,
           &otherDriver1->position.y, &otherDriver2->position.x, &otherDriver2->position.y);
    readUntilEndOfLine();
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