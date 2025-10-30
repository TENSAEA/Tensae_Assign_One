CC=gcc
CFLAGS=-O3 -march=native -fopenmp -DSTREAM_ARRAY_SIZE=$(ARRAY_SIZE) -DNTIMES=10

all: stream

stream: stream.c
	$(CC) $(CFLAGS) -o stream stream.c

clean:
	rm -f stream
