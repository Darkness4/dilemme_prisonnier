#!/bin/sh
script_path=`dirname $0`

cd "$script_path/../"
mkdir -p build
cd build
if which cmake; then
    if which ninja; then
        cmake -GNinja .. || (echo "cmake ninja Failed" && exit 1)
        ninja || (echo "ninja Failed" && exit 1)
    elif which make; then
        cmake .. || (echo "cmake Failed" && exit 1)
        make || (echo "make Failed" && exit 1)
    else
        echo "Please install either Make or Ninja build system"
        exit 1
    fi
else
    echo "Please install cmake >= 3.0.0"
    exit 1
fi


