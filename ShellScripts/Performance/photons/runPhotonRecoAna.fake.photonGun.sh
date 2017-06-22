#!/bin/bash

#g++ -Wall -Werror -Wextra -O2 `root-config --cflags --libs` -lTMVA -lRooFitCore -lRooFit -MMD -MF ./build/Performance/photons/photonRecoAna.d Performance/photons/photonRecoAna.C -o Performance/photons/photonRecoAna.exe

runCmd="time -p"
runCmdStr="time -p"
if [[ $USER == "tatar" ]]; then
  runCmd="$HOME/code/scripts/myRun.sh"
  runCmdStr="myRun"
fi

inputFile="/mnt/hadoop/cms/store/user/tatar/EGamma/SingleGammaFlatPt10To200_pythia8_Hydjet/0.root"

outDir="/export/d00/scratch/"$USER"/EWJTA-out/Performance/photons/"
outList=(
$outDir"photonRecoAna_fake_SingleGammaFlatPt10To200_pythia8_Hydjet.root"
$outDir"photonRecoAna_fake_GED_SingleGammaFlatPt10To200_pythia8_Hydjet.root"
$outDir"photonRecoAna_fake_SIG_SingleGammaFlatPt10To200_pythia8_Hydjet.root"
$outDir"photonRecoAna_fake_SIG_GED_SingleGammaFlatPt10To200_pythia8_Hydjet.root"
);

configList=(
"Configurations/Performance/photons/photonRecoAna.fake.photonGun.conf"
"Configurations/Performance/photons/photonRecoAna.fake.photonGun.GED.conf"
"Configurations/Performance/photons/photonRecoAna.fake.photonGun.SIG.conf"
"Configurations/Performance/photons/photonRecoAna.fake.photonGun.SIG.GED.conf"
);

mkdir -p outDir

arrayIndices=${!outList[*]}
for i1 in $arrayIndices
do
    configFile=${configList[i1]}
    outputFile=${outList[i1]}
    outputFileLOG="${outputFile/.root/.log}"
    $runCmd Performance/photons/photonRecoAna.exe $configFile $inputFile $outputFile &> $outputFileLOG &
    echo "$runCmdStr Performance/photons/photonRecoAna.exe $configFile $inputFile $outputFile &> $outputFileLOG &"
done

