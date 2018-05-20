cmake_minimum_required(VERSION 3.7)
project(ProjetEnsi_GrandPrix)

set(CMAKE_CXX_STANDARD 11)

include_directories(include CCollections/include)
LINK_DIRECTORIES(lib)

set(SOURCE_FILES
        include/raceManagerInterface.h
        include/utils.h
        src/main.c
        src/raceManagerInterface.c src/graph.c include/graph.h src/utils.c src/raceManager.c include/raceManager.h src/racetrack.c include/racetrack.h src/driver.c include/driver.h src/dijkstra.c include/dijkstra.h src/antColony.c include/antColony.h src/possibilityTree.c include/possibilityTree.h)

add_executable(ProjetEnsi_GrandPrix ${SOURCE_FILES})

target_link_libraries(ProjetEnsi_GrandPrix CCollections)
