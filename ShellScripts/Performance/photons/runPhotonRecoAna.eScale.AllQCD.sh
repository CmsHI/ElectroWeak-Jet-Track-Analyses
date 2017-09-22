#!/bin/bash

#g++ -Wall -Werror -Wextra -O2 `root-config --cflags --libs` -lTMVA -lRooFitCore -lRooFit -MMD -MF ./build/Performance/photons/photonRecoAna.d Performance/photons/photonRecoAna.C -o Performance/photons/photonRecoAna.exe

runCmd="./ShellScripts/myRun.sh"
progPath="./Performance/photons/photonRecoAna.exe"
inputFile="/mnt/hadoop/cms/store/user/tatar/official/Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB/HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v14-v1-FOREST/0.root"

outDir="/export/d00/scratch/"$USER"/EWJTA-out/Performance/photons/"
outList=(
$outDir"photonRecoAna_eScale_AllQCDPhoton30_Hydjet_Cymbal_MB.root"
$outDir"photonRecoAna_eScale_GED_AllQCDPhoton30_Hydjet_Cymbal_MB.root"
$outDir"photonRecoAna_eScale_SIG_AllQCDPhoton30_Hydjet_Cymbal_MB.root"
$outDir"photonRecoAna_eScale_SIG_GED_AllQCDPhoton30_Hydjet_Cymbal_MB.root"
);

configList=(
"Configurations/Performance/photons/photonRecoAna.eScale.AllQCD.conf"
"Configurations/Performance/photons/photonRecoAna.eScale.AllQCD.GED.conf"
"Configurations/Performance/photons/photonRecoAna.eScale.AllQCD.SIG.conf"
"Configurations/Performance/photons/photonRecoAna.eScale.AllQCD.SIG.GED.conf"
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

