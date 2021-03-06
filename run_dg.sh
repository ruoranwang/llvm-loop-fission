#!/bin/bash

fpath=$1
filename=$(basename "$fpath")
file="${filename%.*}"

clang -c -emit-llvm $fpath -o $file.bc
opt -indvars -instcombine -loop-simplify -simplifycfg -mem2reg $file.bc > $file.simple.bc
#opt -mem2reg -simplifycfg -loop-simplify -instcombine -indvars $file.bc > $file.simple.bc
#opt -basicaa -da -load ../DG/Debug+Asserts/lib/DG.so -dg test.simple.bc >/dev/null
#opt -basicaa -da -load ../Release+Asserts/lib/splitpass.so -dg -rtrdg $file.simple.bc > /dev/null 
opt -load Release+Asserts/lib/splitpass.so -basicaa -da -dg < $file.simple.bc > /dev/null

llvm-dis $file.simple.bc
