PRG		= repeater
OBJ		= repeater.o

MCU_TARGET	= atmega8
F_CPU		= 8000000
OPTIMIZE	= -O2

DEFS		= -DF_CPU=$(F_CPU) -I../include
LIBS		=

CC		= avr-gcc

override CFLAGS	= -g -Wall $(OPTIMIZE) -mmcu=$(MCU_TARGET) $(DEFS)
override LDFLAGS= -Wl,-Map,$(PRG).map

OBJCOPY		= avr-objcopy
OBJDUMP		= avr-objdump

all: $(PRG).elf lst text eeprom

$(PRG).elf: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

morse.o: morse.c

repeater.o: repeater.c morse.o

clean:
	$(RM) *.o $(PRG).{bin,elf,hex,lst,map,srec}
	$(RM) $(PRG)_eeprom.{bin,elf,hex,srec}

lst: $(PRG).lst

%.lst: %.elf
	$(OBJDUMP) -h -S $< > $@

text: hex bin srec

hex: $(PRG).hex

%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

bin: $(PRG).bin

%.bin: %.elf
	$(OBJCOPY) -j .text -j .data -O binary $< $@

srec: $(PRG).srec

%.srec: %.elf
	$(OBJCOPY) -j .text -j .data -O srec $< $@

# Rules for building the .eeprom rom images

eeprom: ehex ebin esrec

ehex:  $(PRG)_eeprom.hex
ebin:  $(PRG)_eeprom.bin
esrec: $(PRG)_eeprom.srec

%_eeprom.hex: %.elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O ihex $< $@ \
	|| { echo empty $@ not generated; exit 0; }

%_eeprom.srec: %.elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O srec $< $@ \
	|| { echo empty $@ not generated; exit 0; }

%_eeprom.bin: %.elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O binary $< $@ \
	|| { echo empty $@ not generated; exit 0; }
