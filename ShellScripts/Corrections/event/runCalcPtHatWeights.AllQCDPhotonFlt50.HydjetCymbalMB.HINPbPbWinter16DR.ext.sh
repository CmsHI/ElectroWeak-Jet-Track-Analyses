#!/bin/bash

## First run the following script so that the input for this script is produced :
## ./ShellScripts/event/runDrawSpectra.pthat.AllQCDPhotonFlt50.HydjetCymbalMB.HINPbPbWinter16DR.ext.sh

runCmd="./ShellScripts/myRun.sh"
progPath="./Corrections/calcPtHatWeights.exe"

outDirBase=$EWJTAOUTGRID
if [ -z "$outDirBase" ]; then
  USERGRID=$(voms-proxy-info -issuer | awk '{split($0,myLine,"/CN="); print myLine[2]}')
  if [ -z "$USERGRID" ]; then
    USERGRID=$USER
  fi
  outDirBase="/mnt/hadoop/cms/store/user/"$USERGRID"/EWJTA-out"
fi

file1=$outDirBase"/Configurations/event/spectra_pthat_AllQCDPhoton30Flt50_Hydjet_Cymbal_MB_HINPbPbWinter16DR_ext.root"
file2=$outDirBase"/Configurations/event/spectra_pthat_AllQCDPhotonXXFlt50_Hydjet_Cymbal_MB_HINPbPbWinter16DR_ext.root"

hist1="h_0"
hist2="h_0"

ptHatMins=(30 50 80  120);
ptHatMaxs=(50 80 120 -1);

arrayIndices=${!ptHatMins[*]}
for i1 in $arrayIndices
do
    ptHatMin=${ptHatMins[i1]}
    ptHatMax=${ptHatMaxs[i1]}
    $runCmd $progPath $file1 $file2 $hist1 $hist2 $ptHatMin $ptHatMax
done
