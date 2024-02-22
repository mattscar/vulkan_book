PROJ=compute_dot

CPP=gcc

CPP_FLAGS=-DDEBUG -Wall -Wno-missing-braces -DVK_USE_PLATFORM_XCB_KHR  

INC_DIRS=/usr/include ../..

LIBS=-lxcb -lvulkan

$(PROJ): main.c \
         device_utils.c \
         error_utils.c \
         command_utils.c \
         pipeline_utils.c \
         resource_utils.c \
         descriptor_utils.c
	$(CPP) $(CPP_FLAGS) -o $@ $^ $(INC_DIRS:%=-I%) $(LIBS)

.PHONY: clean

clean:
	rm $(PROJ)