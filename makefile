CC = gcc
CFLAGS = -g

all: control.o write.o
	$(CC) $(CFLAGS) -o control control.o
	$(CC) $(CFLAGS) -o write write.o

control.o: control.c headers.h
	$(CC) -c control.c

write.o: write.c headers.h
	$(CC) -c write.c

clean:
	rm *.o control write