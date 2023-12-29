#!/bin/bash

for file in abc_script/c*_out.src
do
    echo "Running $file"
    ./abc/abc -f $file > $file.log
done

for file in abc_script/c*_orig.src
do
    echo "Running $file"
    ./abc/abc -f $file > $file.log
done
