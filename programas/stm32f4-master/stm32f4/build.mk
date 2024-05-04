
libstm32f4.a_SOURCES := $(addprefix stm32f4/src/,\
	eeprom.c \
	fault.c \
	gpio.c \
	i2c.c \
	interrupt.c \
	led.c \
	rcc.c \
	spi.c \
	timer.c \
	usart.c \
	usb_cdc.c)

libstm32f4.a_OBJECTS := $(patsubst stm32f4/src/%.c,stm32f4/build/%.o,$(libstm32f4.a_SOURCES))

$(eval $(call toolchain,stm32f4/build/libstm32f4.a))
stm32f4/build/libstm32f4.a: CFLAGS  += -Istm32f4/include \
                                       -Istm32f4/usb/include \
                                       -I$(FREERTOS_PREFIX)/FreeRTOS/Source/include \
                                       -I$(FREERTOS_PREFIX)/FreeRTOS/Source/portable/GCC/ARM_CM4F
stm32f4/build/libstm32f4.a: OBJECTS := $(libstm32f4.a_OBJECTS)
stm32f4/build/libstm32f4.a: stm32f4/usb/build/libstm32_usb.a
stm32f4/build/libstm32f4.a: FreeRTOS/build/libFreeRTOS.a
stm32f4/build/libstm32f4.a: $(libstm32f4.a_OBJECTS)
stm32f4/build/libstm32f4.a: | stm32f4/build
	$(call cmd,ar)

$(libstm32f4.a_OBJECTS): stm32f4/build/%.o: stm32f4/src/%.c
	$(call cmd,cc_o_c)

stm32f4/build:
	$(call cmd,mkdir)

clean::
	$(RM) -r stm32f4/build
