#!/bin/bash

#g++ -Wall -Werror -Wextra -O2 `root-config --cflags --libs` -lTMVA -lRooFitCore -lRooFit -MMD -MF ./build/Performance/photons/photonSpectraAna.d Performance/photons/photonSpectraAna.C -o Performance/photons/photonSpectraAna.exe

runCmd="./ShellScripts/myRun.sh"
progPath="./Performance/photons/photonSpectraAna.exe"
inputFile="/mnt/hadoop/cms/store/user/tatar/official/Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB/HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v14-v1-FOREST/0.root"

outDir="/export/d00/scratch/"$USER"/EWJTA-out/Performance/photons/"
outList=(
$outDir"photonSpectraAna_AllQCDPhoton30_Hydjet_Cymbal_MB.root"
);

configList=(
"Configurations/Performance/photons/photonSpectraAna.conf"
);

mkdir -p outDir

arrayIndices=${!outList[*]}
for i1 in $arrayIndices
do
    configFile=${configList[i1]}
    outputFile=${outList[i1]}
    outputFileLOG="${outputFile/.root/.log}"
    $runCmd $progPath $configFile $inputFile $outputFile &> $outputFileLOG &
    echo "$runCmd $progPath $configFile $inputFile $outputFile &> $outputFileLOG &"
done
