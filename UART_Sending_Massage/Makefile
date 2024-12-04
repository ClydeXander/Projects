PROJ_NAME = Sending_Message_via_UART

#Source files
SRCFL = $(PROJ_NAME).c
SRCFL += stm32f10x_rcc.c
SRCFL += stm32f10x_gpio.c
SRCFL += stm32f10x_usart.c

#Build Directory
BUILD_DIR = build
ELF_DIR = $(BUILD_DIR)/elf
HEX_DIR = $(BUILD_DIR)/hex
BIN_DIR = $(BUILD_DIR)/bin

#Build  Files
ELF_FILE = $(ELF_DIR)/$(PROJ_NAME).elf
HEX_FILE = $(HEX_DIR)/$(PROJ_NAME).hex
BIN_FILE = $(BIN_DIR)/$(PROJ_NAME).bin

#Put your STM32 library code directory here
STM_COMMON = /home/orhanveli/MCU_Related/Manufacturers/Stm32_Dev/STM32F10x_StdPeriph_Lib_V3.6.0/Libraries
STM_COMMON_CMSIS = /home/orhanveli/MCU_Related/Manufacturers/Stm32_Dev/STM32F10x_StdPeriph_Lib_V3.6.0/Libraries/CMSIS/CM3

#Complier settings. Only edit CFLAGS to include other header files.
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

#Complier flags.
CFLAGS = -mcpu=cortex-m3 -mthumb -T ~/MCU_Related/Test_STM32/STM32F103C8TX_FLASH.ld -mthumb-interwork -mfloat-abi=softfp
CFLAGS += --specs=nosys.specs -g3 -std=c99 -O2
CFLAGS += -I $(STM_COMMON)/../Project/STM32F10x_StdPeriph_Template
CFLAGS += -I $(STM_COMMON_CMSIS)/DeviceSupport/ST/STM32F10x
CFLAGS += -I $(STM_COMMON_CMSIS)/CoreSupport
CFLAGS += -I $(STM_COMMON)/STM32F10x_StdPeriph_Driver/inc
CFLAGS += -Wall -Wextra -Werror
CFLAGS += -D STM32F10X_MD -D USE_STDPERIPH_DRIVER
#Source File
SRCFL += $(STM_COMMON_CMSIS)/DeviceSupport/ST/STM32F10x/startup/TrueSTUDIO/startup_stm32f10x_md.s

#System File
SRCFL += $(STM_COMMON_CMSIS)/DeviceSupport/ST/STM32F10x/system_stm32f10x.c

vpath %.c $(STM_COMMON)/STM32F10x_StdPeriph_Driver/src \

.PHONY: proj

$(BUILD_DIR) $(ELF_DIR) $(HEX_DIR) $(BIN_DIR):
	@mkdir -p $@

all: proj

proj: $(ELF_FILE) $(HEX_FILE) $(BIN_FILE)

# Generate .elf file in ELF_DIR
$(ELF_FILE): $(SRCFL) | $(ELF_DIR)
	$(CC) $(CFLAGS) $^ -o $@

# Generate .hex file in HEX_DIR using patsubst
$(HEX_FILE): $(ELF_FILE) | $(HEX_DIR)
	$(OBJCOPY) -O ihex $< $@

# Generate .bin file in BIN_DIR using patsubst
$(BIN_FILE): $(ELF_FILE) | $(BIN_DIR)
	$(OBJCOPY) -O binary $< $@

clean:
	rm -rf $(BUILD_DIR)

flash:
	st-flash --format=binary write $(BIN_FILE) 0x8000000
