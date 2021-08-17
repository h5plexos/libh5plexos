CFLAGS=-static -std=gnu11 -Wall -Ideps/usr/include
CURL=curl -L
LIB_SRC_DIR=deps/src

h5plexos: src/h5plexos_cli.o lib/h5plexos.o lib/plexostables.o lib/parsexml.o lib/makehdf5.o
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

get-deps: get-musl get-libz get-libzip get-libexpat get-libhdf5

get-musl:
	mkdir -p $(LIB_SRC_DIR)
	$(CURL) https://musl.libc.org/releases/musl-1.2.2.tar.gz | tar -xz -C $(LIB_SRC_DIR)

get-libz:
	mkdir -p $(LIB_SRC_DIR)
	$(CURL) http://zlib.net/zlib-1.2.11.tar.gz | tar -xz -C $(LIB_SRC_DIR)

get-libzip:
	mkdir -p $(LIB_SRC_DIR)
	$(CURL) https://github.com/nih-at/libzip/releases/download/v1.8.0/libzip-1.8.0.tar.gz | tar -xz -C $(LIB_SRC_DIR)

get-libexpat:
	mkdir -p $(LIB_SRC_DIR)
	$(CURL) https://github.com/libexpat/libexpat/releases/download/R_2_4_1/expat-2.4.1.tar.gz | tar -xz -C $(LIB_SRC_DIR)

get-libhdf5:
	mkdir -p $(LIB_SRC_DIR)
	$(CURL) https://support.hdfgroup.org/ftp/HDF5/releases/hdf5-1.10/hdf5-1.10.7/src/hdf5-1.10.7.tar.gz | tar -xz -C $(LIB_SRC_DIR)
