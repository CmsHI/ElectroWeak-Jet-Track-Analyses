#!/bin/bash

#g++ -Wall -Werror -Wextra -O2 `root-config --cflags --libs` -lTMVA -lRooFitCore -lRooFit -MMD -MF ./build/Performance/photons/photonRecoAna.d Performance/photons/photonRecoAna.C -o Performance/photons/photonRecoAna.exe

runCmd="time -p"
runCmdStr="time -p"
if [[ $USER == "tatar" ]]; then
  runCmd="$HOME/code/scripts/myRun.sh"
  runCmdStr="myRun"
fi

progPath="./Performance/photons/photonRecoAna.exe"
inputFile="/mnt/hadoop/cms/store/user/tatar/official/Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB/HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v14-v1-FOREST/0.root"

outDir="/export/d00/scratch/"$USER"/EWJTA-out/Performance/photons/"
outList=(
$outDir"photonRecoAna_eScaleRawE_r9_AllQCDPhoton30_Hydjet_Cymbal_MB.root"
$outDir"photonRecoAna_eScaleRawE_GED_r9_AllQCDPhoton30_Hydjet_Cymbal_MB.root"
$outDir"photonRecoAna_eScaleRawE_SIG_r9_AllQCDPhoton30_Hydjet_Cymbal_MB.root"
$outDir"photonRecoAna_eScaleRawE_SIG_GED_r9_AllQCDPhoton30_Hydjet_Cymbal_MB.root"
);

configList=(
"Configurations/Performance/photons/photonRecoAna.eScaleRawE.AllQCD.r9.conf"
"Configurations/Performance/photons/photonRecoAna.eScaleRawE.AllQCD.GED.r9.conf"
"Configurations/Performance/photons/photonRecoAna.eScaleRawE.AllQCD.SIG.r9.conf"
"Configurations/Performance/photons/photonRecoAna.eScaleRawE.AllQCD.SIG.GED.r9.conf"
);

mkdir -p outDir

arrayIndices=${!outList[*]}
for i1 in $arrayIndices
do
    configFile=${configList[i1]}
    outputFile=${outList[i1]}
    outputFileLOG="${outputFile/.root/.log}"
    $runCmd $progPath $configFile $inputFile $outputFile &> $outputFileLOG &
    echo "$runCmdStr $progPath $configFile $inputFile $outputFile &> $outputFileLOG &"
done

