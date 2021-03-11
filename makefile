CC=gcc
CFLAGS=-lzip -lexpat
h5plexos: src/h5plexos_cli.o lib/h5plexos.o lib/plexostables.o lib/parsexml.o
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
