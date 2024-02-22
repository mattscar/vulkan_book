PROJ=queue_create

CC=gcc

CC_FLAGS=-DDEBUG -Wall -Wno-missing-braces -DVK_USE_PLATFORM_XCB_KHR 

INC_DIRS=/usr/include

LIBS=-lvulkan

$(PROJ): main.c device_utils.c error_utils.c
	$(CC) $(CC_FLAGS) -o $@ $^ $(INC_DIRS:%=-I%) $(LIBS)

clean:
	rm $(PROJ)