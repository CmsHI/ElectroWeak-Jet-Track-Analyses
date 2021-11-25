#!/bin/bash

# simple script to delete a list of files on /mnt/hadoop/

if [ $# -lt 1 ]; then
  echo "Usage: ./ShellScripts/gfalrmFileList.sh [text file with one file per line]"
  exit 1
fi

fileList=$1

# https://stackoverflow.com/questions/10929453/read-a-file-line-by-line-assigning-the-value-to-a-variable/10929511#10929511
# http://mywiki.wooledge.org/BashFAQ/001
while IFS= read -r line; do
#  echo "Deleting file: $line"
  sourceFile=${line}
  srmPrefix="/mnt/hadoop/"
  sourceFileSRM=${sourceFile#${srmPrefix}}
  gfal-rm gsiftp://se01.cmsaf.mit.edu:2811/${sourceFileSRM}
done < "${fileList}"

