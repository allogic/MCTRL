AS = arm-none-eabi-as
CC = arm-none-eabi-gcc
LD = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
GDB = gdb-multiarch
OOCD = openocd

PRO_DIR = .
BUILD_DIR = $(PRO_DIR)/build
CMSIS_DIR += $(PRO_DIR)/cmsis
HAL_DIR += $(PRO_DIR)/hal

TARGET = $(BUILD_DIR)/mctrl

OOCD_CFG = board/stm32f4discovery.cfg

MCUFLAGS += -mcpu=cortex-m4
MCUFLAGS += -march=armv7e-m
MCUFLAGS += -mthumb
MCUFLAGS += -mfloat-abi=soft
MCUFLAGS += -mfpu=fpv4-sp-d16

CFLAGS += -I$(PRO_DIR)/include
CFLAGS += -I$(CMSIS_DIR)/include
CFLAGS += -I$(HAL_DIR)/include
CFLAGS += -DSTM32F407xx
CFLAGS += -g
CFLAGS += -O0
CFLAGS += -std=c99
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -Wno-shift-count-overflow
CFLAGS += -Wno-tautological-compare

LDFLAGS += -nostartfiles
LDFLAGS += -Wl,-Map=$@.map
LDFLAGS += -T stm32f407.ld

GDBFLAGS += --eval-command="target extended-remote localhost:3333"
GDBFLAGS += --eval-command="set listsize 30"
GDBFLAGS += --eval-command="set print pretty on"
GDBFLAGS += --eval-command="set print array on"

C_SRCS += $(PRO_DIR)/src/main.c
C_SRCS += $(PRO_DIR)/src/terminal.c
C_SRCS += $(PRO_DIR)/src/system_stm32f4xx.c

ASM_SRCS += $(PRO_DIR)/src/startup_stm32f407xx.s

OBJS += $(patsubst $(PRO_DIR)/%.c, $(BUILD_DIR)/%.o, $(C_SRCS))
OBJS += $(patsubst $(PRO_DIR)/%.s, $(BUILD_DIR)/%.o, $(ASM_SRCS))

$(BUILD_DIR)/%.o: $(PRO_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(MCUFLAGS) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(PRO_DIR)/%.s
	@mkdir -p $(@D)
	$(CC) $(MCUFLAGS) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(LD) $(OBJS) $(MCUFLAGS) $(LDFLAGS) -o $(TARGET).elf
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