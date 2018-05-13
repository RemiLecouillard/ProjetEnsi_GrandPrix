/*
 * Created by remi on 06/05/18.
*/

#ifndef PROJETENSI_GRANDPRIX_ANTCOLONY_H
#define PROJETENSI_GRANDPRIX_ANTCOLONY_H

#include "driver.h"
#include "graph.h"

#define MIN_PHEROMONE 10

typedef struct antcolony *AntColony;

AntColony newAntColony(Graph graph, Driver *nest, Driver walls[]);

void ACsearchPath(AntColony this);

#endif //PROJETENSI_GRANDPRIX_ANTCOLONY_H
