/*
 * Created by remi on 06/05/18.
*/

#include <graph.h>
#include <ArrayList.h>
#include <Stack.h>
#include <time.h>
#include "antColony.h"

#define NUMBER_OF_ANTS 200
#define PHEROMONE 400
#define MAX_PHEROMONE 120
#define EVAPORATION_RATE 0.3
#define NUMBER_ITERATION 5000

#define _ants this->ants
#define _graph this->graph
#define _nest this->nest
#define _walls this->walls

#ifdef ANT
typedef struct ant *Ant;

typedef struct pathnode *PathNode;

struct ant{
    Stack path;
    ArrayList visited;
    Point position;
    Vector velocity;
    int gasoline;
    int boosts;
};

struct antcolony {
    Graph graph;
    struct ant ants[NUMBER_OF_ANTS];
    Driver *nest;
    Driver* walls;
};

struct pathnode {
    Point point;
    Vector velocity;
};

void initAnt(AntColony this, Ant ant);

int isDead(Ant ant);

void backToNest(AntColony this, Ant ant);

PathNode newPathNode(Point p, Vector v);

EdgeVelocity getRandomDestination(AntColony this, Ant ant);

void evaporate(VertexVelocity);

int min(int a, int b);

AntColony newAntColony(Graph graph, Driver *nest, Driver walls[]) {
    AntColony this;

    this = malloc(sizeof(struct antcolony));


    _graph = graph;
    _nest = nest;
    _walls = walls;

    return this;
}

void moveAnt(AntColony this,Ant ant) {
    EdgeVelocity edge;

    if (isDead(ant)) {
        //fprintf(debug, "is dead\n");
        //fflush(debug);
        initAnt(this, ant);
        return;
    }
    //fprintf(debug, "is alive\n");
    //fflush(debug);
    if (raceIsArrival(_graph->racetrack, ant->position)) {
        //fprintf(debug, "is at arrival\n");
      //  fflush(debug);
        backToNest(this, ant);
        return;
    }
    //fprintf(debug, "choose destination...\n");
    //fflush(debug);
    edge = getRandomDestination(this, ant);
    //fprintf(debug, "over\n");
    //fflush(debug);
    if (edge == NULL) {
        //fprintf(debug, "No destination possible :(\n");
        //fflush(debug);
        initAnt(this, ant);
        return;
    }
    //fprintf(debug, "choosed %d %d\n", edge->to.x, edge->to.y);
    //fflush(debug);
    ant->velocity = vectorAdd(ant->velocity, edge->acceleration);
    ant->position = edge->to;
    ant->gasoline -= edge->gasolineCost;
    ant->boosts -= edge->boostCost;

    StackAdd(ant->path, newPathNode(ant->position, ant->velocity));
    ArrayListAppend(ant->visited, newPoint(ant->position.x, ant->position.y));
    //fprintf(debug, "finish, added to the path\n");
    //fflush(debug);
}

void debugAnt(AntColony this, Ant ant, int i) {
    fprintf(debug, "\n************* %d ant %d %d with %d %d *************\n", i, ant->position.x, ant->position.y, ant->velocity.x, ant->velocity.y);
    fprintf(debug, "gasoline left %d, boost left %d\n", ant->gasoline, ant->boosts);
    fflush(debug);
}

void ACmoveAnts(AntColony this, int firstAnt, int lastAnt) {
    int i;

    for (i = firstAnt; i < lastAnt; i ++) {
        //debugAnt(this, &_ants[i], i);
        moveAnt(this, &_ants[i]);
    }
}

void ACsearchPath(AntColony this) {
    int i;
    clock_t begin;

    //fprintf(debug, "Init ants\n");
    //fflush(debug);
    begin = clock();

    for (i = 0; i < NUMBER_OF_ANTS; i ++) {
        initAnt(this, &_ants[i]);
    }

    i = NUMBER_ITERATION;

    //fprintf(debug, "Move all ants\n");
    //fflush(debug);
    srand(time(NULL));

    while(clock() - begin < CLOCKS_PER_SEC*0.9) {
        ACmoveAnts(this, 0, NUMBER_OF_ANTS);
        i++;
        if (i % 20 == 0) {
           graphVertexVelocityForEach(_graph, evaporate);
        }
    }

    fprintf(debug, "number of iteration %d\n", i);
}

void initAnt(AntColony this, Ant ant) {
    if (ant->path) {
        StackDelete(ant->path);
    }
    if (ant->visited) {
        ArrayListDelete(ant->visited);
    }
    ant->visited = newArrayList(sizeof(Point));
    ant->path = newStack();
    ant->gasoline = _nest->gasoline;
    ant->boosts = _nest->boostLeft;
    ant->position = _nest->position;
    ant->velocity = _nest->velocity;
}

int isDead(Ant ant) {
    return ant->gasoline <= 0 || ant->boosts < 0;
}

void backToNest(AntColony this, Ant ant) {
    PathNode path = NULL;
    int pheromone;
    fprintf(debug, "path found !!! gasoline %d \n", ant->gasoline);

    while(!StackIsEmpty(ant->path)) {
        path = StackPop(ant->path);
        pheromone = ( graphVertexVelocityGetPheromone(_graph, path->point, path->velocity) + (PHEROMONE /(int)ArrayListGetLength(ant->visited)));
        pheromone = min(pheromone, MAX_PHEROMONE);
        graphVertexVelocitySetPheromone(_graph, path->point, path->velocity, pheromone);
        free(path);
    }

    fflush(debug);
    initAnt(this, ant);
}

PathNode newPathNode(Point p, Vector v) {
    PathNode node;

    node = malloc(sizeof(struct pathnode));

    node->velocity = v;
    node->point = p;

    return node;
}

EdgeVelocity getRandomDestination(AntColony this, Ant ant) {
    LinkedList neighbors; /*edges */
    EdgeVelocity edge;
    int choice;
    int total;
    int i;
    int heuristic, increment;
    int array[25]; /* maximum destination possible */

    total = 0;
    i = 0;

    neighbors = graphVertexVelocityGetNeighbors(_graph, ant->position, ant->velocity);

    LinkedListResetCurrent(neighbors);

    if (LinkedListGetLength(neighbors) == 0) {
        return NULL;
    }

    while (LinkedListMoveCurrentNext(neighbors)) {
        edge = (EdgeVelocity) LinkedListGetCurrent(neighbors);
        if (!ArrayListContainsValue(ant->visited, newPoint(edge->to.x, edge->to.y))) {
            heuristic = graphVertexIsDijkstraPath(_graph, edge->to) == 1 ? 4 : 1;
            heuristic *= edge->acceleration.x == 1 ? 4 : 1;
            increment = graphVertexVelocityGetPheromone(_graph, edge->to, vectorAdd(ant->velocity, edge->acceleration)) * heuristic;
            total += min(increment, MAX_PHEROMONE);
        }
        array[i++] = total;
    }

    if (total == 0) {
        return NULL;
    }

    choice = rand() % total;

    for (i = i - 1; i > 0; i --) {
        if (array[i - 1] <= choice)
            break;
    }

    return (EdgeVelocity) LinkedListGetAtIndex(neighbors, i);
}

void evaporate(VertexVelocity vertex) {
    vertex->pheromone = (int) (vertex->pheromone - MIN_PHEROMONE) * EVAPORATION_RATE + MIN_PHEROMONE;
}

int min(int a, int b) {
    return a > b ? b : a;
}

#endif