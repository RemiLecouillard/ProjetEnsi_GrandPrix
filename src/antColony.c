/*
 * Created by remi on 06/05/18.
*/

#include <graph.h>
#include <ArrayList.h>
#include <Stack.h>
#include "antColony.h"

#define TOTAL_PHEROMON 100.
#define NUMBER_OF_ANTS 100

#define _ants this->ants
#define _graph this->graph

typedef struct ant *Ant;

struct ant{
    Stack path;
    Point position;
    int gasoline;
    int boosts;
};

struct antcolony {
    Graph graph;
    struct ant ants[NUMBER_OF_ANTS];
};

void moveAnt(AntColony this,Ant ant) {

}

void ACmoveAnts(AntColony this, int firstAnt, int lastAnt) {
    int i;
    for (i = firstAnt; i < lastAnt; i ++) {
        moveAnt(this, &_ants[i]);
    }
}

void ACsearchPath(AntColony this, Driver nest, Driver walls[]) {

}