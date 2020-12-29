PROJECT = HB
TARGET ?= $(PROJECT).elf
SRC_DIRS ?= ./src ./platform_src
LIBRARY_PATHS = ./STM32G4xx_HAL_Driver
#LIBRARIES = libdriver.a
LINKER_SCRIPT = linker/STM32G474RETx_FLASH.ld
COMPONENT = hrtim common usart protocol RCC adc crc dma

CC 		:= arm-none-eabi-gcc
AS 		:= arm-none-eabi-as
LD		:= arm-none-eabi-ld
OBJCOPY := arm-none-eabi-objcopy
OBJDUMP := arm-none-eabi-objdump

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
$(info $$SRCS is [$(SRCS)])
OBJS := $(addsuffix .o,$(basename $(SRCS)))
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find ./platform_inc -type d) $(shell find ./inc -type d) $(shell find ./STM32G4xx_HAL_Driver/inc -type d) $(foreach dir,$(COMPONENT),$(shell find ./Driver_card/$(dir)/ -type d))
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPU = -mcpu=cortex-m4 -mthumb
CFLAGS   ?= $(INC_FLAGS) -march=armv7e-m -c -fno-common -fmessage-length=0 -Wall -fno-exceptions -ffunction-sections -fdata-sections -g3 -DSTM32G474xx -O3 -fomit-frame-pointer
LDFLAGS  ?= -T $(LINKER_SCRIPT) -mcpu=cortex-m4 -mthumb -Wl,--gc-sections,-Map=$(PROJECT).map,--cref 
COMMON_FLAGS ?= --specs=nano.specs

all: libbuild $(PROJECT).bin

%.o: %.c
	$(CC) $(CFLAGS) $(CPU) -c $< -o $@
	
%.o: %.s
	$(AS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $(COMMON_FLAGS) $(CPU) $(OBJS) -L ./Driver_card -ldriver_card -L $(LIBRARY_PATHS) -ldriver -o $@ $(LOADLIBES) $(LDLIBS)

$(PROJECT).bin: $(TARGET)
	$(OBJCOPY) -O binary $< $@
	$(OBJDUMP) -D $(TARGET) > $(PROJECT).list

.PHONY: libbuild
libbuild:
	$(MAKE) -C ./STM32G4xx_HAL_Driver -j4
	$(MAKE) -C ./Driver_card -j4

.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS) $(DEPS) *.list *.map *.bin
	$(MAKE) -C ./Driver_card clean
	
#.PHONY: clean_elf
#clean_elf:
#	$(RM) $(TARGET)
	
.PHONY: flash
flash:
	openocd -f ../OpenOCD-20200729-0.10.0/share/openocd/scripts/board/st_nucleo_g4.cfg -c init -c "reset halt" -c "flash write_image erase HB.elf" -c "verify_image HB.elf" -c reset -c shutdown

.PHONY: cleanall
cleanall:
	$(RM) $(TARGET) $(OBJS) $(DEPS) *.list *.map *.bin
	$(MAKE) -C ./STM32G4xx_HAL_Driver clean
	$(MAKE) -C ./Driver_card clean
	
-include $(DEPS)
