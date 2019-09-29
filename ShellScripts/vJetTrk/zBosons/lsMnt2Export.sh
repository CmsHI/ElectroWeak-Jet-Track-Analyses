#!/bin/bash

# simple script to locate copies of skims on /export

if [ $# -lt 1 ]; then
  echo "Usage: ./ShellScripts/lsMnt2Export.sh [mnt file]"
  echo "Example: ./ShellScripts/lsMnt2Export.sh /mnt/hadoop/cms/store/user/username/file.root"
  exit 1
fi

inputFile=${1}

str1_0="\/mnt\/hadoop\/cms\/store/user/katatar"
str2_0="/export/d00/scratch/"${USER}

str1_1="\/merged\/job0.root"
str2_1=".root"

inputTmp=${inputFile}
inputTmp="${inputTmp/${str1_0}/${str2_0}}"
inputTmp="${inputTmp/${str1_1}/${str2_1}}"

ls -1altrh ${inputTmp}

