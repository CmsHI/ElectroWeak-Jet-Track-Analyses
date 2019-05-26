#!/bin/bash

runCmd="./ShellScripts/myRun.sh"
progPath="./Plotting/tmva/plotTMVA.exe"
progCode="${progPath/.exe/.C}"
#g++ $progCode $(root-config --cflags --libs) -Werror -Wall -Wno-narrowing -lTMVA -lTMVAGui -lRooFitCore -lRooFit -O2 -o $progPath || exit 1

inputList=(
$EWJTAOUT"/Performance/photons/identification/pbpb18/tmvaTrainID_EB_minPt40_HoE_sieie.root"
$EWJTAOUT"/Performance/photons/identification/pbpb18/tmvaTrainID_EB_minPt40_HoE_sieie.root"
);

dirPath="dataset/InputVariables_Id"

variableList=(
"phoHoverE"
"phoSigmaIEtaIEta_2012"
);

arrayIndices=${!inputList[*]}
for i1 in $arrayIndices
do
    inputFile=${inputList[i1]}
    variable=${variableList[i1]}
    outputFile=${inputFile[i1]}
    outputFile="${inputFile/.root//plot_${variable}}"
    outputFileLOG=${outputFile}".log"
    outDir=$(dirname "${outputFileLOG}")
    mkdir -p $outDir
    $runCmd $progPath $inputFile $dirPath $variable $outputFile &> $outputFileLOG &
    echo "$runCmd $progPath $inputFile $dirPath $variable $outputFile &> $outputFileLOG &"
done

