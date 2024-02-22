PROJ=spinning_spheres

CPP=g++

CPP_FLAGS := -std=c++11 -DDEBUG -Wall -Wno-missing-braces -DVK_USE_PLATFORM_XCB_KHR
LD_FLAGS := 

SRCDIR:=.
SRCFILES:=$(shell find . -name "*.cpp")
OBJFILES:=$(patsubst %.cpp,%.o,$(SRCFILES))

INC_DIRS=/usr/include ../..
LIBS=-lxcb -lvulkan -lpthread

all: $(PROJ)

$(PROJ): $(OBJFILES)
	$(CPP) $(LDFLAGS) -o $@ $^ $(LIBS)
  
%.o: %.cpp
	$(CPP) $(CPP_FLAGS) -c -o $@ $^ $(INC_DIRS:%=-I%) 

.PHONY: clean

clean:
	rm $(PROJ) $(OBJFILES)