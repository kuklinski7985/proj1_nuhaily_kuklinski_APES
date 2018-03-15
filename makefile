SOURCES = main.c i2c_wrapper.c tempsense.c temp_ops.c light_ops.c lightsense.c logger/logger.c logger/sync_fileio.c remote_socket_server.c ipc_messq.c #myusrled.c
OBJS = $(SOURCES:.c=.o)
IMP = $(SOURCES:.c=.i)
INCLUDES =
CC = arm-linux-gnueabihf-gcc
DEBUG = -pthread -lrt #-g -Wall -Werror -O0
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
	$(CC) $(DEBUG) $(OBJS) $(LDFLAGS) -o project1.elf -lrt
	size project1.elf $(OBJS)


.PHONY: clean
clean:
	-rm *.i *.o *.map *.d project1.elf a.out
	-rm logger/*.o logger/*.d project1.elf a.out
