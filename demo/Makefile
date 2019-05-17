Author = qitas

######################################
# target
######################################
TARGET = target


######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization
OPT = -Og


#######################################
# paths
#######################################
# source path

# firmware library path
PERIFLIB_PATH = 

# Build path
BUILD_DIR = build

######################################
# source
######################################
# C sources
C_SOURCES =  $(shell find ./ -name '*.c')

ASM_SOURCES = LIBS/startup_stm32f103xb.s


######################################
# firmware library
######################################
PERIFLIB_SOURCES = 


#######################################
# binaries
#######################################
PREFIX=arm-none-eabi

CC=$(PREFIX)-gcc
LD=$(PREFIX)-gcc
AS=$(PREFIX)-as
CP=$(PREFIX)-objcopy
OD=$(PREFIX)-objdump

OBJCOPYFLAGS = -O binary
BIN=$(CP) -O ihex 
#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m3

# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS =  \
-DSTM32F103X_MD \
-DUSE_STDPERIPH_DRIVER\

# AS includes
AS_INCLUDES = 

# C includes
C_INCLUDES = -I CORE                  \
           -I LIBS/inc    \
           -I BSP    \
           -I SYSTEM        \
           -I USER	\


# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif


# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)"


#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = LIBS/STM32F103R8Tx_FLASH.ld

# libraries
LIBS = -lc -lm -lnosys 
LIBDIR = 
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir $@		

#######################################
# clean up
#######################################
clean:
	-rm -fR .dep $(BUILD_DIR)

flash:
	st-flash write $(BUILD_DIR)/$(TARGET).bin 0x8000000

erase:
	st-flash erase
  
#######################################
# dependencies
#######################################
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)

# *** EOF ***
