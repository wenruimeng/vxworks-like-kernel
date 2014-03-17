IMAGENAME = rain
ENDIAN    = EB
DEBUG     = 0

#HOST = CYGWIN
#HOST = VXWORKS
HOST = LINUX

ifeq ($(HOST),LINUX)
CC        = mipsisa32-elf-gcc
LD        = mipsisa32-elf-ld
OBJCOPY   = mipsisa32-elf-objcopy
OBJDUMP   = mipsisa32-elf-objdump
else
CC        = ccmips
LD        = ldmips
AR        = armips
OBJCOPY   = objcopymips
OBJDUMP   = objdumpmips
endif

ifeq ($(ENDIAN),EL)
OFORMAT = elf32-littlemips
else
OFORMAT = elf32-bigmips
endif

ROOT	= ./..
SRCDIR	= $(ROOT)/src/arch/private $(ROOT)/src/arch/mips/private $(ROOT)/src/rain $(ROOT)/src/utils $(ROOT)/src/os
BUILD	= build
BUILD_PATH = $(ROOT)/${BUILD}
INCDIR	= $(ROOT)/inc

VPATH	= $(SRCDIR)

KERNEL_LIB = ${BUILD_PATH}/kernel.a

INCLUDE   = -I$(INCDIR)/lib -I$(INCDIR)/arch/mips -I$(INCDIR)/arch/mips/private  -I$(INCDIR)/private \
            -I$(INCDIR)/board/gxemul -I$(INCDIR)/arch  -I$(INCDIR)/board -I$(INCDIR) 

W_OPTS    = -Wimplicit -Wformat
CC_OPTS   = -O2 -G0 -c -$(ENDIAN) $(INCLUDE) -D$(ENDIAN) -mips32 -mabi=32 -fno-builtin $(W_OPTS) -DKERNEL
CC_OPTS_A = $(CC_OPTS) -D_ASSEMBLER_

# earlier it was -oformat instead of --oformat
SRC_C  = $(foreach dir, $(SRCDIR), $(wildcard $(dir)/*.c))
SRC_S  = $(foreach dir, $(SRCDIR), $(wildcard $(dir)/*.S))
SRC    = $(SRC_C) $(SRC_S)

OBJ_C  = $(notdir $(patsubst %.c, %.o, $(SRC_C)))
OBJ_S  = $(notdir $(patsubst %.S, %.o, $(SRC_S)))
#OBJ_C  = $(subst src,${BUILD}, $(patsubst %.c, %.o, $(SRC_C)))
#OBJ_S  = $(subst src,${BUILD}, $(patsubst %.S, %.o, $(SRC_S)))
OBJ    = $(OBJ_C) $(OBJ_S)

# **********************************************
# Rules
# **********************************************

.PHONY : all
all : $(KERNEL_LIB)

$(KERNEL_LIB) : $(OBJ)
	rm -f $(KERNEL_LIB)
	$(AR) crs $(KERNEL_LIB) $(OBJ)

$(OBJ_C) : %.o : %.c 
	$(CC) $(CC_OPTS) -o $@ $<

$(OBJ_S) : %.o : %.S
	$(CC) $(CC_OPTS_A) -o $@ $<

$(OBJ) : ./Makefile

.PHONY : clean depend discclean

clean :
	rm -f *.o
	rm -f *.a
	
discclean :
	rm -f *.o
	rm -f *.map
	rm -f *.elf
	rm -f *.bin
	rm -f depend.kernel

depend :
	$(CC) $(INCLUDE) -M $(SRC) > depend.kernel

-include depend.kernel
