# libh5plexos

A C library (and accompanying command-line interface) for processing PLEXOS 8
solution zipfiles into the H5PLEXOS file format.

libh5plexos has the following dependencies:

 - [`libzip`](https://libzip.org) 1.x
 - [`libexpat`](https://libexpat.github.io) 2.2.x

## Command-line usage

From a shell prompt:

```shell
h5plexos inputfile.zip outputfile.h5
```

## C interface

In a C program (or a scripting language FFI):

```c
h5plexos(const char* infile, const char* outfile)
```
