#!/bin/bash

runCmd="./ShellScripts/myRun.sh"
progPath="./Performance/objects/tmvaEval.exe"
progCode="${progPath/.exe/.C}"
#g++ $progCode $(root-config --cflags --libs) -Werror -Wall -Wno-narrowing -lTMVA -lXMLIO -lRooFitCore -lRooFit -O2 -o $progPath || exit 1

configList=(
"Configurations/Performance/photons/identification/pbpb18/EB/tmvaEval.sumIso3.conf"
"Configurations/Performance/photons/identification/pbpb18/EB/tmvaEval.pfSumIso3.subUE.conf"
);

inputList=(
$EWJTAOUT"/Performance/photons/identification/pbpb18/EB/tmvaTrainID_sumIso3.root"
$EWJTAOUT"/Performance/photons/identification/pbpb18/EB/tmvaTrainID_pfSumIso3_subUE.root"
);

xmlList=(
$EWJTAOUT"/Performance/photons/identification/pbpb18/EB/tmvaTrainID_sumIso3_CutsGA.xml"
$EWJTAOUT"/Performance/photons/identification/pbpb18/EB/tmvaTrainID_pfSumIso3_subUE_CutsGA.xml"
);

methodNameList=(
"CutsGA"
"CutsGA"
);

outDirBase=$EWJTAOUT
if [ -z "$outDirBase" ]; then
  outDirBase="/export/d00/scratch/"$USER"/EWJTA-out"
fi
outList=(
$outDirBase"/Performance/photons/identification/pbpb18/EB/tmvaEval_tmvaTrainID_sumIso3.root"
$outDirBase"/Performance/photons/identification/pbpb18/EB/tmvaEval_tmvaTrainID_pfSumIso3_subUE.root"
);

arrayIndices=${!outList[*]}
for i1 in $arrayIndices
do
    configFile=${configList[i1]}
    inputFile=${inputList[i1]}
    xmlFile=${xmlList[i1]}
    methodName=${methodNameList[i1]}
    outputFile=${outList[i1]}
    outputFileLOG="${outputFile/.root/.log}"
    outDir=$(dirname "${outputFile}")
    mkdir -p $outDir
    $runCmd $progPath $configFile $inputFile $xmlFile $outputFile $methodName &> $outputFileLOG &
    echo "$runCmd $progPath $configFile $inputFile $xmlFile $outputFile $methodName &> $outputFileLOG &"
done

