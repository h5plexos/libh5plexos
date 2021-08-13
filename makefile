CC=gcc -std=gnu11 -Wall
CFLAGS=-lzip -lexpat -lhdf5_serial -lhdf5_serial_hl
h5plexos: src/h5plexos_cli.o lib/h5plexos.o lib/plexostables.o lib/parsexml.o lib/makehdf5.o
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
