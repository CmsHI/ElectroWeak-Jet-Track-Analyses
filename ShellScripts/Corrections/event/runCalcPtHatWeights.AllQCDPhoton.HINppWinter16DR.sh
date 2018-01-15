#!/bin/bash

runCmd="./ShellScripts/myRun.sh"
progPath="./Corrections/calcPtHatWeights.exe"

outDirBase=$EWJTAOUT
if [ -z "$outDirBase" ]; then
  outDirBase="/export/d00/scratch/"$USER"/EWJTA-out"
fi

file1=$outDirBase"/Configurations/event/spectra_pthat_Photon15_pp502_TuneCUETP8M1_HINppWinter16DR.root"
file2=$outDirBase"/Configurations/event/spectra_pthat_PhotonXX_pp502_TuneCUETP8M1_HINppWinter16DR.root"

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
#    echo "$runCmd $progPath $file1 $file2 $hist1 $hist2 $ptHatMin $ptHatMax"
done
