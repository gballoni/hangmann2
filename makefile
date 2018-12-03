all: compila
compila: servidor.o cliente.o

servidor.o: servidor.cpp
	g++ servidor.cpp -o servidor -pthread -g

cliente.o: cliente.cpp
	g++ cliente.cpp -o cliente -pthread -g
clean:
	rm servidor cliente
	