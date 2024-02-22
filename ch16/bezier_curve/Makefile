PROJ=bezier_curve

CPP=g++

CPP_FLAGS=-std=c++11 -DDEBUG -Wall -Wno-missing-braces -DVK_USE_PLATFORM_XCB_KHR

INC_DIRS=/usr/include ../..

LIBS=-lxcb -lvulkan

$(PROJ): main.cpp \
         DeviceUtils.cpp \
         SurfaceUtils.cpp \
         SwapchainUtils.cpp \
         ErrorUtils.cpp \
         CommandUtils.cpp \
         SyncUtils.cpp \
         RenderPassUtils.cpp \
         FramebufferUtils.cpp \
         PipelineUtils.cpp \
         ResourceUtils.cpp
	$(CPP) $(CPP_FLAGS) -o $@ $^ $(INC_DIRS:%=-I%) $(LIBS)

clean:
	rm $(PROJ)