cmake_minimum_required(VERSION 4.2)
project(code C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(code
        ca.c
        ht.c
        ht.h
        main.c)

add_executable(fib
        fib.c
)