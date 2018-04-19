cmake_minimum_required(VERSION 3.7)
project(ProjetEnsi_GrandPrix)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

set(SOURCE_FILES
        include/galileo.h
        include/raceManagerInterface.h
        include/util.h
        src/galileo.c
        src/main.c
        src/raceManagerInterface.c)

add_executable(ProjetEnsi_GrandPrix ${SOURCE_FILES})