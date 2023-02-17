#!/bin/bash

file=$1
if [ ! -f "$file" ]; then
    echo "File $file does not exists"
    exit 1;
fi

grep -qw "\[ERROR\]" $file
# if [ $? -eq 0 ]; then
#     grep -w "\[ERROR\]" $file
#     exit 1;
# fi