#!/bin/bash

echo "Building Launcher"

executable="panui.exe"

mkdir -p obj
mkdir -p bin

source=(
 "launcher.cpp")

cflags="-std=c++14 -O0 -g -ggdb -isystem include -isystem .. -Wall -Wextra -pedantic"
linker="-lmingw32"

cmd="g++ $cflags"

if [ $OSTYPE == "msys" ]; then
    console="-mconsole"
fi

objects=""

for i in "${source[@]}"
do
    obj="`echo $i | sed 's-src/-obj/-g' | sed 's-.cpp-.o-g'`"
    deps=($(gcc -std=c++11 -MM $i | sed -e 's/^\w*.o://' | tr '\n' ' ' | sed -e 's/\\//g' | sed 's/ \+//' | sed 's/ \+/\n/g'))
    for j in "${deps[@]}"
    do
        if test $j -nt $obj; then
            echo "$cmd -c $i -o $obj"
            $cmd -c $i -o $obj
			break
        fi
    done
    objects="$objects $obj"
done
echo "g++ -o $executable $objects $linker"
g++ -m32 -o $executable $objects $linker

echo "Done building Launcher"

#./test.sh