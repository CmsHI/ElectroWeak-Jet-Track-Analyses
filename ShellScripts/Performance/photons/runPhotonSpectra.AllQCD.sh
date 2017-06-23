#!/bin/bash

#g++ -Wall -Werror -Wextra -O2 `root-config --cflags --libs` -lTMVA -lRooFitCore -lRooFit -MMD -MF ./build/Performance/photons/photonSpectra.d Performance/photons/photonSpectra.C -o Performance/photons/photonSpectra.exe

runCmd="time -p"
runCmdStr="time -p"
if [[ $USER == "tatar" ]]; then
  runCmd="$HOME/code/scripts/myRun.sh"
  runCmdStr="myRun"
fi

inputFile="/mnt/hadoop/cms/store/user/tatar/official/Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB/HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v14-v1-FOREST/0.root"

outDir="/export/d00/scratch/"$USER"/EWJTA-out/Performance/photons/"
outList=(
$outDir"photonSpectra_AllQCDPhoton30_Hydjet_Cymbal_MB.root"
$outDir"photonSpectra_GED_AllQCDPhoton30_Hydjet_Cymbal_MB.root"
);

configList=(
"Configurations/Performance/photons/photonSpectra.AllQCD.conf"
"Configurations/Performance/photons/photonSpectra.AllQCD.GED.conf"
);

mkdir -p outDir

arrayIndices=${!outList[*]}
for i1 in $arrayIndices
do
    configFile=${configList[i1]}
    outputFile=${outList[i1]}
    outputFileLOG="${outputFile/.root/.log}"
    $runCmd Performance/photons/photonSpectra.exe $configFile $inputFile $outputFile &> $outputFileLOG &
    echo "$runCmdStr Performance/photons/photonSpectra.exe $configFile $inputFile $outputFile &> $outputFileLOG &"
done

