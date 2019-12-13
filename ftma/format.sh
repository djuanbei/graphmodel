#!/bin/bash

allfiles=$(find include -name *.h)
for f in $allfiles
do
    echo $f
    clang-format -i $f
done

allfiles=$(find src -name *.h)
for f in $allfiles
do
    echo $f
    clang-format -i $f
done

allfiles=$(find include -name *.hpp)

for f in $allfiles
do
    echo $f
    clang-format -i $f
done

allfiles=$(find src -name *.hpp)

for f in $allfiles
do
    echo $f
    clang-format -i $f
done

allfiles=$(find test -name *.cpp)

for f in $allfiles
do
    echo $f
    clang-format -i $f
done


allfiles=$(find src -name *.cpp)

for f in $allfiles
do
    echo $f
    clang-format -i $f
done

allfiles=$(find src -name *.c)

for f in $allfiles
do
    echo $f
    clang-format -i $f
done
