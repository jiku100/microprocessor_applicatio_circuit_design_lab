TARGET=test
CC=gcc

$(TARGET) : test.o
	$(CC) -o $@ $^

test.o : test.c
	$(CC) -c -o $@ $^

clean :
	rm -f *.o test

