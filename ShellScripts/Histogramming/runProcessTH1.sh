#!/bin/bash

runCmd="./ShellScripts/myRun.sh"

progPath="./Histogramming/processTH1.exe"
progCode="${progPath/.exe/.C}"
g++ $progCode $(root-config --cflags --libs) -Werror -Wall -Wextra -Wno-narrowing -lTMVA -lRooFitCore -lRooFit -O2 -o $progPath || exit 1

input1="./Data/zBosons/zJet_ResultData.root"
inputsList=(
$input1
$input1
$input1
$input1","$input1
$input1","$input1
$input1","$input1
$input1","$input1
$input1","$input1
);

outList=(
"./Histogramming/processTH1.root"
"./Histogramming/processTH1.root"
"./Histogramming/processTH1.root"
"./Histogramming/processTH1.root"
"./Histogramming/processTH1.root"
"./Histogramming/processTH1.root"
"./Histogramming/processTH1.root"
"./Histogramming/processTH1.root"
);

hInPathsList=(
"h1D_xjz_pp"
"h1D_xjz_pp"
"h1D_xjz_pbpb_cent030"
"h1D_xjz_pbpb_cent030,h1D_xjz_pp"
"h1D_rjz_pbpb_cent030,h1D_rjz_pp"
"h1D_xjzMean_pp,h1D_rjz_pp"
"h1D_xjz_pp_cent030,h1D_xjz_pp"
"h1D_xjzMean_pbpb_cent030,h1D_xjzMean_pp"
);

hOutPathList=(
"h1D_xjz_pp"
"h1D_xjz_pp_unitNorm"
"h1D_xjz_pbpb_cent030_unitNorm"
"h1D_xjz_pbpb_cent030_pp_SUMMED"
"h1D_rjz_pbpb_cent030_pp_SUBTRACTED"
"h1D_xjzMean_rjz_pp_MULTIPLIED"
"h1D_xjz_pp_cent030_pp_DIVIDED"
"h1D_xjzMean_pbpb_cent030_pp_DIVIDED"
);

writeMode="UPDATE"

operationList=(
"NONE"
"unitnorm"
"unitnorm"
"add"
"sub"
"mul"
"div"
"div"
);


arrayIndices=${!outList[*]}
for i1 in $arrayIndices
do
    inputFiles=${inputsList[i1]}
    outputFile=${outList[i1]}
    hInPaths=${hInPathsList[i1]}
    hOutPath=${hOutPathList[i1]}
    operation=${operationList[i1]}
    outputFileLOG="${outputFile/.root/_${hOutPath}.log}"
    outDir=$(dirname "${outputFile}")
    mkdir -p $outDir
    $runCmd $progPath $inputFiles $outputFile $writeMode $hInPaths $hOutPath $operation &> $outputFileLOG
    echo "$runCmd $progPath $inputFiles $outputFile $writeMode $hInPaths $hOutPath $operation &> $outputFileLOG"
done

