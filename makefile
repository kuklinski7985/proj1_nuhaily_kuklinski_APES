SOURCES = maintemp.c i2c_wrapper.c tempsense.c
OBJS = $(SOURCES:.c=.o)
IMP = $(SOURCES:.c=.i)
INCLUDES = 
CC = arm-linux-gnueabihf-gcc
DEBUG = -g -Wall -Werror -O0
CPPFLAGS = 
LDFLAGS = -lm -Wl,-Map,project1.map
CFLAGS = -c
LFLAGS = -S

%.o:%.c
	$(CC) $(DEBUG) $(CPPFLAGS) $(CFLAGS) -MMD $^ -o $@


%.i:%.c
	$(CC) $(DEBUG) -E $(CPPFLAGS) $^ -o $@

%.asm:%.c

	$(CC) $(DEBUG) $(CPPFLAGS) $(CFLAGS) -S $< -o $@


.PHONY: compile-all
compile-all: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS)  -o $@

.PHONY: build
build: $(OBJS)
	$(CC) $(DEBUG) $(OBJS) $(PLATFORM_FLAGS) $(LDFLAGS) -o project1.elf
	size project1.elf $(OBJS)


.PHONY: clean
clean:
	-rm *.i *.o *.map *.d project1.elf a.out
