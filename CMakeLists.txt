cmake_minimum_required(VERSION 3.5)
project(homekit)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
include_directories(include accessory profiles hap)

set(SOURCE_FILES  main.c
        accessory/accessory.c
        profiles/light_bulb.c
        hap/bonjour.c hap/mthread.c)

add_executable(light_bulb ${SOURCE_FILES})

target_link_libraries(light_bulb libpthread.so libdns_sd.so)