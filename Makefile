###########################################################################
#
# MODULE:             HomeKit - Makefile
#
# REVISION:           $Revision: 1.0 $
#
# DATED:              $Date: 2017-01-11 11:16:28 +0000 $
#
# AUTHOR:             PCT
#
###########################################################################
#
# Copyright panchangtao@gmail.com 2017. All rights reserved
#
###########################################################################

##############################################################################
# Library target name

TARGET := libhomekit

LIB_VERSION_MAJOR = 1
LIB_VERSION_MINOR = 0

LIB_HOMEKIT_NAME = $(TARGET).so.$(LIB_VERSION_MAJOR).$(LIB_VERSION_MINOR)
LIB_HOMEKIT_SONAME = $(TARGET).so.$(LIB_VERSION_MAJOR)

##############################################################################
# Path definitions

LIB_BASE_DIR = $(abspath ..)
LIB_INC = $(LIB_BASE_DIR)/include
LIB_ACCESSORY = $(LIB_BASE_DIR)/accessory
LIB_SERVICE = $(LIB_BASE_DIR)/service
LIB_IP = $(LIB_BASE_DIR)/ip
LIB_CHARACTERISTIC = $(LIB_BASE_DIR)/characteristic

##############################################################################
# Library object files

vpath % $(LIB_INC) $(LIB_ACCESSORY) $(LIB_SERVICE) $(LIB_IP) $(LIB_CHARACTERISTIC)
SOURCE := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst %.c,%.o,$(SOURCE))

##############################################################################
# Library header search paths

INCFLAGS += -I$(LIB_INC)

#########################################################################
# Dependency rules

PROJ_LIBS += -lpthread -ljson-c -lsqlite3 -lzmq
CC ?= gcc
RM ?= -rm
CFLAGS += -ggdb -Wno-unused-but-set-variable -Wall -O0

.PHONY: all clean install
all:$(TARGET).a $(TARGET).so

$(TARGET).a:$(OBJS)
	$(info Archiving $@ ...)
	$(AR) $(ARFLAGS) $@ $(OBJS)
	@echo

$(TARGET).so:$(OBJS)
	$(info Building shared library $@ ...)
	$(CC) -shared -Wl,-soname,$(LIB_HOMEKIT_SONAME) $(LDFLAGS) $(PROJ_LIBS) -o $@ $^
	ln -sf $(LIB_HOMEKIT_NAME) $(LIB_HOMEKIT_SONAME) 
	@echo

install:
	$(info install $@ ...)

clean:
	$(RM) -f $(LIB_HOMEKIT_NAME)*
