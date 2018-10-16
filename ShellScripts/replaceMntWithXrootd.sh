#!/bin/bash

# simple script to replace the "/mnt" path with "xroot".

if [ $# -lt 1 ]; then
  echo "Usage: ./ShellScripts/replaceMntWithXrootd.sh [inputFile]"
  echo "Example: ./ShellScripts/replaceMntWithXrootd.sh Configurations/filelists/Pythia8_AllQCDPhoton30Flt30_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST.list"
  exit 1
fi

inputFile=$1
if [[ $inputFile = *.root ]];  # if input is a ROOT file, then just print the new file path
then
  echo $inputFile | sed "s,/mnt/hadoop/cms/store,root://xrootd.cmsaf.mit.edu//store,g"
else
  sed -i "s,/mnt/hadoop/cms/store,root://xrootd.cmsaf.mit.edu//store,g" $inputFile
fi


