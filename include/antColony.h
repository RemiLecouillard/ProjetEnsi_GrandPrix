/*
 * Created by remi on 06/05/18.
*/

#ifndef PROJETENSI_GRANDPRIX_ANTCOLONY_H
#define PROJETENSI_GRANDPRIX_ANTCOLONY_H

#include "driver.h"

typedef struct antcolony *AntColony;

AntColony newAntColony(Graph graph);

void ACsearchPath(AntColony this, Driver nest, Driver walls[]);

#endif //PROJETENSI_GRANDPRIX_ANTCOLONY_H
