CC = gcc
CFLAGS = -Wall -ansi -pedantic -g
EXEC = banksimulator
OBJ = main.o LinkedList.o Customer.o Queue.o Teller.o Parameters.o

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -pthread -o $(EXEC)

main.o: main.c linkedlist.h customer.h queue.h teller.h parameters.h
	$(CC) $(CFLAGS) -pthread -c main.c -c

Customer.o: Customer.c customer.h macros.h queue.h parameters.h
	$(CC) $(CFLAGS) -pthread -c Customer.c

Parameters.o: Parameters.c parameters.h queue.h
	$(CC) $(CFLAGS) -pthread -c Parameters.c
 
Queue.o: Queue.c queue.h linkedlist.h
	$(CC) $(CFLAGS) -pthread -c Queue.c

LinkedList.o: LinkedList.c linkedlist.h
	$(CC) $(CFLAGS) -c LinkedList.c

Teller.o: Teller.c teller.h queue.h customer.h parameters.h
	$(CC) $(CFLAGS) -pthread -c Teller.c 

clean:
	rm -f *.o
	rm $(EXEC)