#!/bin/bash

#g++ -Wall -Werror -Wextra -O2 `root-config --cflags --libs` -lTMVA -lRooFitCore -lRooFit -MMD -MF ./build/Performance/photons/photonRecoAna.d Performance/photons/photonRecoAna.C -o Performance/photons/photonRecoAna.exe

runCmd="./ShellScripts/myRun.sh"
progPath="./Performance/photons/photonRecoAna.exe"
inputFile="/mnt/hadoop/cms/store/user/tatar/EGamma/SingleGammaFlatPt10To200_pythia8_Hydjet/0.root"

configList=(
"Configurations/Performance/photons/photonRecoAna.eScale.photonGun.conf"
"Configurations/Performance/photons/photonRecoAna.eScale.photonGun.GED.conf"
"Configurations/Performance/photons/photonRecoAna.eScale.photonGun.SIG.conf"
"Configurations/Performance/photons/photonRecoAna.eScale.photonGun.SIG.GED.conf"
);

outDirBase=$EWJTAOUT
if [ -z "$outDirBase" ]; then
  outDirBase="/export/d00/scratch/"$USER"/EWJTA-out"
fi
outList=(
$outDirBase"/Performance/photons/photonRecoAna_eScale_SingleGammaFlatPt10To200_pythia8_Hydjet.root"
$outDirBase"/Performance/photons/photonRecoAna_eScale_GED_SingleGammaFlatPt10To200_pythia8_Hydjet.root"
$outDirBase"/Performance/photons/photonRecoAna_eScale_SIG_SingleGammaFlatPt10To200_pythia8_Hydjet.root"
$outDirBase"/Performance/photons/photonRecoAna_eScale_SIG_GED_SingleGammaFlatPt10To200_pythia8_Hydjet.root"
);

arrayIndices=${!outList[*]}
for i1 in $arrayIndices
do
    configFile=${configList[i1]}
    outputFile=${outList[i1]}
    outputFileLOG="${outputFile/.root/.log}"
    outDir=$(dirname "${outputFile}")
    mkdir -p $outDir
    $runCmd $progPath $configFile $inputFile $outputFile &> $outputFileLOG &
    echo "$runCmd $progPath $configFile $inputFile $outputFile &> $outputFileLOG &"
done

