//
// Created by lecouillard on 18/04/18.
//

#include <limits.h>
#include <time.h>

#include <Stack.h>

#include <utils.h>
#include <graph.h>
#include <racetrack.h>
#include <driver.h>
#include <dijkstra.h>
#include <raceManager.h>


Vector getBestDirection(Racetrack racetrack,Driver *myDriver,Driver *otherDriver1,Driver *otherDriver2) {
    static Stack stack = NULL;
    static Graph graph = NULL;
    Point source, next, *tmp;
    Vector acceleration;
    LinkedList arrivals;
    clock_t start_t, end_t;

    if (!stack) {
        start_t = clock();
        fflush(debug);
        graph = newGraph(racetrack);
        source = myDriver->position;
        arrivals = raceGetArrival(racetrack);
        stack = dijkstra(graph, source, arrivals);
        end_t = clock();
        fprintf(debug, "Time taken start %ld end %ld time : %lf sec\n%p path\n", start_t, end_t, ((double)(end_t - start_t) / CLOCKS_PER_SEC), stack);
        fflush(debug);
        displayDebug(racetrack, 0);
    }
    tmp = StackPop(stack);
    next = *tmp;
    free(tmp);
    acceleration = driverGetNeededAcceleration(myDriver, next);
    fprintf(debug, "***********Action*************\ndriver coord %d %d\ndriver velocity %d %d\n", myDriver->position.x, myDriver->position.y,myDriver->velocity.x, myDriver->velocity.y);
    fprintf(debug, "Point %d %d\nacceleration -> %d %d\n\n", next.x, next.y, acceleration.x, acceleration.y);
    fflush(debug);
    driverAddAcceleration(myDriver, acceleration);
    return acceleration;
}