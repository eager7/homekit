cmake_minimum_required(VERSION 3.5)
project(homekit)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
include_directories(include accessory service characteristic profiles)

set(SOURCE_FILES  main.c
        accessory/accessory.c
        profiles/light_bulb.c
)

add_executable(light_bulb ${SOURCE_FILES})