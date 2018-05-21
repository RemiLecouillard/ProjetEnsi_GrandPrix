/*
 * Created by remi on 20/05/18.
*/

#ifndef PROJETENSI_GRANDPRIX_POSSIBILITYTREE_H
#define PROJETENSI_GRANDPRIX_POSSIBILITYTREE_H


#include <Stack.h>
#include "graph.h"

Stack shortestPath(Graph graph, Driver start, Driver* walls);

#endif //PROJETENSI_GRANDPRIX_POSSIBILITYTREE_H
