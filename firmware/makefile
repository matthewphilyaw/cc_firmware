#######################################################################
#                            Project Setup                            #
#######################################################################

PROJECT=cc_firmware
OUTDIR=build
INCDIR=include system/include 
SRCDIR=src system/src

BINELF=$(PROJECT).elf
BINHEX=$(PROJECT).hex
BINARY=$(PROJECT).bin
LISTING=$(PROJECT).lst

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
ASOURCES=$(shell find -L $(SRCDIR) -name '*.S')
CSOURCES=$(shell find -L $(SRCDIR) -name '*.c')
CXXSOURCES=$(shell find -L $(SRCDIR) -name '*.cpp')

# create object file paths in the outdir
OBJECTS=$(CXXSOURCES:%.cpp=$(OUTDIR)/%.o)
OBJECTS+=$(ASOURCES:%.S=$(OUTDIR)/%.o)
OBJECTS+=$(CSOURCES:%.c=$(OUTDIR)/%.o)

# create out dirs to make
OUT_DIRS=$(subst /,/,$(sort $(dir $(OBJECTS))))

LINK_LIBS=./system/lib/libstm32plus-debug-f030-8000000i.a

#######################################################################
#                        Compile/Linker Flags                         #
#######################################################################

DEFS=-DSTM32PLUS_F0_30 -DHSI_VALUE=8000000
MCFLAGS=-mcpu=cortex-m0 -mthumb
BASE_FLAGS=$(MCFLAGS) $(DEFS) $(INCLUDES)
BASE_FLAGS+=-fno-exceptions -ffunction-sections -fdata-sections -Wall -gdwarf-2 -c
CFLAGS= $(BASE_FLAGS)
CXXFLAGS=$(BASE_FLAGS) -std=gnu++11 -fno-rtti -fno-threadsafe-statics -Wextra -I./system/include/stl -I./system/lib
LDSCRIPT=-T ./system/ldscripts/Linker.ld -gdwarf-2 -Wl,-Map=$(OUTDIR)/output.map -Wl,-gc-sections -Wl,-wrap,__aeabi_unwind_cpp_pr0 -Wl,-wrap,__aeabi_unwind_cpp_pr1 -Wl,-wrap,__aeabi_unwind_cpp_pr2 
LDFLAGS=-Xlinker $(LDSCRIPT) $(MCFLAGS) $(INCLUDES_LIBS)

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

release: $(OUTDIR)/$(LISTING)
release: $(OUTDIR)/$(BINHEX)
release: $(OUTDIR)/$(BINARY)

$(OUTDIR)/$(BINARY): $(OUTDIR)/$(BINELF)
	$(CP) -S -O binary $< $@
	@echo "Objcopy from ELF to bin complete!\n"

$(OUTDIR)/$(BINHEX): $(OUTDIR)/$(BINELF)
	$(CP) -O ihex $< $@
	@echo "Objcopy from ELF to IHEX complete!\n"

$(OUTDIR)/$(LISTING): $(OUTDIR)/$(BINELF)
	$(OD) -D -S $< > $(OUTDIR)/$(LISTING)
	@echo "Objdump from ELF to listing complete!\n"

$(OUTDIR)/$(BINELF): $(OBJECTS)
	$(CXX) -o $@ $(LDFLAGS) $(OBJECTS) $(LINK_LIBS)
	@echo "Linking complete!\n"
	$(SIZE) $(OUTDIR)/$(BINELF)

$(OUTDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@
	@echo "Compiled "$<"!\n"

$(OUTDIR)/%.o: %.c
	$(CC) $(CFLAGS) $< -o $@
	@echo "Compiled "$<"!\n"

$(OUTDIR)/%.o: %.S
	$(CC) $(CFLAGS) $< -o $@
	@echo "Assambled "$<"!\n"

clean:
	rm -f $(OBJECTS) $(OBJECTS:.o=.d) $(OUTDIR)/$(BINELF) $(OUTDIR)/$(BINHEX) $(OUTDIR)/output.map $(OUTDIR)/$(PROJECT).bin
