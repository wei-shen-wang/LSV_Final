#!/bin/bash

for file in good_benchmark/c*.v
do
    echo "Running $file"
    ./bin/obfuscate $file &> $file.log
done