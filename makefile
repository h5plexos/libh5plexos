CFLAGS=-static -std=gnu11 -Wall

h5plexos: src/h5plexos_cli.o lib/h5plexos.o lib/plexostables.o lib/parsexml.o lib/makehdf5.o
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
