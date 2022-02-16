cmake_minimum_required(VERSION 3.21)
project(SER334_MOD_7 C)

set(CMAKE_C_STANDARD 99)

add_executable(SER334_MOD_7 User.c CovertInstanceHost.c CovertLoadBalancer.c
        )

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")

