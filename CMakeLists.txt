cmake_minimum_required(VERSION 3.5)
project(homekit)

SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
include_directories(include accessory profiles hap
        include/sodium
        rfc6234-master
        #Chacha20 poly1305-opt-master
        )
include_directories(/usr/include/sys)

set(SOURCE_FILES  main.c
        accessory/accessory.c
        profiles/light_bulb.c
        hap/bonjour.c hap/mthread.c hap/http_handle.c hap/http_handle.h hap/pairing.c hap/pairing.h
        rfc6234-master/hkdf.c  rfc6234-master/hmac.c  rfc6234-master/sha_m.c
        #Chacha20/chacha20_simple.c
        #poly1305-opt-master/poly1305.c
        )


link_directories(./lib)

add_executable(light_bulb ${SOURCE_FILES})
target_link_libraries(light_bulb
        -lpthread -ldns_sd -lsrp -lssl -lcrypto
        )