#!/bin/bash

allfiles=$(find . -name *.h)
for f in $allfiles
do
    echo $f
    clang-format -i $f
done

allfiles=$(find . -name *.hpp)

for f in $allfiles
do
    echo $f
    clang-format -i $f
done


allfiles=$(find . -name *.cpp)

for f in $allfiles
do
    echo $f
    clang-format -i $f
done

allfiles=$(find . -name *.c)

for f in $allfiles
do
    echo $f
    clang-format -i $f
done
