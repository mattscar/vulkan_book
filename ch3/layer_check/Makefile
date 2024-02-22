PROJ=layer_check

CC=gcc

INC_DIRS=/usr/include

LIBS=-lvulkan

$(PROJ): $(PROJ).c
	$(CC) $(CC_FLAGS) -o $@ $^ $(INC_DIRS:%=-I%) $(LIBS)

clean:
	rm $(PROJ)