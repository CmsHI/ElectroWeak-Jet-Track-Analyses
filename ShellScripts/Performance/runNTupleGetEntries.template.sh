#!/bin/bash

runCmd="./ShellScripts/myRun.sh"
progPath="./Performance/nTupleGetEntries.exe"
inputFile="Configurations/filelists/Pythia8_Photon30_pp502_TuneCUETP8M1_HINppWinter16DR_FOREST.list"

make $progPath || exit 1

outDirBase=$EWJTAOUT
if [ -z "$outDirBase" ]; then
  outDirBase="/export/d00/scratch/"$USER"/EWJTA-out"
fi
outputSuffix="Pythia8_Photon30_pp502_TuneCUETP8M1_HINppWinter16DR"
outputFiles=(
$outDirBase"/Performance/nTupleGetEntries_"$outputSuffix".log"
);

arrayIndices=${!outputFiles[*]}
for i1 in $arrayIndices
do
    outputFile=${outputFiles[i1]}
    outDir=$(dirname "${outputFile}")
    mkdir -p $outDir
    $runCmd $progPath $inputFile &> $outputFile &
    echo "$runCmd $progPath $inputFile &> $outputFile &"
done


