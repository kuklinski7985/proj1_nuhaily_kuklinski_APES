SOURCES = maintemp.c i2c_wrapper.c

OBJS = $(SOURCES:.c=.o)
IMP = $(SOURCES:.c=.i)

DEBUG = -g -Wall
CFLAGS = -pthread -lrt
CC = arm-linux-gnueabihf-gcc

%.o : %.c
	$(CC) $(CFLAGS) $< -o $@

.PHONY: build
build: $(OBJS)
	#$(CC) $(DEBUG) $(OBJS) -o $@
	$(CC) -O $@ $^ $(DEBUG) 

.PHONY: clean
clean:
	-rm *.i *.o *.map *.d *.elf unittest *.out
