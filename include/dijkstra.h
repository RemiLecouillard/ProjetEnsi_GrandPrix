/*
 * Created by remi on 04/05/18.
*/

#ifndef PROJETENSI_GRANDPRIX_DIJKSTRA_H
#define PROJETENSI_GRANDPRIX_DIJKSTRA_H

typedef struct dijkstra *Dijkstra;

struct dijkstra *newDijkstra(Graph graph,LinkedList source,LinkedList destination, Driver walls[]);

void dijkstraFindShortestPath(struct dijkstra *this);

void dijkstraDelete(struct dijkstra *this);

#endif //PROJETENSI_GRANDPRIX_DIJKSTRA_H
