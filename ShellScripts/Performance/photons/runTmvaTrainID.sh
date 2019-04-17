#!/bin/bash

inputSig="Configurations/filelists/Pythia8_PhotonXX_pp502_TuneCUETP8M1_HINppWinter16DR_FOREST.list"
inputBkg="Configurations/filelists/Pythia8_EmEnrDijetXX_pp502_TuneCUETP8M1_HINppWinter16DR_FOREST.list"

runCmd="./ShellScripts/myRun.sh"
progPath="./Performance/objects/tmvaTrainID.exe"
progCode="${progPath/.exe/.C}"
g++ $progCode $(root-config --cflags --libs) -Werror -Wall -Wno-narrowing -lTMVA -lRooFitCore -lRooFit -O2 -o $progPath || exit 1

configList=(
"DUMMY.conf"
);

outDirBase=$EWJTAOUT
if [ -z "$outDirBase" ]; then
  outDirBase="/export/d00/scratch/"$USER"/EWJTA-out"
fi
outList=(
$outDirBase"/Performance/photons/tmvatrainID_out.root"
);

methodLabelList=(
"preSel_phoEt40_EB_hoe0p1_vars_sieieMin0Max0p14_sumIsoMinM1000Max50"
);

arrayIndices=${!outList[*]}
for i1 in $arrayIndices
do
    configFile=${configList[i1]}
    methodLabel=${methodLabelList[i1]}
    outputFile=${outList[i1]}
    outputFile="${outputFile/.root/_${methodLabel}.root}"
    outputFileLOG="${outputFile/.root/.log}"
    outDir=$(dirname "${outputFile}")
    mkdir -p $outDir
    $runCmd $progPath $configFile $inputSig $inputBkg $outputFile $methodLabel &> $outputFileLOG &
    echo "$runCmd $progPath $configFile $inputSig $inputBkg $outputFile $methodLabel &> $outputFileLOG &"
done

