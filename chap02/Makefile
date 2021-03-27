TARGET_EMU = px86
TARGET_ASM = helloworld

OBJS_EMU = main.o
OBJS_ASM = helloworld.o

CC = gcc
CFLAGS += -Wall

LD = ld
AS = nasm

.PHONY: all clean
all :
	make $(TARGET)

*.o : *.c Makefile
	$(CC) $(CFLAGS) -c $<

$(TARGET) : $(OBJS) Makefile
	$(CC) -o $@ $(OBJS)


*.bin : *.asm Makefile
	$(AS) -o $@ $<

clean :
	rm -f $(OBJS) $(TARGET)