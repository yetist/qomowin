GCCWARNINGS ?= -Waggregate-return -Wcast-align -Wdeclaration-after-statement -Werror-implicit-function-declaration -Wextra -Wno-sign-compare -Wno-unused-parameter -Winit-self -Wmissing-declarations -Wmissing-prototypes -Wnested-externs -Wpointer-arith -Wundef
CFLAGS += -O2 -Wall $(GCCWARNINGS) -pipe -mno-cygwin -mms-bitfields -g
CC := i486-mingw32-gcc
WINDRES := i486-mingw32-windres
DLL_LD_FLAGS += -Wl,--enable-auto-image-base
WINE := ../tools/wine

WINAPP := -mwindows
LDFLAGS := $(WINAPP)

TARGET := zh_CN
DLL_NAME := $(TARGET).dll

DLL_RC_SRC = zh_CN_resource.rc
DLL_C_SRC = $(TARGET).c

DLL_OBJECTS = $(DLL_RC_SRC:%.rc=%.o)

all: $(TARGET).dll

%.o: %.rc
	$(WINDRES) -i $< -o $@

$(TARGET).dll $(TARGET).dll.a: $(DLL_OBJECTS)
	$(CC) -shared $(DLL_C_SRC) $(DLL_OBJECTS) $(DLL_LD_FLAGS) -Wl,--output-def,$(TARGET).def,--out-implib,$(TARGET).dll.a -o $(TARGET).dll

clean:
	rm -f $(TARGET).dll $(TARGET).dll.a $(TARGET).def $(DLL_OBJECTS)

.PHONY: all clean test
