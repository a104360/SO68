
libs = $(wildcard ../include/*.h)
cflags = -g -I../include


all: testll

../obj/parser.o: ../src/parser.c $(libs)
	gcc $(cflags) -c $< -o $@

../obj/request.o: ../src/request.c $(libs)
	gcc $(cflags) -c $< -o $@

../obj/linkedList.o: ../collections/linkedList.c $(libs)
	gcc $(cflags) -c $< -o $@
 
testeParser.o: testeParser.c
	gcc $(cflags) -c $< -o $@

parser: $(../obj/parser.o) $(testeParser.o) $(libs)
	gcc $(cflags) ../obj/parser.o testeParser.o -o $@

testRequest.o: testRequest.c $(libs) 
	gcc $(cflags) -c $< -o $@

testRequest: ../obj/parser.o ../obj/request.o  testRequest.o
	gcc $^ -o $@

testll.o: testLL.c $(libs)
	gcc $(cflags) -c $< -o $@

testll: ../obj/request.o ../obj/linkedList.o testll.o
	gcc $^ -o $@

clean: $(parser) $(testRequest) $(testll)
	rm parser testRequest testll *.o ../obj/*.o
