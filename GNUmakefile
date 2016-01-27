﻿# Makefile for win32api_prosecc_name.exe

TARGET:= win32api_prosecc_name.exe
SRCS  := ./win32api_prosecc_name/Source.cpp ./win32api_prosecc_name/win_system_info_c.cpp

# 基本コマンド
RM 		:= rm
CXX 	:= g++
CC 		:= gcc
# ifeq ($(CXX),g++)
# ADDITIONALPOTIONS_default = -fexec-charset=CP932
# else
# ADDITIONALPOTIONS_default =
# endif
# ADDITIONALPOTIONS := $(ADDITIONALPOTIONS_default)
# デバッグ時とリリース時の微調整
CXX_DEBUG_FLAGS		=	-g -O0 -Wall
CXX_RELEASE_FLAGS	=	-O2

# 基本オプション
CPPFLAGS = -std=c++14 -fvisibility=hidden
ifeq ($(CXX),g++)
CPPFLAGS += --input-charset=utf-8
endif

# LBITS := $(shell getconf LONG_BIT)
# ifeq ($(LBITS),64)
#    CPPFLAGS+= -mrdrnd
# else
#    CPPFLAGS+= -mrdrnd -m32
# endif

# make
# debug
.PHONY: Debug
Debug: CXXFLAGS+=$(CXX_DEBUG_FLAGS)
Debug: all
# release
.PHONY: Release
Release: CXXFLAGS+=$(CXX_RELEASE_FLAGS)
Release: all

all: $(TARGET)
$(TARGET): $(SRCS)
	$(CXX) $^ -o $@ $(CXXFLAGS) $(CPPFLAGS) $(ADDITIONALPOTIONS)

# make clean
.PHONY: clean
clean:
	$(RM) -f *.exe