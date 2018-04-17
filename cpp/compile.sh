#!/bin/bash

if [ $# -lt 1 ]; then

  echo " Usage: $0 <file_name>"
  exit 1
fi

filename=$1
base=$(echo $filename | sed -e 's/\..\+//g')

echo $base
target=${base}

cmd="g++ --std=c++11 -g ${filename} -lpthread -o ${target}"

echo "cmd: $cmd"

eval $cmd
