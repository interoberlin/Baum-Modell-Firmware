
#
# Makefile for nRF51 projects
#
# Author: Matthias Bock <mail@matthiasbock.net>
# License: GNU GPLv3
#
 
#
# Toolchain
#
TOOLCHAIN_PATH   = /usr/bin/
TOOLCHAIN_PREFIX = arm-none-eabi
AS      = $(TOOLCHAIN_PATH)$(TOOLCHAIN_PREFIX)-as
CC      = $(TOOLCHAIN_PATH)$(TOOLCHAIN_PREFIX)-gcc
LD      = $(TOOLCHAIN_PATH)$(TOOLCHAIN_PREFIX)-ld
OBJCOPY = $(TOOLCHAIN_PATH)$(TOOLCHAIN_PREFIX)-objcopy
OBJDUMP = $(TOOLCHAIN_PATH)$(TOOLCHAIN_PREFIX)-objdump
SIZE    = $(TOOLCHAIN_PATH)$(TOOLCHAIN_PREFIX)-size
GDB     = $(TOOLCHAIN_PATH)$(TOOLCHAIN_PREFIX)-gdb

#
# Compiler and Linker
#
CFLAGS += -DNRF51
CFLAGS += -DBLE_STACK_SUPPORT_REQD
CFLAGS += -std=gnu99 -Wall -Wextra -g
CFLAGS += -mcpu=cortex-m0 -mthumb -mabi=aapcs -mfloat-abi=soft
CFLAGS += -O3
CFLAGS += -ffreestanding

# keep every function in separate section. This will allow linker to dump unused functions
CFLAGS += -ffunction-sections -fdata-sections

# Do not use: -fno-strict-aliasing
# see Redmine-Ticket #1051
CFLAGS += -fstrict-aliasing

CFLAGS += -fno-builtin --short-enums
CFLAGS += -I nordic/arm/
CFLAGS += -I nordic/
CFLAGS += -I nordic/sdk/nrf51822/Include/
CFLAGS += -I nordic/sdk/nrf51822/Include/app_common/
CFLAGS += -I nordic/sdk/nrf51822/Include/ble/
CFLAGS += -I nordic/sdk/nrf51822/Include/ble/ble_services/
CFLAGS += -I nordic/sdk/nrf51822/Include/sd_common/
CFLAGS += -I nordic/sdk/nrf51822/Include/s110/
CFLAGS += -I sdk/

# TODO: auto-detect chip revision
CHIP_REVISION = aa

LINKER_SCRIPT_BLANK = sdk/linker/nrf51-blank-xx$(CHIP_REVISION).ld
LINKER_SCRIPT_SOFTDEVICE = sdk/linker/nrf51-s110-xx$(CHIP_REVISION).ld
LDFLAGS += -L /usr/lib/gcc/arm-none-eabi/4.8/armv6-m/
LDFLAGS += -L /usr/lib/arm-none-eabi/newlib/armv6-m/
LDFLAGS += -static
LDFLAGS += -nostartfiles -nostdlib
LDFLAGS += --start-group
LDFLAGS += -lgcc -lc -nosys
LDFLAGS += -Map tree.map

# required for semihosting 
CFLAGS  += --specs=rdimon.specs
LDFLAGS += -lrdimon

# filename of used softdevice (full path)
SOFTDEVICE = nordic/softdevice/s110_nrf51822_7.3.0_softdevice.hex
NORDIC_SDK_SOURCE = nordic/sdk/nrf51822/Source
SOFTDEVICE_DEPENDENCIES = $(NORDIC_SDK_SOURCE)/sd_common/softdevice_handler.o $(NORDIC_SDK_SOURCE)/ble/ble_advdata.o $(NORDIC_SDK_SOURCE)/ble/ble_conn_params.o $(NORDIC_SDK_SOURCE)/app_common/app_timer.o

#
# Build targets
#

all: tree.elf test_ble.elf

tree.elf: sdk/nrf51_startup.o nordic/nrf_delay.o led.o tree.o
	$(LD) $(LDFLAGS) -T $(LINKER_SCRIPT_BLANK) $^ -o $@

test_ble.elf: sdk/nrf51_startup.o $(SOFTDEVICE_DEPENDENCIES) bluetooth.o test_ble.o
	$(LD) $(LDFLAGS) -T $(LINKER_SCRIPT_SOFTDEVICE) $^ -o $@

%.o: %.c %.s
	$(CC) $(CFLAGS) -c $< -o $@

%.hex: %.elf
	$(OBJCOPY) -Oihex $< $@

%.bin: %.elf
	$(OBJCOPY) -Obinary $< $@

clean:
	rm -f *.o */*.o nordic/sdk/nrf51822/Source/*/*.o *.out *.bin *.elf *.hex *.map


#
# Flash the SoftDevice (Bluetooth Low Energy stack) HEX-file into the chip
#
define LOAD_SOFTDEVICE_COMMAND
target remote localhost:3333
monitor reset halt
load
endef
export LOAD_SOFTDEVICE_COMMAND

softdevice:
	echo "$$LOAD_SOFTDEVICE_COMMAND" | $(GDB) $(SOFTDEVICE)


#
# Debugger
#
OPENOCD_CFG_DIR = sdk/debug/
# workaround: problems because the folder is named like the target in the subfolder's Makefile
.PHONY: debug
include sdk/debug/Makefile
