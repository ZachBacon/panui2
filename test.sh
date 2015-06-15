#!/bin/bash

echo "Building Panui"

executable="panui.exe"

mkdir -p obj
mkdir -p bin

source=(
 "src/panui2.cpp")

cflags="-std=c++14 -O0 -g -ggdb -isystem include $forceinclude -isystem .. -Wall -Wextra -pedantic -Wno-unused-parameter"
linker="obj/hiro.o  -lSDL2"
linker+=" -lkernel32 -luser32 -lgdi32 -ladvapi32 -lole32 -lcomctl32 -lcomdlg32 -luxtheme -lmsimg32 -lshlwapi"

cmd="g++ $cflags"

if [ $OSTYPE == "msys" ]; then
    linker+=" -mconsole -mwindows"
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
