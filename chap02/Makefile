# EMULATOR
TARGET_EMU = px86
OBJS_EMU = main.o
SOURCE_EMU = main.c
CC = gcc
CFLAGS_EMU += -Wall


# PROGRAM : .s -(nasm)-> binary
TARGET_ASM = helloworld
AS = nasm

.PHONY: all clean
all :
	make $(TARGET_EMU) $(TARGET_ASM)

$(OBJS_EMU) : $(SOURCE_EMU) Makefile
	$(CC) $(CFLAGS_EMU) -c $<

$(TARGET_EMU) : $(OBJS_EMU) Makefile
	$(CC) -o $@ $(OBJS_EMU)

$(TARGET_ASM) : *.s Makefile
	$(AS) -f bin -o $@ $<

clean :
	rm -f $(OBJS_EMU) $(TARGET_EMU) $(TARGET_ASM)
