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
OBJCOPY   = objcopymips
OBJDUMP   = objdumpmips
endif

ifeq ($(ENDIAN),EL)
OFORMAT = elf32-littlemips
else
OFORMAT = elf32-bigmips
endif

ROOT	= ./..
SRCDIR	= $(ROOT)/src/arch $(ROOT)/src/arch/mips $(ROOT)/src/board/gxemul \
			$(ROOT)/src/lib $(ROOT)/src/usrapp/app1 $(ROOT)/src/test 
BUILD	= build
BUILD_PATH = $(ROOT)/${BUILD}
INCDIR	= $(ROOT)/inc
KERNEL_LIB = $(BUILD_PATH)/kernel.a
VPATH	= $(SRCDIR)

IMAGE_ELF = ${BUILD_PATH}/$(IMAGENAME).elf
IMAGE_BIN = ${BUILD_PATH}/$(IMAGENAME).bin
IMAGE_SREC = ${BUILD_PATH}/$(IMAGENAME).srec
MAP       = ${BUILD_PATH}/$(IMAGENAME).map


INCLUDE   = -I$(INCDIR)/lib -I$(INCDIR)/arch/mips -I$(INCDIR)/board/gxemul -I$(INCDIR)/arch  -I$(INCDIR)/board -I$(INCDIR) 
# INCLUDE   = -I$(INCDIR)/lib -I$(INCDIR)/arch/mips -I$(INCDIR)/arch/mips/private  -I$(INCDIR)/private \
            -I$(INCDIR)/board/gxemul -I$(INCDIR)/arch  -I$(INCDIR)/board -I$(INCDIR) 

W_OPTS    = -Wimplicit -Wformat
CC_OPTS   = -O2 -c -G0 -$(ENDIAN) $(INCLUDE) -D$(ENDIAN) -mips32 -mabi=32 -fno-builtin $(W_OPTS)
CC_OPTS_A = $(CC_OPTS) -D_ASSEMBLER_

LD_SCRIPT = $(ROOT)/build/link.xn

# earlier it was -oformat instead of --oformat
LD_OPTS   = -g -T $(LD_SCRIPT) -o $(IMAGE_ELF) -Map $(MAP) --oformat $(OFORMAT)
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
all : $(IMAGE_BIN)

$(IMAGE_BIN) : $(IMAGE_ELF)
	$(OBJCOPY) -O binary $(IMAGE_ELF) $(IMAGE_BIN)
	$(OBJCOPY) -O srec $(IMAGE_ELF) $(IMAGE_SREC)

$(IMAGE_ELF) : $(OBJ)
	$(LD) $(LD_OPTS) $(OBJ) $(KERNEL_LIB)
	$(OBJDUMP) -D $(IMAGE_ELF)> z.txt

$(OBJ_C) : %.o : %.c 
	$(CC) $(CC_OPTS) -o $@ $<

$(OBJ_S) : %.o : %.S
	$(CC) $(CC_OPTS_A) -o $@ $<

$(IMAGE_ELF) : $(LD_SCRIPT)
$(OBJ) : ./Makefile

.PHONY : clean discclean depend

clean :
	rm -f *.o
	rm -f *.map
	rm -f *.elf
	rm -f *.bin
	rm -f *.srec
	
discclean :
	rm -f *.o
	rm -f *.map
	rm -f *.elf
	rm -f *.bin
	rm -f depend.rain

depend :
	$(CC) $(INCLUDE) -M $(SRC) > depend.rain

-include depend.rain
