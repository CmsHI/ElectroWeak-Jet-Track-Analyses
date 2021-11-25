#!/bin/bash

# simple script to replace the "/mnt/T2_US_MIT/hadoop" path with "/mnt/hadoop".

if [ $# -lt 1 ]; then
  echo "Usage: ./ShellScripts/replaceMntT2USMIT.sh [inputFile]"
  echo "Example: ./ShellScripts/replaceMntT2USMIT.sh Configurations/filelists/Pythia8_AllQCDPhoton30Flt30_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST.list"
  exit 1
fi

inputFiles=( $@ )
for inputFile in "${inputFiles[@]}"
do
  if [[ $inputFile = *.root ]];  # if input is a ROOT file, then just print the new file path
  then
    echo $inputFile | sed "s,/mnt/T2_US_MIT/hadoop/cms/store,/mnt/hadoop/cms/store,g"
  else
    sed -i "s,/mnt/T2_US_MIT/hadoop/cms/store,/mnt/hadoop/cms/store,g" $inputFile
  fi
done

