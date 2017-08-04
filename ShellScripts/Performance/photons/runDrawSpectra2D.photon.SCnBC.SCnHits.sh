#!/bin/bash

#g++ -Wall -Werror -Wextra -O2 -Wno-narrowing `root-config --cflags --libs` -lTMVA -lRooFitCore -lRooFit -MMD -MF ./build/Performance/drawSpectra.d Performance/drawSpectra.C -o Performance/drawSpectra.exe

runCmd="time -p"
runCmdStr="time -p"
if [[ $USER == "tatar" ]]; then
  runCmd="$HOME/code/scripts/myRun.sh"
  runCmdStr="myRun"
fi

progPath="./Performance/drawSpectra.exe"
inputFile="/mnt/hadoop/cms/store/user/tatar/official/Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB/HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v14-v1-FOREST/0.root"
#inputFile="/mnt/hadoop/cms/store/user/tatar/EGamma/SingleGammaFlatPt10To100_pythia8_Hydjet/0.root"

configFiles=(
"Configurations/photons/spectra2D.photon.SCnBC.conf"
"Configurations/photons/spectra2D.photon.SCnBC.GED.conf"
"Configurations/photons/spectra2D.photon.SCnHits.conf"
"Configurations/photons/spectra2D.photon.SCnHits.GED.conf"
);

outDirBase="/export/d00/scratch/"$USER"/EWJTA-out/"
outputSuffix="Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB"
#outputSuffix="SingleGammaFlatPt10To100_pythia8_Hydjet"
outputFiles=(
$outDirBase"Configurations/photons/spectra2D_photon_SCnBC_"$outputSuffix".root"
$outDirBase"Configurations/photons/spectra2D_photon_SCnBC_GED_"$outputSuffix".root"
$outDirBase"Configurations/photons/spectra2D_photon_SCnHits_"$outputSuffix".root"
$outDirBase"Configurations/photons/spectra2D_photon_SCnHits_GED_"$outputSuffix".root"
);

arrayIndices=${!outputFiles[*]}
for i1 in $arrayIndices
do
    configFile=${configFiles[i1]}
    outputFile=${outputFiles[i1]}
    outputFileLOG="${outputFile/.root/.log}"
    outDir=$(dirname "${outputFile}")
    mkdir -p $outDir
    $runCmd $progPath $configFile $inputFile $outputFile &> $outputFileLOG &
    echo "$runCmdStr $progPath $configFile $inputFile $outputFile &> $outputFileLOG &"
done


