cmake_minimum_required(VERSION 3.30)
project(Nearest_neighbor_search C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(ARROW REQUIRED IMPORTED_TARGET arrow)
pkg_check_modules(PARQUET REQUIRED IMPORTED_TARGET parquet)

set(SRCS
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
add_executable(main src/main.c ${SRCS})
target_include_directories(main PRIVATE ${ARROW_INCLUDE_DIRS} ${PARQUET_INCLUDE_DIRS})
target_link_libraries(main PRIVATE PkgConfig::ARROW PkgConfig::PARQUET)

enable_testing()
set(TEST_SRCS
        tests/test_vector.c
        tests/test_priority_queue.c
        tests/test_load_datasets.c
        tests/test_linear_store.c
        tests/test_vector_store.c
        tests/test_nsw_store.c)
foreach (test_src ${TEST_SRCS})
    get_filename_component(test_name ${test_src} NAME_WE)
    add_executable(${test_name} ${test_src} ${SRCS})
    target_include_directories(${test_name} PRIVATE ${CMAKE_SOURCE_DIR}/src)
    target_link_libraries(${test_name} PRIVATE PkgConfig::ARROW PkgConfig::PARQUET)
    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach ()

set(BENCHMARK_SRCS
        benchmarks/benchmark_vector_store.c
)
foreach (benchmark_src ${BENCHMARK_SRCS})
    get_filename_component(benchmark_name ${benchmark_src} NAME_WE)
    add_executable(${benchmark_name} ${benchmark_src} ${SRCS})
    target_include_directories(${benchmark_name} PRIVATE ${CMAKE_SOURCE_DIR}/src)
    target_link_libraries(${benchmark_name} PRIVATE PkgConfig::ARROW PkgConfig::PARQUET)
    add_test(NAME ${benchmark_name} COMMAND ${benchmark_name})
endforeach ()
