#!/bin/bash

# simple script to replace the "xroot" path with "/mnt".

if [ $# -lt 1 ]; then
  echo "Usage: ./ShellScripts/replaceXrootdWithMnt.sh [inputFile]"
  echo "Example: ./ShellScripts/replaceXrootdWithMnt.sh Configurations/filelists/Pythia8_AllQCDPhoton30Flt30_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST.list"
  exit 1
fi

inputFiles=( $@ )
for inputFile in "${inputFiles[@]}"
do
  if [[ $inputFile = *.root ]];  # if input is a ROOT file, then just print the new file path
  then
    echo $inputFile | sed "s,root://xrootd.cmsaf.mit.edu//store,/mnt/hadoop/cms/store,g"
  else
    sed -i "s,root://xrootd.cmsaf.mit.edu//store,/mnt/hadoop/cms/store,g" $inputFile
  fi
done

