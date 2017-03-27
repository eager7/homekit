#
#   File:    Makefile
#   Package: WACServer
#   Version: WAC_POSIX_Server_1.20
#   
#   Disclaimer: IMPORTANT: This Apple software is supplied to you, by Apple Inc. ("Apple"), in your
#   capacity as a current, and in good standing, Licensee in the MFi Licensing Program. Use of this
#   Apple software is governed by and subject to the terms and conditions of your MFi License,
#   including, but not limited to, the restrictions specified in the provision entitled ”Public
#   Software”, and is further subject to your agreement to the following additional terms, and your
#   agreement that the use, installation, modification or redistribution of this Apple software
#   constitutes acceptance of these additional terms. If you do not agree with these additional terms,
#   please do not use, install, modify or redistribute this Apple software.
#   
#   Subject to all of these terms and in consideration of your agreement to abide by them, Apple grants
#   you, for as long as you are a current and in good-standing MFi Licensee, a personal, non-exclusive
#   license, under Apple's copyrights in this original Apple software (the "Apple Software"), to use,
#   reproduce, and modify the Apple Software in source form, and to use, reproduce, modify, and
#   redistribute the Apple Software, with or without modifications, in binary form. While you may not
#   redistribute the Apple Software in source form, should you redistribute the Apple Software in binary
#   form, you must retain this notice and the following text and disclaimers in all such redistributions
#   of the Apple Software. Neither the name, trademarks, service marks, or logos of Apple Inc. may be
#   used to endorse or promote products derived from the Apple Software without specific prior written
#   permission from Apple. Except as expressly stated in this notice, no other rights or licenses,
#   express or implied, are granted by Apple herein, including but not limited to any patent rights that
#   may be infringed by your derivative works or by other works in which the Apple Software may be
#   incorporated.
#   
#   Unless you explicitly state otherwise, if you provide any ideas, suggestions, recommendations, bug
#   fixes or enhancements to Apple in connection with this software (“Feedback”), you hereby grant to
#   Apple a non-exclusive, fully paid-up, perpetual, irrevocable, worldwide license to make, use,
#   reproduce, incorporate, modify, display, perform, sell, make or have made derivative works of,
#   distribute (directly or indirectly) and sublicense, such Feedback in connection with Apple products
#   and services. Providing this Feedback is voluntary, but if you do provide Feedback to Apple, you
#   acknowledge and agree that Apple may exercise the license granted above without the payment of
#   royalties or further consideration to Participant.
#   
#   The Apple Software is provided by Apple on an "AS IS" basis. APPLE MAKES NO WARRANTIES, EXPRESS OR
#   IMPLIED, INCLUDING WITHOUT LIMITATION THE IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY
#   AND FITNESS FOR A PARTICULAR PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND OPERATION ALONE OR
#   IN COMBINATION WITH YOUR PRODUCTS.
#   
#   IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL OR CONSEQUENTIAL DAMAGES
#   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
#   PROFITS; OR BUSINESS INTERRUPTION) ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION, MODIFICATION
#   AND/OR DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED AND WHETHER UNDER THEORY OF CONTRACT, TORT
#   (INCLUDING NEGLIGENCE), STRICT LIABILITY OR OTHERWISE, EVEN IF APPLE HAS BEEN ADVISED OF THE
#   POSSIBILITY OF SUCH DAMAGE.
#   
#   Copyright (C) 2013 Apple Inc. All Rights Reserved.
#
#   Build Instructions
#   ------------------
#   See README.txt
#
#   Build Options
#   -------------
#   debug            -- 1=Compile in debug code, asserts, logs, etc. 0=Strip out debug code for a release build.
#   trace            -- 1=Compile in logs for function entry. 0=Strip out function entry prints. (Must have debug=1)
#   time_platform    -- 1=Compile in platform function timer macro. 0=Strip out function timer macro.
#

# Stop and print error if the caller has not included the platform specific makefile
ifeq ($(platform_makefile),)
platform_makefile=Platform/Platform.include.mk
#$(error Caller needs to include platform specific Makefile. See example, Platform/PlatformMakefileExample. Usage: platform_makefile=<path>/<makefile>)
endif

# SRCROOT should always be the current directory
SRCROOT         = $(CURDIR)

# .o directory
ODIR            = obj

# Include Platform Makefile
include         $(platform_makefile)

# WAC Source VPATHS
VPATH           += $(SRCROOT)/Sources
VPATH           += $(SRCROOT)/Support
VPATH           += $(SRCROOT)/Platform

# WAC External VPATHS
VPATH           += $(SRCROOT)/External/Curve25519
VPATH           += $(SRCROOT)/External/GladmanAES
VPATH           += $(SRCROOT)/External/crypto

# WAC Source Include Paths
INCLUDES        += -I$(SRCROOT)/Sources
INCLUDES        += -I$(SRCROOT)/Support

# WAC Platform Source Include Paths
INCLUDES        += -I$(SRCROOT)/Platform

# WAC External Source Include Paths
INCLUDES        += -I$(SRCROOT)/External/GladmanAES
INCLUDES        += -I$(SRCROOT)/External/Curve25519
INCLUDES        += -I$(SRCROOT)/External/crypto

# mDNS Source Include Paths
INCLUDES        += -I$(MDNSROOT)/mDNSCore
INCLUDES        += -I$(MDNSROOT)/mDNSPosix
INCLUDES        += -I$(MDNSROOT)/mDNSShared

# WAC Core Objects
OBJS            += WACServer.o
OBJS            += WACBonjour.o

# WAC Support Objects
OBJS            += HTTPServer.o
OBJS            += HTTPUtils.o
OBJS            += StringUtils.o
OBJS            += TLVUtils.o
OBJS            += URLUtils.o
OBJS            += TimeUtils.o
OBJS            += SHAUtils.o
OBJS            += AESUtils.o
OBJS            += SecurityUtils.o
OBJS            += SocketUtils.o
OBJS            += AppleDeviceIE.o
OBJS            += MFiSAPServer.o

# WAC External Support Objects
# # Curve25519
OBJS            += curve25519-donna.o
# # GladmanAES
OBJS            += aes_modes.o
OBJS            += aescrypt.o
OBJS            += aeskey.o
OBJS            += aestab.o
OBJS            += gcm.o
OBJS            += gf128mul.o
# # crypto
OBJS            += aes.o
OBJS            += base64.o
OBJS            += bigint.o
OBJS            += hmac.o
OBJS            += md5.o
OBJS            += rc4.o
OBJS            += rsakey.o
OBJS            += rsapem.o
OBJS            += sha1.o
# Fix to place .o files in ODIR
_OBJS = $(patsubst %,$(ODIR)/%,$(OBJS))

# mDNS Libraries
LINKFLAGS       += -L$(MDNSROOT)/mDNSPosix/build/prod
LIBS            += -ldns_sd

# Flags
COMMONFLAGS     += -D_GNU_SOURCE
COMMONFLAGS     += -DDEBUG_EXPORT_ERROR_STRINGS=1

# debug=1 Flags
ifeq ($(debug))
COMMONFLAGS += -DDEBUG=1
COMMONFLAGS += -g
# debug=1 trace=1 Flags
# Trace will print function entries when debug=1
ifeq ($(trace))
COMMONFLAGS += -DTRACE=1
endif
endif

# time_platform=1 Flags
ifeq ($(time_platform),1)
COMMONFLAGS += -DTIME_PLATFORM=1
endif

COMMONFLAGS     += -DAES_UTILS_HAS_GLADMAN_GCM=1
COMMONFLAGS     += -DAES_UTILS_USE_GLADMAN_AES=1
COMMONFLAGS     += -DTARGET_HAS_MD5_UTILS=1
COMMONFLAGS     += -DTARGET_HAS_SHA_UTILS=1
COMMONFLAGS     += -DTARGET_NO_OPENSSL=1
COMMONFLAGS     += -DUSE_VIA_ACE_IF_PRESENT=0

# Warnings
COMMON_WARNINGS += -W
COMMON_WARNINGS += -Wall
#COMMON_WARNINGS += -Werror
COMMON_WARNINGS += -Wextra
COMMON_WARNINGS += -Wformat
COMMON_WARNINGS += -Wmissing-braces
COMMON_WARNINGS += -Wno-cast-align
COMMON_WARNINGS += -Wparentheses
COMMON_WARNINGS += -Wshadow
COMMON_WARNINGS += -Wsign-compare
COMMON_WARNINGS += -Wswitch
COMMON_WARNINGS += -Wuninitialized
COMMON_WARNINGS += -Wunknown-pragmas
COMMON_WARNINGS += -Wunused-function
COMMON_WARNINGS += -Wunused-label
COMMON_WARNINGS += -Wunused-parameter
COMMON_WARNINGS += -Wunused-value
COMMON_WARNINGS += -Wunused-variable

CWARNS          += $(COMMON_WARNINGS)
CWARNS          += -Wmissing-prototypes
CXXWARNS        += $(COMMON_WARNINGS)
CXXWARNS        += -Wnon-virtual-dtor
CXXWARNS        += -Woverloaded-virtual

CFLAGS          += $(INCLUDES) $(COMMONFLAGS) $(CWARNS)   -std=gnu99 -Os
CXXFLAGS        += $(INCLUDES) $(COMMONFLAGS) $(CXXWARNS) -Os

# Rules
.PHONY : all
all: setup WACServer

.PHONY : setup
setup:
# Make obj directory
	@mkdir -p $(ODIR)

$(ODIR)/%.o: %.c $(HEADERS)
# If verbose, print gcc execution, else hide
	@echo ">> Compiling $(notdir $<)"
ifeq ($(verbose),1)
	$(CC) $(CFLAGS) -c -o $@ $<
else
	@$(CC) $(CFLAGS) -c -o $@ $<
endif

WACServer: $(_OBJS)
	@echo ">> Linking $@..."
ifeq ($(verbose),1)
	$(CC) $(CFLAGS) $^ $(LINKFLAGS) $(LIBS) -o $@
else
	@$(CC) $(CFLAGS) $^ $(LINKFLAGS) $(LIBS) -o $@
endif
	
ifndef debug
	@echo ">> Stripping..."
ifeq ($(verbose),1)
	$(STRIP) $@
else
	@$(STRIP) $@
endif
endif
	@echo "-------------------------"
	@echo "BUILD COMPLETE: $@"
	@echo "-------------------------"

.PHONY : clean
clean:
	@-rm -rf $(ODIR) WACServer
	@echo "--------------"
	@echo "CLEAN COMPLETE"
	@echo "--------------"

