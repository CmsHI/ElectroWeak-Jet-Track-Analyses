#!/bin/bash

#g++ -Wall -Werror -Wextra -O2 `root-config --cflags --libs` -lTMVA -lRooFitCore -lRooFit -MMD -MF ./build/Performance/photons/photonRecoAna.d Performance/photons/photonRecoAna.C -o Performance/photons/photonRecoAna.exe

runCmd="./ShellScripts/myRun.sh"
progPath="./Performance/photons/photonRecoAna.exe"
inputList=(
"Configurations/filelists/Zee/Pythia8_Ze10e10_Hydjet_MB_HiFall15_FOREST_extendEC.list"
"Configurations/filelists/Zee/Pythia8_Ze10e10_Hydjet_MB_HiFall15_FOREST_extendEC.list"
"Configurations/filelists/Zee/Pythia8_Ze10e10_Hydjet_MB_HiFall15_FOREST_extendEC_pfSC_phiWidth0p20.list"
);

outDirBase=$EWJTAOUT
if [ -z "$outDirBase" ]; then
  outDirBase="/export/d00/scratch/"$USER"/EWJTA-out"
fi
outList=(
$outDirBase"/Performance/photons/photonRecoAna_eScaleRawE_Ze10e10_Hydjet_MB_HiFall15_FOREST_extendEC.root"
$outDirBase"/Performance/photons/photonRecoAna_eScaleRawE_GED_Ze10e10_Hydjet_MB_HiFall15_FOREST_extendEC.root"
$outDirBase"/Performance/photons/photonRecoAna_eScaleRawE_GED_Ze10e10_Hydjet_MB_HiFall15_FOREST_extendEC_pfSC_phiWidth0p20.root"
);

configList=(
"Configurations/Performance/photons/photonRecoAna.eScaleRawE.Zee.conf"
"Configurations/Performance/photons/photonRecoAna.eScaleRawE.Zee.GED.conf"
"Configurations/Performance/photons/photonRecoAna.eScaleRawE.Zee.GED.conf"
);

arrayIndices=${!outList[*]}
for i1 in $arrayIndices
do
    configFile=${configList[i1]}
    inputFile=${inputList[i1]}
    outputFile=${outList[i1]}
    outputFileLOG="${outputFile/.root/.log}"
    outDir=$(dirname "${outputFile}")
    mkdir -p $outDir
    $runCmd $progPath $configFile $inputFile $outputFile &> $outputFileLOG &
    echo "$runCmd $progPath $configFile $inputFile $outputFile &> $outputFileLOG &"
done

