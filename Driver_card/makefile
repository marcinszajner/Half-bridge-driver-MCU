PROJECT = driver_card
TARGET ?= lib$(PROJECT).a
SRC_DIRS ?= ./src
COMPONENT = hrtim common usart dispatcher RCC adc crc dma msg
LIBRARY_PATHS = ./../STM32G4xx_HAL_Driver
LIBRARIES = 

CC 		:= arm-none-eabi-gcc
AS 		:= arm-none-eabi-as
LD		:= arm-none-eabi-ld
OBJCOPY := arm-none-eabi-objcopy
OBJDUMP := arm-none-eabi-objdump
AR      := arm-none-eabi-ar

SRCS := $(foreach dir,$(COMPONENT),$(shell find $(dir)/ -name *.cpp -or -name *.c -or -name *.s))
$(info $$SRCS is [$(SRCS)])

OBJS := $(addsuffix .o,$(basename $(SRCS)))
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(foreach dir,$(COMPONENT),$(shell find ./$(dir)/ -type d)) $(shell find ./../platform_inc -type d) $(shell find ./../STM32G4xx_HAL_Driver/inc -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))
$(info $$INC_FLAGS is [$(INC_FLAGS)])

CPU = -mcpu=cortex-m4 -mthumb
CFLAGS   ?= $(INC_FLAGS) -march=armv7e-m -fno-common -fmessage-length=0 -Wall -fno-exceptions -DSTM32G474xx -O3 -g3 -ffunction-sections -fdata-sections -fomit-frame-pointer -ffreestanding -nostdlib --specs=nosys.specs -nostartfiles

all: lib$(PROJECT).a

%.o: %.c
	$(CC) $(CFLAGS) $(CPU) -c $< -o $@
	
%.o: %.s
	$(CC) $(CFLAGS) $< -o $@

$(TARGET): $(OBJS)
	$(CC)  $(OBJS) -L $(LIBRARY_PATHS) -ldriver -o $@

lib$(PROJECT).a: $(TARGET)
	$(AR) rvs lib$(PROJECT).a $(OBJS)

.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS) $(DEPS) *.a

-include $(DEPS)
