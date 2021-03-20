TARGET = px86
OBJS = main.o
CC = gcc
CFLAGS += -Wall

.PHONY: all clean
all :
	make $(TARGET)

*.o : *.c Makefile
	$(CC) $(CFLAGS) -c $<

$(TARGET) : $(OBJS) Makefile
	$(CC) -o $@ $(OBJS)

clean :
	rm -f $(OBJS) $(TARGET)