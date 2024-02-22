PROJ=rainbow_triangle

CC=gcc

CC_FLAGS=-DDEBUG -Wall -Wno-missing-braces -DVK_USE_PLATFORM_XCB_KHR 

INC_DIRS=/usr/include

LIBS=-lxcb -lvulkan

$(PROJ): main.c \
         device_utils.c \
         surface_utils.c \
         swapchain_utils.c \
         error_utils.c \
         command_utils.c \
         sync_utils.c \
         render_pass_utils.c \
         framebuffer_utils.c \
         pipeline_utils.c \
         resource_utils.c
	$(CC) $(CC_FLAGS) -o $@ $^ $(INC_DIRS:%=-I%) $(LIBS)

.PHONY: clean

clean:
	rm $(PROJ)
