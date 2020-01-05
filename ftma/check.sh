#!/bin/bash
# bear make 
# export COMPILE_DB=$(/bin/pwd);
# grep file compile_commands.json |
# awk '{ print $2; }' |
# sed 's/\"//g' |
# while read FILE; do
#   (cd $(dirname ${FILE});
#    clang-check -analyze -p ${COMPILE_DB} $(basename ${FILE})
#   );
# done
export COMPILE_DB=$(/bin/pwd);
grep file compile_commands.json |
awk '{ print $2; }' |
sed 's/\"//g' |
while read FILE; do
    (cd $(dirname ${FILE});
     echo ${FILE}
   /Users/yunyun/mycode/c++/llvm-project/build/bin/clang-check -analyze -p ${COMPILE_DB} $(basename ${FILE})
  );
done
