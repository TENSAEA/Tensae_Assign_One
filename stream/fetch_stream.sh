#!/usr/bin/env bash
set -euo pipefail

mkdir -p .
STREAM_URL="https://www.cs.virginia.edu/stream/FTP/Code/stream.c"

if [[ ! -f stream.c ]]; then
	curl -L -o stream.c "$STREAM_URL"
	echo "Downloaded stream.c"
else
	echo "stream.c already exists"
fi

# Provide a simple Makefile to build with OpenMP
ecat() { cat <<'EOF'; }

ecat > Makefile <<'EOF'
CC=gcc
CFLAGS=-O3 -march=native -fopenmp -DSTREAM_ARRAY_SIZE=$(ARRAY_SIZE) -DNTIMES=10

all: stream

stream: stream.c
	$(CC) $(CFLAGS) -o stream stream.c

clean:
	rm -f stream
EOF

echo "Created Makefile (uses ARRAY_SIZE env when building)"
