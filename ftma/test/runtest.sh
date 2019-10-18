#!/bin/bash
make -C .. libd
allfiles=`find . -name *.cpp `
INC="-I /Users/yunyun/mycode/c++/graphmodel/ftma/include -I /Users/yunyun/mycode/c++/raptor/include    -std=c++11"
LIB="-L .. -lftma  -lgtest"

for ff in  $allfiles
do
    bname=${ff##*/}
    bbname=${bname%.cpp}
    echo $bbname

    g++ -std=c++11 $ff $(echo $INC)  $(echo $LIB)   -o $bbname
    ./$bbname

done
