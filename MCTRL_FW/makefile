AS = arm-none-eabi-as
CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld
OBJCOPY = arm-none-eabi-objcopy
GDB = gdb-multiarch
OOCD = openocd

PRO_DIR = .
BUILD_DIR = $(PRO_DIR)/build
CMSIS_DIR += $(PRO_DIR)/cmsis
HAL_DIR += $(PRO_DIR)/hal

TARGET = $(BUILD_DIR)/mctrl

OOCD_CFG = board/stm32f4discovery.cfg

CFLAGS += -I$(PRO_DIR)/include
CFLAGS += -I$(CMSIS_DIR)/include
CFLAGS += -I$(HAL_DIR)/include

CFLAGS += -DSTM32F407xx

CFLAGS += -mcpu=cortex-m4
CFLAGS += -mthumb
CFLAGS += -mfloat-abi=soft

CFLAGS += -g
CFLAGS += -O0
CFLAGS += -std=c99

CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -Wno-shift-count-overflow
CFLAGS += -Wno-tautological-compare

LDFLAGS += -nostdlib
LDFLAGS += -T stm32f407.ld

GDBFLAGS += --eval-command="target extended-remote localhost:3333"
GDBFLAGS += --eval-command="set listsize 30"
GDBFLAGS += --eval-command="set print pretty on"
GDBFLAGS += --eval-command="set print array on"

PRO_C_SRCS += $(wildcard $(PRO_DIR)/src/main.c)
PRO_C_SRCS += $(wildcard $(PRO_DIR)/src/system_stm32f4xx.c)
#PRO_C_SRCS += $(wildcard $(HAL_DIR)/src/stm32f4xx_ll_rcc.c)
#PRO_C_SRCS += $(wildcard $(HAL_DIR)/src/stm32f4xx_ll_gpio.c)

PRO_ASM_SRCS += $(wildcard $(PRO_DIR)/src/startup_stm32f407xx.s)

OBJS += $(patsubst $(PRO_DIR)/%.c, $(BUILD_DIR)/%.o, $(PRO_C_SRCS))
OBJS += $(patsubst $(PRO_DIR)/%.s, $(BUILD_DIR)/%.o, $(PRO_ASM_SRCS))

$(BUILD_DIR)/%.o: $(PRO_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(PRO_DIR)/%.s
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) -o $(TARGET).elf $(OBJS)
	$(OBJCOPY) -O binary $(TARGET).elf $(TARGET).bin

all: $(TARGET)

load:
	$(OOCD) -f $(OOCD_CFG) -c "program $(TARGET).elf verify reset exit"

clean:
	rm -rf $(BUILD_DIR)

server:
	$(OOCD) -f $(OOCD_CFG)

debug:
	$(GDB) $(GDBFLAGS) $(TARGET).elf