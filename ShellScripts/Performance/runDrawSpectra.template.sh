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

configFiles=(
"Configurations/photons/spectra.photon.conf"
"Configurations/photons/spectra.phoEt.conf"
);

outDirBase="/export/d00/scratch/"$USER"/EWJTA-out/"
outputFiles=(
$outDirBase"Configurations/photons/spectra_photon_Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB.root"
$outDirBase"Configurations/photons/spectra_phoEt_Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB.root"
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


