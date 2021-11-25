#!/bin/bash

# simple script to replace the "/mnt" path with srm path.

if [ $# -lt 1 ]; then
  echo "Usage: ./ShellScripts/replaceMntWithSRM.sh [inputFile]"
  echo "Example: ./ShellScripts/replaceMntWithSRM.sh Configurations/filelists/Pythia8_AllQCDPhoton30Flt30_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST.list"
  exit 1
fi

inputFiles=( $@ )
for inputFile in "${inputFiles[@]}"
do
  if [[ $inputFile = *.root ]];  # if input is a ROOT file, then just print the new file path
  then
    if [[ $inputFile = *mnt\/T2_US_MIT* ]];
    then
      echo $inputFile | sed "s,/mnt/T2_US_MIT/hadoop/cms/store,gsiftp://se01.cmsaf.mit.edu:2811/,g"
    else
      echo $inputFile | sed "s,/mnt/hadoop/cms/store,gsiftp://se01.cmsaf.mit.edu:2811/,g"
    fi
  else
    sed -i "s,/mnt/hadoop/cms/store,gsiftp://se01.cmsaf.mit.edu:2811/,g" $inputFile
    sed -i "s,/mnt/T2_US_MIT/hadoop/cms/store,gsiftp://se01.cmsaf.mit.edu:2811/,g" $inputFile
  fi
done

