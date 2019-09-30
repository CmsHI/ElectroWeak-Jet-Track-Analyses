#!/bin/bash

## First run the following script so that the input for this script is produced :
## ShellScripts/event/HIRun2017PP/runDrawTree.pthat.QCDPhoton.RunIIpp5Spring18DR.sh

runCmd="./ShellScripts/myRun.sh"
progPath="./Corrections/calcPtHatWeights.exe"
progCode="${progPath/.exe/.C}"
g++ $progCode $(root-config --cflags --libs) -Werror -Wall -Wno-narrowing -lTMVA -lRooFitCore -lRooFit -O2 -o $progPath || exit 1

outDirBase=$EWJTAOUT
if [ -z "$outDirBase" ]; then
  USERGRID=$(voms-proxy-info -issuer | awk '{split($0,myLine,"/CN="); print myLine[2]}')
  if [ -z "$USERGRID" ]; then
    USERGRID=$USER
  fi
  outDirBase="/mnt/hadoop/cms/store/user/"$USERGRID"/EWJTA-out"
fi

file1=$outDirBase"/Configurations/event/HIRun2017PP/spectra_pthat_QCDPhoton15_TuneCP5_5p02TeV_pythia8_RunIIpp5Spring18DR.root"
file2=$outDirBase"/Configurations/event/HIRun2017PP/spectra_pthat_QCDPhotonXX_TuneCP5_5p02TeV_pythia8_RunIIpp5Spring18DR.root"

hist1="h_0"
hist2="h_0"

ptHatMins=(15 30 50 80  120 170);
ptHatMaxs=(30 50 80 120 170 -1);

arrayIndices=${!ptHatMins[*]}
for i1 in $arrayIndices
do
    ptHatMin=${ptHatMins[i1]}
    ptHatMax=${ptHatMaxs[i1]}
    $runCmd $progPath $file1 $file2 $hist1 $hist2 $ptHatMin $ptHatMax
done
