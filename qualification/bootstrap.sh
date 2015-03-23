
EXT_SOURCES=~/Dropbox/ext-src


cd ext

mkdir ext
cd ext


tar -zxf $EXT_SOURCES/zlib-1.2.8.tar.gz


tar -zxf $EXT_SOURCES/boost_1_54_0.tar.gz
cd boost_1_54_0

chmod +x ./bootstrap.sh
./bootstrap.sh --with-toolset=clang
./b2 -j2 --with-iostreams --with-chrono --with-date_time --with-filesystem --with-log --with-program_options --with-signals --with-serialization --with-system --with-thread -sZLIB_SOURCE=`pwd`/../zlib-1.2.8

cd ..



cd ../..
