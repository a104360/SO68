CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude
LDFLAGS = -lm 
DEPS = $(wildcard include/*.h)
SRC = $(wildcard src/*.c)
OBJ = $(SRC:src/.c=obj/.o)


all: folders bin/client bin/orchestrator 

#orchestrator: bin/orchestrator
#client: bin/client

folders:
	@mkdir -p src include obj bin tmp

bin/orchestrator: obj/orchestrator.o obj/request.o obj/parser.o $(DEPS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

bin/client: obj/client.o obj/request.o obj/parser.o $(DEPS) #obj/client.o $(DEPS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

obj/%.o: src/%.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f obj/* tmp/* bin/*
