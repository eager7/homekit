cmake_minimum_required(VERSION 3.5)
project(homekit)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
include_directories(include accessory service characteristic)
set(SOURCE_FILES accessory/accessory.c characteristic/characteristic.c service/service.c)
add_executable(homekit ${SOURCE_FILES})