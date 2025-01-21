cmake_minimum_required(VERSION 3.30)
project(Nearest_neighbor_search C)

set(CMAKE_C_STANDARD 11)

add_executable(Nearest_neighbor_search src/main.c
        src/vector.c
        src/vector.h
        src/vector_store.c
        src/vector_store.h
        src/priority_queue.c
        src/priority_queue.h)
