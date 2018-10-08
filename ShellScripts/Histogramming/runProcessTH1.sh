#!/bin/bash

runCmd="./ShellScripts/myRun.sh"

progPath="./Histogramming/processTH1.exe"
progCode="${progPath/.exe/.C}"
g++ $progCode $(root-config --cflags --libs) -Werror -Wall -Wextra -Wno-narrowing -lTMVA -lRooFitCore -lRooFit -O2 -o $progPath || exit 1

inputList=(
"./Data/zBosons/zJet_ResultData.root"
);

outList=(
"./Histogramming/processTH1.root"
);

writeMode="UPDATE"

arrayIndices=${!outList[*]}
for i1 in $arrayIndices
do
    inputFile=${inputList[i1]}
    outputFile=${outList[i1]}
    histDenomPath=${histDenomPathList[i1]}
    histNumPaths=${histNumPathsList[i1]}
    outputFileLOG="${outputFile/.root/.log}"
    outDir=$(dirname "${outputFile}")
    mkdir -p $outDir
    $runCmd $progPath $inputFile $outputFile $writeMode &> $outputFileLOG
    echo "$runCmd $progPath $inputFile $outputFile $writeMode &> $outputFileLOG"
done

