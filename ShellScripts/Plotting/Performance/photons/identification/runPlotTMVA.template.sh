#!/bin/bash

runCmd="./ShellScripts/myRun.sh"
progPath="./Plotting/tmva/plotTMVA.exe"
progCode="${progPath/.exe/.C}"
#g++ $progCode $(root-config --cflags --libs) -Werror -Wall -Wno-narrowing -lTMVA -lTMVAGui -lRooFitCore -lRooFit -O2 -o $progPath || exit 1

inputList=(
$EWJTAOUT"/Performance/photons/identification/pbpb18/tmvaTrainID_EB_minPt40_HoE_sieie.root"
$EWJTAOUT"/Performance/photons/identification/pbpb18/tmvaTrainID_EB_minPt40_HoE_sieie_sumIso4.root"
$EWJTAOUT"/Performance/photons/identification/pbpb18/tmvaBook_EB_minPt40.root"
);

dirPath="dataset/InputVariables_Id"

variableList=(
""
"phoHoverE,phoSigmaIEtaIEta_2012,sumIso4"
""
);

arrayIndices=${!inputList[*]}
for i1 in $arrayIndices
do
    inputFile=${inputList[i1]}
    variables=${variableList[i1]}
    outputDir="${inputFile/.root/_plots}"
    outputFileLOG=${outputDir}".log"
    mkdir -p $outputDir
    $runCmd $progPath $inputFile $dirPath $outputDir --variables=${variables} &> $outputFileLOG &
    echo "$runCmd $progPath $inputFile $dirPath $outputDir --variables=${variables} &> $outputFileLOG &"
    wait
done

