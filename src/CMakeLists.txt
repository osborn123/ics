cmake_minimum_required(VERSION 3.26)
project(src C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_executable(src
        include/cache.h
        include/common.h
        include/ll.h
        include/pt.h
        include/tlb.h
        include/types.h
        cache.c
        common.c
        ll.c
        pt.c
        sim.c
        tlb.c)
