#!/bin/bash

## First run the following script so that the input for this script is produced :
## ./ShellScripts/event/runDrawSpectra.pthat.AllQCDPhotonFlt30.HydjetCymbalMB.HINPbPbWinter16DR.sh

runCmd="./ShellScripts/myRun.sh"
progPath="./Corrections/calcPtHatWeights.exe"

outDirBase=$EWJTAOUT
if [ -z "$outDirBase" ]; then
  outDirBase="/export/d00/scratch/"$USER"/EWJTA-out"
fi

file1=$outDirBase"/Configurations/event/spectra_pthat_AllQCDPhoton15Flt30_Hydjet_Cymbal_MB_HINPbPbWinter16DR.root"
file2=$outDirBase"/Configurations/event/spectra_pthat_AllQCDPhotonXXFlt30_Hydjet_Cymbal_MB_HINPbPbWinter16DR.root"

hist1="h_0"
hist2="h_0"

ptHatMins=(15 30 50 80  120);
ptHatMaxs=(30 50 80 120 -1);

arrayIndices=${!ptHatMins[*]}
for i1 in $arrayIndices
do
    ptHatMin=${ptHatMins[i1]}
    ptHatMax=${ptHatMaxs[i1]}
    $runCmd $progPath $file1 $file2 $hist1 $hist2 $ptHatMin $ptHatMax
done
