
#
# Makefile for the Baumhaus Tree firmware
#
# Author: Matthias Bock <mail@matthiasbock.net>
# License: GNU GPLv3
#

PROJECT_ROOT = .

include Makefile.pre-targets

CFLAGS += -DNRF51
CFLAGS += -DDEBUG_NRF_USER
CFLAGS += -DBLE_STACK_SUPPORT_REQD
CFLAGS += -DS110

# disable for release version:
CFLAGS += -DBOARD_NRFDUINO

# TODO: auto-detect chip revision
CHIP_REVISION = aa

LINKER_SCRIPT_BLANK = sdk/linker/nrf51-blank-xx$(CHIP_REVISION).ld
LINKER_SCRIPT_SOFTDEVICE = sdk/linker/nrf51-s110-xx$(CHIP_REVISION).ld

# required for semihosting 
CFLAGS  += --specs=rdimon.specs
LDFLAGS += -lrdimon

# filename of used softdevice (full path)
SOFTDEVICE = nordic/softdevice/s110_nrf51822_7.3.0_softdevice.hex
NORDIC_SDK_SOURCE = nordic/sdk/nrf51822/Source
SOFTDEVICE_DEPENDENCIES = \
	$(NORDIC_SDK_SOURCE)/sd_common/softdevice_handler.o \
	$(NORDIC_SDK_SOURCE)/ble/ble_advdata.o \
	$(NORDIC_SDK_SOURCE)/ble/ble_conn_params.o \
	$(NORDIC_SDK_SOURCE)/app_common/app_timer.o

DEBUG_BINARY = tree.elf

#
# Build targets
#

all: main.elf test_ble.elf

main.elf: \
	sdk/nrf51_startup.o \
	nordic/nrf_delay.o \
	leds/sk6812.o \
	main.o
		$(LD) $(LDFLAGS) -T $(LINKER_SCRIPT_BLANK) $^ -o $@ -Map main.map

test_ble.elf: \
	sdk/nrf51_startup.o \
	$(SOFTDEVICE_DEPENDENCIES) \
	bluetooth/bluetooth.o \
	test_ble.o
		$(LD) $(LDFLAGS) -T $(LINKER_SCRIPT_SOFTDEVICE) $^ -o $@ -Map test_ble.map


include Makefile.post-targets
