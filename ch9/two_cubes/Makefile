PROJ=two_cubes

CPP=g++

CPP_FLAGS=-std=c++11 -DDEBUG -Wall -DVK_USE_PLATFORM_XCB_KHR 

INC_DIRS=/usr/include ../..

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
         resource_utils.c \
         descriptor_utils.c
	$(CPP) $(CPP_FLAGS) -o $@ $^ $(INC_DIRS:%=-I%) $(LIBS)
  
#$(LIB_DIRS:%=-L%) $(LIBS)

.PHONY: clean

clean:
	rm $(PROJ)