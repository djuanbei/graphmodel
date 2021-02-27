#!/bin/bash
cd $1
make -C .. libd
allfiles=`find . -name *.cpp `
INC="-I /Users/yunyun/mycode/c++/graphmodel/ftma/include -I /Users/yunyun/mycode/c++/raptor/include    -std=c++11"
LIB="-L .. -lgmodel  -lgtest"

for ff in  $allfiles
do
    bname=${ff##*/}
    bbname=${bname%.cpp}
    echo $bbname

    g++ -std=c++11 -fdiagnostics-absolute-paths $ff $(echo $INC)  $(echo $LIB)   -o $bbname
    ./$bbname

done
