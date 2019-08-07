#!/bin/sh
script_path=$(dirname `realpath $0`)
echo $PWD
echo "Moving to $script_path"
cd "$script_path/../"
echo $PWD
mkdir -p build
echo "Moving to build"
cd build
echo $PWD
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


