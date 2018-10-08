#!/bin/bash

runCmd="./ShellScripts/myRun.sh"

progPath="./Histogramming/processTH1.exe"
progCode="${progPath/.exe/.C}"
g++ $progCode $(root-config --cflags --libs) -Werror -Wall -Wextra -Wno-narrowing -lTMVA -lRooFitCore -lRooFit -O2 -o $progPath || exit 1

inputList=(
"./Data/zBosons/zJet_ResultData.root"
"./Data/zBosons/zJet_ResultData.root"
);

outList=(
"./Histogramming/processTH1.root"
"./Histogramming/processTH1.root"
);

hInPathList=(
"h1D_xjz_pp"
"h1D_xjz_pbpb_cent030"
);

hOutPathList=(
"h1D_xjz_pp_unitNorm"
"h1D_xjz_pbpb_cent030_unitNorm"
);

writeMode="UPDATE"

arrayIndices=${!outList[*]}
for i1 in $arrayIndices
do
    inputFile=${inputList[i1]}
    outputFile=${outList[i1]}
    hInPath=${hInPathList[i1]}
    hOutPath=${hOutPathList[i1]}
    outputFileLOG="${outputFile/.root/.log}"
    outDir=$(dirname "${outputFile}")
    mkdir -p $outDir
    $runCmd $progPath $inputFile $outputFile $writeMode $hInPath $hOutPath &> $outputFileLOG
    echo "$runCmd $progPath $inputFile $outputFile $writeMode $hInPath $hOutPath &> $outputFileLOG"
done

