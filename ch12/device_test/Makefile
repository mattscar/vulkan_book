PROJ=device_test

CPP=g++

CPP_FLAGS=-std=c++11 -DDEBUG -Wall -Wno-missing-braces

INC_DIRS=/usr/include ../..

LIBS=-lvulkan

$(PROJ): $(PROJ).cpp
	$(CPP) $(CPP_FLAGS) -o $@ $^ $(INC_DIRS:%=-I%) $(LIBS)

clean:
	rm $(PROJ)