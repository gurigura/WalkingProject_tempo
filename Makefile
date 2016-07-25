####################
# Default Target Parameter

PROGRAM		= mysample
TARGET_DIR	= target_dir

####################
# User Additional Definition for Compile and Link

#EXTRA_CFLAGS	= -Wall -Werror
EXTRA_CFLAGS	=
EXTRA_LFLAGS	=

####################
# Shell Definition

SHELL		= /bin/sh

####################
# Compile System Parameter

CC		= c++
RM		= /bin/rm -rf
MAKE		= make
LOCAL		= /usr/local
FK_LIB		= $(LOCAL)/lib
FK_INCLUDE	= $(LOCAL)/include

OPS		= -O -std=c++14

SYSOPS		= -D_LINUX_ -DHAVE_NANOSLEEP -DHAVE_TIME_H

INCLDIRS	= -I. -I$(FK_INCLUDE)

LFLAGS		= -L$(FK_LIB) -fPIC

SYSLIBS		= -lwiringPi -liconv -lopenal -lvorbisfile -lvorbis -logg \
			-lfltk_images -lfltk_forms -lfltk_gl -lfltk \
			-ljpeg -lpng -lfreetype -lGL -lpthread \
			-lXext -lX11 -lz -lm

CFLAGS		= $(EXTRA_CFLAGS) $(OPS) $(SYSOPS) $(INCLDIRS)
LINK		= $(EXTRA_LFLAGS) $(LFLAGS) $(FKLIB) $(SYSLIBS)
EXECOPT		= -o $(PROGRAM)
FKLIB		= \
			-lFK3_audio \
			-lFK3_util \
			-lFK3_fltkWin \
			-lFK3_graphics \
			-lFK3_scene \
			-lFK3_model \
			-lFK3_solid \
			-lFK3_shape \
			-lFK3_parser \
			-lFK3_shape \
			-lFK3_solid \
			-lFK3_material \
			-lFK3_math \
			-lFK3_base \
			-lFK3_fltkErr

####################
# File List Parser

HEADERS		= $(shell find . -name "*.h" -print)
CPPSRCS		= $(shell find . -name "*.cpp" -print)
CXXSRCS		= $(shell find . -name "*.cxx" -print)
CSRCS		= $(shell find . -name "*.c" -print)

OBJS		= $(CPPSRCS:cpp=o) $(CSRCS:c=o) $(CXXSRCS:cxx=o)

####################
# Target List

all : $(PROGRAM)

$(PROGRAM) : $(OBJS)
	$(CC) $(LFLAGS) $(EXECOPT) $(OBJS) $(LINK)

clean:
	$(RM) $(OBJS) $(PROGRAM) Makefile.bak \
	*.out core *.core *~ #*#

####################
# Suffixes Rules

.SUFFIXES : .cxx .o
.SUFFIXES : .cpp .o
.SUFFIXES : .c .o

%.o : %.cxx $(HEADERS)
	$(CC) -c $(CFLAGS) $< -o $@

%.o : %.cpp $(HEADERS)
	$(CC) -c $(CFLAGS) $< -o $@

%.o : %.c $(HEADERS)
	$(CC) -c $(CFLAGS) $< -o $@

####################
