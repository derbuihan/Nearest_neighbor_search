cmake_minimum_required(VERSION 3.30)
project(Nearest_neighbor_search C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(ARROW REQUIRED IMPORTED_TARGET arrow)
pkg_check_modules(PARQUET REQUIRED IMPORTED_TARGET parquet)

add_executable(Nearest_neighbor_search src/main.c
        src/vector.c
        src/vector.h
        src/linear_store.c
        src/linear_store.h
        src/priority_queue.c
        src/priority_queue.h
        src/vector_store.c
        src/vector_store.h
        src/nsw_store.c
        src/nsw_store.h
        src/load_datasets.cpp
        src/load_datasets.h)

target_include_directories(Nearest_neighbor_search
        PRIVATE
        ${ARROW_INCLUDE_DIRS}
        ${PARQUET_INCLUDE_DIRS}
)

target_link_libraries(Nearest_neighbor_search
        PRIVATE
        PkgConfig::ARROW
        PkgConfig::PARQUET
)

enable_testing()

add_executable(test_vector tests/test_vector.c src/vector.c)
target_include_directories(test_vector PRIVATE ${CMAKE_SOURCE_DIR}/src)
add_test(NAME VectorTest COMMAND test_vector)

add_executable(test_priority_queue tests/test_priority_queue.c src/priority_queue.c)
target_include_directories(test_priority_queue PRIVATE ${CMAKE_SOURCE_DIR}/src)
add_test(NAME PriorityQueueTest COMMAND test_priority_queue)
