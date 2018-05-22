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
 * @date 04/05/18
*/

#ifndef PROJETENSI_GRANDPRIX_DIJKSTRA_H
#define PROJETENSI_GRANDPRIX_DIJKSTRA_H

/**
 *
 */
typedef struct dijkstra *Dijkstra;

/**
 *
 * @param graph
 * @param source
 * @param destination
 * @param walls
 * @return
 */
struct dijkstra *newDijkstra(Graph graph,LinkedList source,LinkedList destination, Driver walls[]);

/**
 *
 * @param this
 */
void dijkstraFindShortestPath(struct dijkstra *this);

/**
 *
 * @param this
 */
void dijkstraDelete(struct dijkstra *this);

#endif //PROJETENSI_GRANDPRIX_DIJKSTRA_H
