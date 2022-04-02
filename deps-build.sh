#! /usr/bin/env sh

DEPSDIR=$(pwd)/deps
INSTALLDIR=$DEPSDIR/usr
mkdir -p $INSTALLDIR

cd $DEPSDIR/src/zlib-1.2.12
./configure --prefix=$INSTALLDIR --static
make
make install

cd $DEPSDIR/src/libzip-1.8.0
mkdir -p build
cd build
cmake -DCMAKE_INSTALL_PREFIX=$INSTALLDIR -DZLIB_INCLUDE_DIR=$INSTALLDIR/include -DZLIB_LIBRARY=$INSTALLDIR/lib/libz.a -DENABLE_BZIP2=OFF -DENABLE_LZMA=OFF -DENABLE_ZSTD=OFF -DENABLE_COMMONCRYPTO=OFF -DENABLE_GNUTLS=OFF -DENABLE_MBEDTLS=OFF -DENABLE_OPENSSL=OFF -DENABLE_WINDOWS_CRYPTO=OFF -DBUILD_SHARED_LIBS=OFF -DBUILD_TOOLS=OFF -DBUILD_REGRESS=OFF -DBUILD_EXAMPLES=OFF -DBUILD_DOC=OFF ..
make
make install

cd $DEPSDIR/src/expat-2.4.1
./configure --prefix=$INSTALLDIR --enable-shared=no --without-xmlwf --without-examples --without-tests
make
make install

cd $DEPSDIR/src/hdf5-1.10.7
./configure --prefix=$INSTALLDIR --with-zlib=$INSTALLDIR --disable-shared --enable-tests=no --enable-tools=no
make
make install
