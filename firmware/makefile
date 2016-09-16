#######################################################################
#                            Project Setup                            #
#######################################################################

PROJECT=cc_firmware
OUTDIR=build
INCDIR=include system/include system/include/cmsis system/include/cmsis/device
SRCDIR=src system/src

BINELF=$(PROJECT).elf
BINHEX=$(PROJECT).hex
BINARY=$(PROJECT).bin

#######################################################################
#                                MISC                                 #
#######################################################################

MKDIR_P=mkdir -p

#######################################################################
#                         Arm Tool Chain                              #
#######################################################################

TOOL_ROOT=/usr/local/gcc-arm-none-eabi-5_2-2015q4/bin
CC=$(TOOL_ROOT)/arm-none-eabi-gcc
CXX=$(TOOL_ROOT)/arm-none-eabi-g++
LD=$(TOOL_ROOT)/arm-none-eabi-ld
AR=$(TOOL_ROOT)/arm-none-eabi-ar
AS=$(TOOL_ROOT)/arm-none-eabi-as
CP=$(TOOL_ROOT)/arm-none-eabi-objcopy
OD=$(TOOL_ROOT)/arm-none-eabi-objdump
NM=$(TOOL_ROOT)/arm-none-eabi-nm
SIZE=$(TOOL_ROOT)/arm-none-eabi-size
A2L=$(TOOL_ROOT)/arm-none-eabi-addr2line

#######################################################################
#                            Prep Files                               #
#######################################################################


# find files
INCLUDES=$(INCDIR:%=-I%)
ASOURCES=$(shell find -L $(SRCDIR) -name '*.s')
CSOURCES=$(shell find -L $(SRCDIR) -name '*.c')
CXXSOURCES=$(shell find -L $(SRCDIR) -name '*.cpp')

# create object file paths in the outdir
OBJECTS=$(ASOURCES:%.s=$(OUTDIR)/%.o)
OBJECTS+=$(CSOURCES:%.c=$(OUTDIR)/%.o)
OBJECTS+=$(CXXSOURCES:%.cpp=$(OUTDIR)/%.o)

# create out dirs to make
OUT_DIRS=$(subst /,/,$(sort $(dir $(OBJECTS))))

#######################################################################
#                        Compile/Linker Flags                         #
#######################################################################

DEFS=-DSTM32F030x8
MCFLAGS=-mcpu=cortex-m0 -mthumb
BASE_FLAGS=$(MCFLAGS) $(DEFS) $(INCLUDES)
BASE_FLAGS+=-fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra -gdwarf-2 -c -MMD -MP -MF$(@:.o=.d) -MT$(@)
CFLAGS= $(BASE_FLAGS) -std=gnu11
CXXFLAGS=$(BASE_FLAGS) -std=c++11 -pedantic-errors -fno-rtti -fno-threadsafe-statics
LDSCRIPT=-T ./ldscripts/mem.ld -T ./ldscripts/libs.ld -T ./ldscripts/sections.ld -nostartfiles -Wl,-Map=$(OUTDIR)/output.map -Wl,-gc-sections --specs=nano.specs 
LDFLAGS=$(LDSCRIPT) $(MCFLAGS) $(INCLUDES_LIBS) $(LINK_LIBS)

#######################################################################
#                             Build Rules                             #
#######################################################################

.PHONY: all mkbin release release-memopt debug debug-no-opt clean

#defulat to debug
all: dirs debug

dirs: $(OUT_DIRS)

$(OUT_DIRS):
	$(MKDIR_P) $(OUT_DIRS)

release-memopt: CFLAGS+=-O2
release-memopt: CXXFLAGS+=-O2
release-memopt: LDFLAGS+=-O2
release-memopt: release

debug: DEFS+=-DDEBUG
debug: CFLAGS+=-Og -g3
debug: CXXFLAGS+=-Og -g3
debug: LDFLAGS+=-g3
debug: release

debug-no-opt: DEFS+=-DDEBUG
debug-no-opt: CFLAGS+=-O0 -g3
debug-no-opt: CXXFLAGS+=-O0 -g3
debug-no-opt: LDFLAGS+=-g0
debug-no-opt: release

release: $(OUTDIR)/$(BINHEX)
release: $(OUTDIR)/$(BINARY)

$(OUTDIR)/$(BINARY): $(OUTDIR)/$(BINELF)
	$(CP) -S -O binary $< $@
	@echo "Objcopy from ELF to bin complete!\n"

$(OUTDIR)/$(BINHEX): $(OUTDIR)/$(BINELF)
	$(CP) -O ihex $< $@
	@echo "Objcopy from ELF to IHEX complete!\n"

$(OUTDIR)/$(BINELF): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -lm -o $@
	@echo "Linking complete!\n"
	$(SIZE) $(OUTDIR)/$(BINELF)

$(OUTDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@
	@echo "Compiled "$<"!\n"

$(OUTDIR)/%.o: %.c
	$(CC) $(CFLAGS) $< -o $@
	@echo "Compiled "$<"!\n"

$(OUTDIR)/%.o: %.s
	$(CC) $(CFLAGS) $< -o $@
	@echo "Assambled "$<"!\n"

clean:
	rm -f $(OBJECTS) $(OBJECTS:.o=.d) $(OUTDIR)/$(BINELF) $(OUTDIR)/$(BINHEX) $(OUTDIR)/output.map $(OUTDIR)/$(PROJECT).bin
