PROJ=green_screen

CC=gcc

CC_FLAGS=-DDEBUG -DVK_USE_PLATFORM_XCB_KHR 

INC_DIRS=/usr/include

LIBS=-lxcb -lvulkan

$(PROJ): main.c device_utils.c surface_utils.c swapchain_utils.c error_utils.c command_utils.c sync_utils.c
	$(CC) $(CC_FLAGS) -o $@ $^ $(INC_DIRS:%=-I%) $(LIBS)

.PHONY: clean

clean:
	rm $(PROJ)