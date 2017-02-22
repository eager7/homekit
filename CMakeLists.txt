cmake_minimum_required(VERSION 3.5)
project(homekit)

SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -m32")
include_directories(include accessory profiles hap
        include/sodium
        rfc6234-master
        srp
        Chacha20 poly1305-opt-master ed25519-donna
        curve25519
        )
include_directories(/usr/include/sys)
link_directories(./lib)

set(SOURCE_FILES  main.c
        accessory/accessory.c
        profiles/light_bulb.c
        hap/bonjour.c hap/mthread.c hap/http_handle.c hap/ip.c hap/pairing.c hap/tlv.c hap/list.c
        rfc6234-master/hkdf.c  rfc6234-master/hmac.c  rfc6234-master/sha_m.c
        Chacha20/chacha20_simple.c
        poly1305-opt-master/poly1305.c
        ed25519-donna/ed25519.c
        curve25519/curve25519-donna.c
        )

add_library(libsrp.a STATIC
        srp/cstr.c  srp/srp6_server.c  srp/srp.c  srp/t_conf.c  srp/t_conv.c
        srp/t_math.c  srp/t_misc.c  srp/t_pw.c  srp/t_read.c  srp/t_truerand.c
        )

add_executable(light_bulb ${SOURCE_FILES} )
target_link_libraries(light_bulb
        -lpthread -ldns_sd -lssl -lcrypto -ljson-c
        -lsodium libsrp.a
        )