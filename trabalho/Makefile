CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude
LDFLAGS = -lm 
DEPS = $(wildcard include/*.h)
SRC = $(wildcard src/*.c)
SRC_SERVER = $(filter-out src/client.c,$(SRC))
SRC_CLIENT = $(filter-out src/orchestrator.c,$(SRC))
OBJ = $(SRC:src/.c=obj/.o)
OBJ_SERVER = $(SRC_SERVER:src/.c=obj/.o)
OBJ_CLIENT = $(SRC_CLIENT:src/.c=obj/.o)

all: folders bin/client bin/orchestrator 

#orchestrator: bin/orchestrator
#client: bin/client

folders:
	@mkdir -p src include obj bin tmp

bin/orchestrator: $(OBJ_SERVER) #$(DEPS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

bin/client: $(OBJ_CLIENT) #$(DEPS) #obj/client.o $(DEPS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

obj/%.o: src/%.c $(DEPS)
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm -f obj/* tmp/* bin/*
