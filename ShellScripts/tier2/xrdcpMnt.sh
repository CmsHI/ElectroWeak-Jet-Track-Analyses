#!/bin/bash

# simple script to xrdcp the file with "/mnt" path.

if [ $# -lt 2 ]; then
  echo "Usage: ./ShellScripts/xrdcpMnt.sh [mnt path to src] [destination]"
  echo "Example: ./ShellScripts/xrdcpMnt.sh /mnt/hadoop/cms/store/user/username/file.root /export/d00/scratch/username/filecopy.root"
  exit 1
fi

inputFile=${1}
outputFile=${2}

str1="\/mnt\/T2_US_MIT\/hadoop\/cms\/store"
str2="root://xrootd.cmsaf.mit.edu//store"
inputFileXrootd="${inputFile/${str1}/${str2}}"

str1="\/mnt\/hadoop\/cms\/store"
inputFileXrootd="${inputFile/${str1}/${str2}}"

#echo "input : "${inputFile}
#echo "outputFile : "${outputFile}
#echo "inputFileXrootd : "${inputFileXrootd}

xrdcp ${inputFileXrootd} ${outputFile}

