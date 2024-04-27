
libs = $(wildcard ../include/*.h)
cflags = -g -I../include


all: parser

../obj/parser.o: ../src/parser.c $(libs)
	gcc $(cflags) -c $< -o $@
 
testeParser.o: testeParser.c 
	gcc $(cflags) -c $< -o $@

parser: $(../obj/parser.o) $(testeParser.o) $(libs)
	gcc $(cflags) ../obj/parser.o testeParser.o -o $@

clean: 
	rm parser ./*.o