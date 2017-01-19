cmake_minimum_required(VERSION 3.5)
project(homekit)

SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
include_directories(include accessory profiles hap srp rfc6234-master)

set(SOURCE_FILES  main.c
        accessory/accessory.c
        profiles/light_bulb.c
        hap/bonjour.c hap/mthread.c hap/http_handle.c hap/http_handle.h hap/pairing.c hap/pairing.h
        srp/cstr.c  srp/srp6_server.c  srp/srp.c  srp/t_conf.c  srp/t_conv.c  srp/t_math.c
        srp/t_misc.c  srp/t_pw.c  srp/t_read.c  srp/t_truerand.c
        rfc6234-master/hkdf.c  rfc6234-master/hmac.c  rfc6234-master/sha.c

        )

add_executable(light_bulb ${SOURCE_FILES})

target_link_libraries(light_bulb libpthread.so libdns_sd.so libssl.so libcrypto.so)