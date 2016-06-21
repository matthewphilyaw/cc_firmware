###
# Project Settings
PROJECT=cc_firmware

###
# GNU ARM Embedded Toolchain
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

###
# Directory Structure
BINDIR=bin
INCDIR=include system/include system/include/cmsis system/include/cmsis/device
SRCDIR=src system/src

###
# Find source files
ASOURCES=$(shell find -L $(SRCDIR) -name '*.s')
CSOURCES=$(shell find -L $(SRCDIR) -name '*.c')
CXXSOURCES=$(shell find -L $(SRCDIR) -name '*.cpp')
INCLUDES=$(INCDIR:%=-I%)
# Find libraries
INCLUDES_LIBS=
LINK_LIBS=
# Create object list
OBJECTS=$(ASOURCES:%.s=%.o)
OBJECTS+=$(CSOURCES:%.c=%.o)
OBJECTS+=$(CXXSOURCES:%.cpp=%.o)
# Define output files ELF & IHEX
BINELF=$(PROJECT).elf
BINHEX=$(PROJECT).hex
BINARY=$(PROJECT).bin

###
# MCU FLAGS
MCFLAGS=-mcpu=cortex-m0 -mthumb
# COMPILE FLAGS
DEFS=-DSTM32F030x8
CFLAGS=-c $(MCFLAGS) $(DEFS) $(INCLUDES) -std=gnu11 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra -c -MMD -MP -MF$(@:.o=.d) -MT$(@)
CXXFLAGS=-c $(MCFLAGS) $(DEFS) $(INCLUDES) -std=c++11 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra # LINKER FLAGS
LDSCRIPT=-T ./ldscripts/mem.ld -T ./ldscripts/libs.ld -T ./ldscripts/sections.ld -nostartfiles -Wl,-Map=$(BINDIR)/output.map -Wl,-gc-sections #--specs=nano.specs 
LDFLAGS = $(LDSCRIPT) $(MCFLAGS) $(INCLUDES_LIBS) $(LINK_LIBS)

###
# Build Rules
.PHONY: all mkbin release release-memopt debug clean

all: mkbin release-memopt

mkbin:
	-mkdir bin > /dev/null 2>&1

release-memopt: CFLAGS+=-O2# -flto
release-memopt: CXXFLAGS+=-O2 # -flto
release-memopt: LDFLAGS+=-O2# -flto
release-memopt: release

debug: DEFS+=-DDEBUG
debug: CFLAGS+=-Og -g3
debug: CXXFLAGS+=-Og -g3
debug: LDFLAGS+=-g3
debug: release

release: $(BINDIR)/$(BINHEX)
release: $(BINDIR)/$(BINARY)

$(BINDIR)/$(BINARY): $(BINDIR)/$(BINELF)
	$(CP) -S -O binary $< $@
	@echo "Objcopy from ELF to bin complete!\n"

$(BINDIR)/$(BINHEX): $(BINDIR)/$(BINELF)
	$(CP) -O ihex $< $@
	@echo "Objcopy from ELF to IHEX complete!\n"

##
# C++ linking is used.
#
# Change
#   $(CXX) $(OBJECTS) $(LDFLAGS) -o $@ to 
#   $(CC) $(OBJECTS) $(LDFLAGS) -o $@ if
#   C linker is required.
$(BINDIR)/$(BINELF): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -lm -o $@
	@echo "Linking complete!\n"
	$(SIZE) $(BINDIR)/$(BINELF)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@
	@echo "Compiled "$<"!\n"

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@
	@echo "Compiled "$<"!\n"

%.o: %.s
	$(CC) $(CFLAGS) $< -o $@
	@echo "Assambled "$<"!\n"

clean:
	rm -f $(OBJECTS) $(OBJECTS:.o=.d) $(BINDIR)/$(BINELF) $(BINDIR)/$(BINHEX) $(BINDIR)/output.map $(BINDIR)/$(PROJECT).bin

deploy:
ifeq ($(wildcard /opt/openocd/bin/openocd),)
	openocd -f /usr/local/Cellar/open-ocd/0.9.0/share/openocd/scripts/board/st_nucleo_f0.cfg -c "program bin/"$(BINELF)" verify reset"
else
	openocd -f /usr/local/Cellar/open-ocd/0.9.0/share/openocd/scripts/board/st_nucleo_f0.cfg -c "program bin/"$(BINELF)" verify reset"
endif

