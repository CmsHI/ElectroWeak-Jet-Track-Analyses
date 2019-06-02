#!/bin/bash

inputSig="Configurations/filelists/Pythia8_PhotonXX_pp502_TuneCUETP8M1_HINppWinter16DR_FOREST.list"
inputBkg="Configurations/filelists/Pythia8_EmEnrDijetXX_pp502_TuneCUETP8M1_HINppWinter16DR_FOREST.list"

runCmd="./ShellScripts/myRun.sh"
progPath="./Performance/objects/tmvaTrain.exe"
progCode="${progPath/.exe/.C}"
#g++ $progCode $(root-config --cflags --libs) -Werror -Wall -Wno-narrowing -lTMVA -lRooFitCore -lRooFit -O2 -o $progPath || exit 1

configList=(
"Configurations/Performance/photons/identification/tmvaTrainID.template.conf"
);

outDirBase=$EWJTAOUT
if [ -z "$outDirBase" ]; then
  outDirBase="/export/d00/scratch/"$USER"/EWJTA-out"
fi
outList=(
$outDirBase"/Performance/photons/identification/tmvatrainID_template.root"
);

jobLabelList=(
"tmvaTrainID_FactoryJob_HINppWinter16DR_phoEt40_EB_hoe0p1_vars_sieieMin0Max0p02_sumIsoMinM400Max50"
);

arrayIndices=${!outList[*]}
for i1 in $arrayIndices
do
    configFile=${configList[i1]}
    jobLabel=${jobLabelList[i1]}
    outputFile=${outList[i1]}
    outputFile="${outputFile/.root/_${jobLabel}.root}"
    outputFileLOG="${outputFile/.root/.log}"
    outDir=$(dirname "${outputFile}")
    mkdir -p $outDir
    $runCmd $progPath $configFile $inputSig $inputBkg $outputFile $jobLabel &> $outputFileLOG &
    echo "$runCmd $progPath $configFile $inputSig $inputBkg $outputFile $jobLabel &> $outputFileLOG &"
done

