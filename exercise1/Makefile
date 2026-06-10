CC=gcc
CFLAGS=-fopenmp -O2 -Wall -Wextra -lpthread

pi: pi.c
	$(CC) $(CFLAGS) -o pi pi.c

pi_threads: pi_threads.c
	$(CC) $(CFLAGS) -o pi_threads pi_threads.c

run-pi:
	./pi 

