CC =gcc
CFLAGS =-std=gnu99 -Wall -pedantic -DLIST


#default program make
make: myShell.o
	$(CC) $(CFLAGS) -o myShell myShell.o

myShell.o: myShell.c
	$(CC) $(CFLAGS) -c myShell.c

clean:
	rm *.o myShell