CC=gcc
CFLAGS=-fopenmp -Wall -Wextra -O2

pi_seq: pi_seq.c
	$(CC) $(CFLAGS) -o pi_seq pi_seq.c

pi_par: pi_par.c
	$(CC) $(CFLAGS) -o pi_par pi_par.c

pi_par_critical: pi_par_critical.c
	$(CC) $(CFLAGS) -o pi_par_critical pi_par_critical.c

pi_par_loop: pi_par_loop.c
	$(CC) $(CFLAGS) -o pi_par_loop pi_par_loop.c
