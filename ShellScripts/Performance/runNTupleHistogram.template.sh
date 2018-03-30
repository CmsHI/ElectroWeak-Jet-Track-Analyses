#!/bin/bash

runCmd="./ShellScripts/myRun.sh"
progPath="./Performance/nTupleHistogram.exe"
inputFiles=(
"Configurations/filelists/Pythia8_Photon30_pp502_TuneCUETP8M1_HINppWinter16DR_FOREST.list"
)

make $progPath || exit 1

outDirBase=$EWJTAOUT
if [ -z "$outDirBase" ]; then
  outDirBase="/export/d00/scratch/"$USER"/EWJTA-out"
fi
outputSuffix="Pythia8_Photon30_pp502_TuneCUETP8M1_HINppWinter16DR"
outList=(
$outDirBase"/Performance/nTupleHistogram_"$outputSuffix".root"
);

arrayIndices=${!outList[*]}
for i1 in $arrayIndices
do
    inputFile=${inputFiles[i1]}
    outputFile=${outList[i1]}
    outputFileLOG="${outputFile/.root/.log}"
    outDir=$(dirname "${outputFile}")
    mkdir -p $outDir
    $runCmd $progPath $inputFile $outputFile &> $outputFileLOG &
    echo "$runCmd $progPath $inputFile $outputFile &> $outputFileLOG &"
done

