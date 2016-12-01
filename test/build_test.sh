
# gtest
set -x
git clone https://github.com/yvxiang/gtest

work_dir=`pwd`
mkdir -p gtest_deps/lib
mkdir -p gtest_deps/include

cd gtest
./configure --prefix=$work_dir/gtest_deps --disable-shared --with-pic
make
cp -a lib/.libs/* $work_dir/gtest_deps/lib
cp -a include/gtest $work_dir/gtest_deps/include
cd -

rm -rf gtest

make
