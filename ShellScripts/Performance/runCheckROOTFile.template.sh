#!/bin/bash

runCmd="./ShellScripts/myRun.sh"
progPath="./Performance/checkROOTFile.exe"
progCode="${progPath/.exe/.C}"
#g++ $progCode $(root-config --cflags --libs) -Werror -Wall -Wno-narrowing -lTMVA -lRooFitCore -lRooFit -O2 -o $progPath || exit 1

inputList=(
"Configurations/filelists/official/HIRun2018PbPb/MinBias_Hydjet_Drum5F_2018_5p02TeV_HINPbPbAutumn18DR_NoPU_103X_upgrade2018_realistic_HI_v11_v1_FOREST.list"
);

outDirBase=$EWJTAOUT
if [ -z "$outDirBase" ]; then
  outDirBase="/export/d00/scratch/"$USER"/EWJTA-out"
fi
outList=(
$outDirBase"/Performance/checkROOTFile/out_MinBias_Hydjet_Drum5F_2018_5p02TeV_HINPbPbAutumn18DR_NoPU_103X_upgrade2018_realistic_HI_v11_v1_FOREST.log"
);

arrayIndices=${!outList[*]}
for i1 in $arrayIndices
do
    inputFile=${inputList[i1]}
    outputFile=${outList[i1]}
    outDir=$(dirname "${outputFile}")
    mkdir -p $outDir
    $runCmd $progPath $inputFile &> $outputFile &
    echo "$runCmd $progPath $inputFile &> $outputFile &"
done


