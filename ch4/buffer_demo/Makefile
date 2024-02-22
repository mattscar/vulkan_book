PROJ=buffer_demo

CC=gcc

CC_FLAGS=-DDEBUG

INC_DIRS=/usr/include

LIBS=-lvulkan

$(PROJ): main.c device_utils.c error_utils.c command_utils.c resource_utils.c
	$(CC) $(CC_FLAGS) -o $@ $^ $(INC_DIRS:%=-I%) $(LIBS)

.PHONY: clean

clean:
	rm $(PROJ)