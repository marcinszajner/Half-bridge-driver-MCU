PROJECT = driver
TARGET ?= lib$(PROJECT).a
SRC_DIRS ?= ./src ./system_startup_src
LIBRARY_PATHS = 
LIBRARIES = 
LINKER_SCRIPT = ../linker/STM32G474RETx_FLASH.ld

CC 		:= arm-none-eabi-gcc
AS 		:= arm-none-eabi-as
LD		:= arm-none-eabi-ld
OBJCOPY := arm-none-eabi-objcopy
OBJDUMP := arm-none-eabi-objdump
AR      := arm-none-eabi-ar

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(addsuffix .o,$(basename $(SRCS)))
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find ./inc -type d) $(shell find ./../platform_inc -type d)
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
	$(CC)  $(OBJS) -o $@

lib$(PROJECT).a: $(TARGET)
	$(AR) rvs lib$(PROJECT).a $(OBJS)

.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS) $(DEPS) *.a

-include $(DEPS)