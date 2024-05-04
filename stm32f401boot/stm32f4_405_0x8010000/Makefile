
FLASH       ?= st-flash
TOOLSET     ?= arm-none-eabi-
CC           = $(TOOLSET)gcc
LD           = $(TOOLSET)gcc
AR           = $(TOOLSET)gcc-ar
OBJCOPY      = $(TOOLSET)objcopy
OPTFLAGS    ?= -Og
STRIP=arm-none-eabi-strip


RM = rm -f
fixpath = $(strip $1)

CFLAGS      ?= -mcpu=cortex-m4  -mfloat-abi=hard -mfpu=fpv4-sp-d16 -ffreestanding -nostdlib -nostartfiles  -fno-builtin
LDFLAGS      = -ffreestanding -nostdlib -nostartfiles  -fno-builtin -I include  -I stm32lib -I usblib
INCLUDES     =   -I include -I gpio/Inc -I sd-spi/Inc -I fat32/Inc -I CC/Inc -I stm32lib -I shell/Inc -I usb/Inc -I usb/class -I kernel/Inc -I spi/Inc -I w25q/Inc
CFLAGS2     ?= $(CFLAGS) -mthumb $(OPTFLAGS)
LDSCRIPT     =  sections.ld

OBJDIR       = obj
SOURCES      =   $(wildcard system/*.c) $(wildcard system/*.S)
OBJECTS      = $(addprefix $(OBJDIR)/, $(addsuffix .o, $(notdir $(basename $(SOURCES)))))
SRCLIB         = $(wildcard library/*.c)
LIBOBJ         = $(addprefix $(OBJDIR)/, $(addsuffix .o, $(notdir $(basename $(SRCLIB)))))
SRCGPIO         = $(wildcard gpio/Src/*.c)
GPIOOBJ         = $(addprefix $(OBJDIR)/, $(addsuffix .o, $(notdir $(basename $(SRCGPIO)))))

SRCUSB         = $(wildcard usb/Src/*.c) $(wildcard usb/msc/*.c) $(wildcard usb/cdc/*.c)
USBOBJ         = $(addprefix $(OBJDIR)/, $(addsuffix .o, $(notdir $(basename $(SRCUSB)))))

SRCKERNEL         = $(wildcard kernel/Src/*.c)
kERNELOBJ         = $(addprefix $(OBJDIR)/, $(addsuffix .o, $(notdir $(basename $(SRCKERNEL)))))

SRCSPI         = $(wildcard spi/Src/*.c)
SPIOBJ         = $(addprefix $(OBJDIR)/, $(addsuffix .o, $(notdir $(basename $(SRCSPI)))))


SRCW25Q         = $(wildcard w25q/Src/*.c)
W25QOBJ         = $(addprefix $(OBJDIR)/, $(addsuffix .o, $(notdir $(basename $(SRCW25Q)))))

SRCSDSPI         = $(wildcard sd-spi/Src/*.c)
SDSPIOBJ         = $(addprefix $(OBJDIR)/, $(addsuffix .o, $(notdir $(basename $(SRCSDSPI)))))

SRCFAT32         = $(wildcard fat32/Src/*.c)
FAT32OBJ         = $(addprefix $(OBJDIR)/, $(addsuffix .o, $(notdir $(basename $(SRCFAT32)))))

SRCSHELL         = $(wildcard shell/Src/*.c)
SHELLOBJ         = $(addprefix $(OBJDIR)/, $(addsuffix .o, $(notdir $(basename $(SRCSHELL)))))

 

SRCCC         = $(wildcard cc/*.c)  
CCOBJ         = $(addprefix $(OBJDIR)/, $(addsuffix .o, $(notdir $(basename $(SRCCC)))))



DOUT         = kernel


SRCPATH      = $(sort $(dir $(SOURCES) $(SRCLIB) $(SRCGPIO) $(SRCUSB) $(SRCKERNEL) $(SRCSPI) $(SRCCC) $(SRCSHELL) $(SRCFAT32) $(SRCSDSPI) $(SRCW25Q) ))
vpath %.c $(SRCPATH)
vpath %.S $(SRCPATH)


 
$(OBJDIR):
	@mkdir $@

flash:
	#st-flash --reset --format ihex write kernel.hex
	st-flash write $(DOUT).bin 0x08000000
	st-flash reset
reset:
	st-flash reset


demo: $(DOUT).bin
		arm-none-eabi-objdump -d kernel.elf > kernel.list


	
$(DOUT).bin : $(DOUT).elf
	@echo building $@
	@$(OBJCOPY) -O binary $< $@



$(DOUT).elf : $(OBJDIR) $(LIBOBJ) $(SHELLOBJ) $(CCOBJ) $(GPIOOBJ) $(USBOBJ) $(kERNELOBJ) $(SPIOBJ) $(SDSPIOBJ) $(FAT32OBJ) $(W25QOBJ) $(OBJECTS) 
	@echo building $@
	@$(LD) $(CFLAGS2) $(LDFLAGS) -Wl,--script='$(LDSCRIPT)' $(LIBOBJ) $(GPIOOBJ) $(CCOBJ) $(SHELLOBJ) $(USBOBJ) $(FAT32OBJ) $(SDSPIOBJ) $(W25QOBJ) $(SPIOBJ) $(kERNELOBJ) $(OBJECTS) -o $@

clean: $(OBJDIR)
	$(MAKE) --version
	@$(RM) $(DOUT).*
	@$(RM) $(call fixpath, $(OBJDIR)/*.*)


$(OBJDIR)/%.o: %.S
	@echo assembling $<
	@$(CC) $(CFLAGS2)  $(INCLUDES) -c $< -o $@

$(OBJDIR)/%.o: %.c
	@echo compiling $<
	@$(CC) $(CFLAGS2)  $(INCLUDES) -c $< -o $@
