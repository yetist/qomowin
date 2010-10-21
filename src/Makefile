GCCWARNINGS ?= -Waggregate-return -Wcast-align -Wdeclaration-after-statement -Werror-implicit-function-declaration -Wextra -Wno-sign-compare -Wno-unused-parameter -Winit-self -Wmissing-declarations -Wmissing-prototypes -Wnested-externs -Wpointer-arith -Wundef

CFLAGS += -O2 -Wall $(GCCWARNINGS) -pipe -mno-cygwin -mms-bitfields -g -D_DEBUG

CC := i486-mingw32-gcc
WINDRES := i486-mingw32-windres
WINE := ../tools/wine

WINAPP := -mwindows
LDFLAGS := $(WINAPP) -lshlwapi

EXE_TARGET := qomowin
EXE_NAME := $(EXE_TARGET).exe

EXE_RC_SRC = resource.rc
EXE_C_SRC = $(EXE_TARGET).c utils.c qw-debug.c

EXE_OBJECTS = $(EXE_C_SRC:%.c=%.o) $(EXE_RC_SRC:%.rc=%.o)

all: $(EXE_NAME) locale

$(EXE_NAME): $(EXE_OBJECTS)
	$(CC) $(EXE_OBJECTS) -o $(EXE_NAME) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

%.o: %.rc
	$(WINDRES) -i $< -o $@

locale:
	make -C locale

test: $(EXE_NAME)
	$(WINE) ./$(EXE_NAME)

clean:
	make -C locale clean
	rm -f $(EXE_NAME) $(EXE_OBJECTS)

.PHONY: all clean test locale
