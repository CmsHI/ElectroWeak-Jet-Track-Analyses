#!/bin/bash

# simple script to grep error messages from a log file.

if [ $# -lt 1 ]; then
  echo "Usage: ./ShellScripts/grepError.sh [inputFile]"
  echo "Example: ./ShellScripts/grepError.sh my/Analysis/out.log"
  exit 1
fi

files=( $@ )
for file in "${files[@]}"
do
  echo ${file}
  grep -i --color "Segmentation" ${file}
done
