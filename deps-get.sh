#! /usr/bin/env sh

DEPSDIR=deps/src
mkdir -p $DEPSDIR

curl -L http://zlib.net/zlib-1.2.11.tar.gz | tar -xz -C $DEPSDIR
curl -L https://github.com/nih-at/libzip/releases/download/v1.8.0/libzip-1.8.0.tar.gz | tar -xz -C $DEPSDIR
curl -L https://github.com/libexpat/libexpat/releases/download/R_2_4_1/expat-2.4.1.tar.gz | tar -xz -C $DEPSDIR
curl -L https://support.hdfgroup.org/ftp/HDF5/releases/hdf5-1.10/hdf5-1.10.7/src/hdf5-1.10.7.tar.gz | tar -xz -C $DEPSDIR
