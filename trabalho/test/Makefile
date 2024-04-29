
libs = $(wildcard ../include/*.h)
cflags = -g -I../include


all: testRequest

../obj/parser.o: ../src/parser.c $(libs)
	gcc $(cflags) -c $< -o $@

../obj/request.o: ../src/request.c $(libs)
	gcc $(cflags) -c $< -o $@
 
testeParser.o: testeParser.c 
	gcc $(cflags) -c $< -o $@

parser: $(../obj/parser.o) $(testeParser.o) $(libs)
	gcc $(cflags) ../obj/parser.o testeParser.o -o $@

testRequest.o: testRequest.c
	gcc $(cflags) -c $< -o $@

testRequest: $(../obj/parser.o) $(../obj/request.o) $(libs)
	gcc $(cflags) ../obj/parser.o ../obj/request.o testRequest.o -o $@

clean: 
	rm parser testRequest
