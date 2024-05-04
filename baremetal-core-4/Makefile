
FLASH       ?= st-flash
TOOLSET     ?= arm-none-eabi-
CC           = $(TOOLSET)gcc
LD           = $(TOOLSET)gcc
AR           = $(TOOLSET)gcc-ar
OBJCOPY      = $(TOOLSET)objcopy
OPTFLAGS    ?= -Og

ifeq ($(OS),Windows_NT)
	RM = del /Q
	fixpath = $(strip $(subst /,\, $1))
else
	RM = rm -f
	fixpath = $(strip $1)
endif

CFLAGS      ?= -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -ffreestanding -nostdlib -nostartfiles  -fno-builtin
LDFLAGS      = -ffreestanding -nostdlib -nostartfiles  -fno-builtin -I include  -I stm32lib -I usblib
INCLUDES     =  -I include  -I stm32lib -I usblib
CFLAGS2     ?= $(CFLAGS) -mthumb $(OPTFLAGS)
LDSCRIPT     =  sections.ld

OBJDIR       = obj
SOURCES      = $(wildcard system/*.c) $(wildcard system/*.S)
OBJECTS      = $(addprefix $(OBJDIR)/, $(addsuffix .o, $(notdir $(basename $(SOURCES)))))
DSRC         = $(wildcard library/*.c)
DOBJ         = $(addprefix $(OBJDIR)/, $(addsuffix .o, $(notdir $(basename $(DSRC)))))
DSRC2         = $(wildcard littlefs/*.c)
DOBJ2         = $(addprefix $(OBJDIR)/, $(addsuffix .o, $(notdir $(basename $(DSRC2)))))
DOUT         = kernel


SRCPATH      = $(sort $(dir $(SOURCES) $(DSRC) $(DSRC2)))
vpath %.c $(SRCPATH)
vpath %.S $(SRCPATH)
vpath %.s $(SRCPATH)

 
$(OBJDIR):
	@mkdir $@

flash:
	st-flash write $(DOUT).bin 0x08000000
	st-flash reset


demo: $(DOUT).bin
		arm-none-eabi-objdump -d kernel.elf > kernel.list

$(DOUT).bin : $(DOUT).elf
	@echo building $@
	@$(OBJCOPY) -O binary $< $@


$(DOUT).elf : $(OBJDIR) $(DOBJ) $(DOBJ2) $(OBJECTS)
	@echo building $@
	@$(LD) $(CFLAGS2) $(LDFLAGS) -Wl,--script='$(LDSCRIPT)' $(DOBJ) $(DOBJ2) $(OBJECTS) -o $@

clean: $(OBJDIR)
	$(MAKE) --version
	@$(RM) $(DOUT).*
	@$(RM) $(call fixpath, $(OBJDIR)/*.*)


$(OBJDIR)/%.o: %.c
	@echo compiling $<
	@$(CC) $(CFLAGS2)  $(INCLUDES) -c $< -o $@

$(OBJDIR)/%.o: %.S
	@echo assembling $<
	@$(CC) $(CFLAGS2)  $(INCLUDES) -c $< -o $@

$(OBJDIR)/%.o: %.s
	@echo assembling $<
	@$(CC) $(CFLAGS2)  $(INCLUDES) -c $< -o $@

