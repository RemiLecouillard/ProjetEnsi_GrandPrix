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
 * @date 22/04/18
*/

#ifndef PROJETENSI_GRANDPRIX_GRAPH_H
#define PROJETENSI_GRANDPRIX_GRAPH_H


#include <LinkedList.h>

#include <racetrack.h>

typedef struct graph_t *Graph;

typedef struct vertex_t *Vertex;

typedef struct edge_t *Edge;

typedef struct vertexvelocity *VertexVelocity;

typedef struct edgevelocity *EdgeVelocity;

struct graph_t {
    Racetrack racetrack;
    Vertex** vertices;
};

struct vertex_t {
    int distanceFromSource;
    int isSetToExplore;
    int isDijkstraPath;
    Point previousVertex;
    LinkedList neighbors; /* edges */
    VertexVelocity inPutVelocity[11][11];
};

struct edge_t {
    Point to;
    int weight;
};

struct vertexvelocity {
    LinkedList possibleDestination;
#ifdef ANT
    int pheromone;
#endif
};

struct edgevelocity {
    Point to;
    Vector acceleration; // x e [-2; 2] y  e [-2; 2]
    int gasolineCost;
    int boostCost;
};

Graph newGraph(Racetrack racetrack);

void graphVertexSetDistance(Graph graph,Point coord,int dist);

int graphVertexGetDistance(Graph graph,Point coord);

void graphVertexSetPrevious(Graph graph,Point coord,Point prevCoord);

Point graphVertexGetPrevious(Graph graph,Point coord);

LinkedList graphVertexGetNeighbors(Graph graph,Point point);

int graphVertexIsInQueue(Graph,Point);

void graphVertexSetInQueue(Graph,Point);

void graphVertexSetDijkstraPath(Graph, Point, int);

int graphVertexIsDijkstraPath(Graph, Point);

void graphInitDijkstra(Graph);

LinkedList graphVertexVelocityGetNeighbors(Graph this, Point from, Vector velocity);

void graphVertexVelocityForEach(Graph this, void (*foreach)(VertexVelocity));

#ifdef ANT
Vector graphGetDirectionWithMostPheromone(Graph this, Point from, Vector velocity, Point *to);

int graphVertexVelocityGetPheromone(Graph this, Point from, Vector velocity);

void graphVertexVelocitySetPheromone(Graph this, Point from, Vector velocity, int pheromone);
#endif

#endif //PROJETENSI_GRANDPRIX_GRAPH_H
