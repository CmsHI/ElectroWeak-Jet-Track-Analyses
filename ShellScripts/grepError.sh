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
  if [[ ${file} == *.exe || ${file} == *.root ]]; then  # do not check binary files
    echo "File "${file}" is binary. Skipping file".
  else
    #echo ${file}
    #grep -i --color "Segmentation" ${file}
    #grep -i --color "Killed" ${file}
    grep --color -e "Killed" -e "Aborted" -e "Bus error" -e "egmentation" -e "gfal-copy error" -e "error writing" -e "srm client error" -e "Unable to open" ${file}
  fi
done
