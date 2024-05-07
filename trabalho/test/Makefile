
libs = $(wildcard ../include/*.h)
cflags = -g -I../include


all: testList

../obj/parser.o: ../src/parser.c $(libs)
	gcc $(cflags) -c $< -o $@

../obj/linkedList.o: ../src/linkedList.c $(libs)
	gcc $(cflags) -c $< -o $@

../obj/request.o: ../src/request.c ../obj/linkedList.o $(libs)
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

testList.o: testList.c ../obj/request.o ../obj/parser.o ../obj/linkedList.o $(libs)
	gcc $(cflags) -c $< -o $@

testList: ../obj/request.o ../obj/linkedList.o ../obj/parser.o testList.o
	gcc $^ -o $@

clean:
	rm -f parser testRequest testll testList *.o ../obj/*.o
