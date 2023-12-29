#!/bin/bash

for file in benchmarks/*.v
do
    echo "Running $file"
    ./bin/obfuscate $file
done