all: compila
compila: emissor.o receptor.o

emissor.o: emissor.cpp
	g++ emissor.cpp -o emissor

receptor.o: receptor.cpp
	g++ receptor.cpp -o receptor


clean:
	