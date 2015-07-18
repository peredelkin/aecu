OBJECTS=main.o ports.o graphics.o font.o lcd0108.o

MCU_NAME=mega
MCU_NUMBER=64
F_CPU=24576000UL

MMCU=at$(MCU_NAME)$(MCU_NUMBER)
DEV_DEF=__AVR_AT$(MCU_NAME)$(MCU_NUMBER)__

MCUFLAGS=-mmcu=$(MMCU)

CFLAGS=-O3 $(MCUFLAGS) -D$(DEV_DEF) -DF_CPU=$(F_CPU) -ffunction-sections -fdata-sections

LDFLAGS=-Wl,-Map=$(TARGET).map,--cref $(MCUFLAGS) -Wl,--gc-sections

LIBSPATH=

INCS=

TARGET=main
TARGET_HEX=$(TARGET).hex
TARGET_EPP_HEX=$(TARGET).epp
TARGET_EPP_BIN=$(TARGET).epp.bin

CC=avr-gcc
LD=avr-gcc
OBJCOPY=avr-objcopy
RM=rm -f
STRIP=avr-strip
SIZE=avr-size



all: $(OBJECTS)
	$(LD) -o $(TARGET) $(LDFLAGS) $^ $(LIBSPATH) $(LIBS)
	$(STRIP) $(TARGET)
	$(SIZE) -A $(TARGET)
	$(OBJCOPY) -O ihex -R .eeprom $(TARGET) $(TARGET_HEX)
	$(OBJCOPY) -O ihex -j .eeprom --change-section-lma .eeprom=0 $(TARGET) $(TARGET_EPP_HEX)
	$(OBJCOPY) -O binary -j .eeprom --change-section-lma .eeprom=0 $(TARGET) $(TARGET_EPP_BIN)

clean:
	$(RM) $(OBJECTS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS) $(INCS)
	
xmem.o: ./libs/xmem.c
	$(CC) -c -o $@ $< $(CFLAGS) $(INCS)
ports.o: ./libs/ports.c
	$(CC) -c -o $@ $< $(CFLAGS) $(INCS)
pin_state.o: ./libs/pin_state.c
	$(CC) -c -o $@ $< $(CFLAGS) $(INCS)
adc.o: ./libs/adc.c
	$(CC) -c -o $@ $< $(CFLAGS) $(INCS)
usart.o: ./libs/usart.c
	$(CC) -c -o $@ $< $(CFLAGS) $(INCS)
graphics.o: ./libs/graphics.c
	$(CC) -c -o $@ $< $(CFLAGS) $(INCS)
font.o: ./libs/font.c
	$(CC) -c -o $@ $< $(CFLAGS) $(INCS)
lcd0108.o: ./libs/lcd0108.c
	$(CC) -c -o $@ $< $(CFLAGS) $(INCS)