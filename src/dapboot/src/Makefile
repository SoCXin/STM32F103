-include local.mk

BINARY = dapboot
OPENCM3_DIR = ../libopencm3

TARGET ?= STM32F103
include targets.mk

SRCS := $(wildcard *.c)
SRCS += $(wildcard $(TARGET_COMMON_DIR)/*.c)
SRCS += $(wildcard $(TARGET_SPEC_DIR)/*.c)

OBJS += $(SRCS:.c=.o)
DEPS  = $(SRCS:.c=.d)

.DEFAULT_GOAL := $(BINARY).bin

clean::
	@rm -f $(OBJS)
	@rm -f $(DEPS)

include rules.mk

size: $(OBJS) $(BINARY).elf
	@$(PREFIX)size $(OBJS) $(BINARY).elf

debug: $(BINARY).elf
	-$(GDB) --tui --eval "target remote | $(OOCD) -f $(OOCD_INTERFACE) -f $(OOCD_BOARD) -f debug.cfg" $(BINARY).elf

erase:
	$(OOCD) -f $(OOCD_INTERFACE) -f $(OOCD_BOARD) \
            -c "init" -c "reset init" \
            -c "stm32f1x unlock 0; reset halt" \
            -c "flash erase_sector 0 0 last" -c "reset" -c "shutdown"

.PHONY += debug size erase

OBJS := $(sort $(OBJS))

# Add the base directory to the header search path
CPPFLAGS += -I.

# Add target config directory to the header search path
CPPFLAGS += -I$(TARGET_COMMON_DIR)/
CPPFLAGS += -I$(TARGET_SPEC_DIR)/
