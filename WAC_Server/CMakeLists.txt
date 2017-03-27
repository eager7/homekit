cmake_minimum_required(VERSION 3.5)
project(wac)
SET(TARGET wac_demo)

SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -m32")
include_directories(
		../Include
		Platform Sources Support
		External/crypto
		External/Curve25519
		External/GladmanAES
        )
include_directories(/usr/include/sys)
link_directories(./lib)

set(SOURCE_FILES  
		Sources/WACBonjour.c  Sources/WACServer.c
		Support/AESUtils.c       Support/HTTPServer.c  Support/MFiSAPServer.c   Support/SHAUtils.c     Support/StringUtils.c  Support/TLVUtils.c
		Support/AppleDeviceIE.c  Support/HTTPUtils.c   Support/SecurityUtils.c  Support/SocketUtils.c  Support/TimeUtils.c    Support/URLUtils.c
		Platform/PlatformApplyConfiguration.c  Platform/PlatformMFiAuth.c       Platform/PlatformSoftwareAccessPoint.c
		Platform/PlatformBonjour.c             Platform/PlatformRandomNumber.c  Platform/WACMain.c
		External/crypto/aes.c     External/crypto/bigint.c  External/crypto/md5.c  External/crypto/rsakey.c  External/crypto/sha1.c
		External/crypto/base64.c  External/crypto/hmac.c    External/crypto/rc4.c  External/crypto/rsapem.c
		External/Curve25519/curve25519-donna.c  External/Curve25519/curve25519-donna-c64.c  External/Curve25519/curve25519-donna-test.c
		External/GladmanAES/aescrypt.c  External/GladmanAES/aes_modes.c  External/GladmanAES/gcm.c
		External/GladmanAES/aeskey.c    External/GladmanAES/aestab.c     External/GladmanAES/gf128mul.c

		)

#add_library(libsrp.a STATIC)

add_executable(${TARGET} ${SOURCE_FILES} )
#target_link_libraries(${TARGET} -lpthread)
