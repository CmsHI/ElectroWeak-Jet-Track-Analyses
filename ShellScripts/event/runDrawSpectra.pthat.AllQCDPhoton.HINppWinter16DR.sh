#!/bin/bash

#g++ -Wall -Werror -Wextra -O2 -Wno-narrowing `root-config --cflags --libs` -lTMVA -lRooFitCore -lRooFit -MMD -MF ./build/Performance/drawSpectra.d Performance/drawSpectra.C -o Performance/drawSpectra.exe

runCmd="./ShellScripts/myRun.sh"
progPath="./Performance/drawSpectra.exe"
inputFiles=(
"Configurations/filelists/Pythia8_Photon15_pp502_TuneCUETP8M1_HINppWinter16DR_FOREST.list"
"Configurations/filelists/Pythia8_Photon30_pp502_TuneCUETP8M1_HINppWinter16DR_FOREST.list"
"Configurations/filelists/Pythia8_Photon50_pp502_TuneCUETP8M1_HINppWinter16DR_FOREST.list"
"Configurations/filelists/Pythia8_Photon80_pp502_TuneCUETP8M1_HINppWinter16DR_FOREST.list"
"Configurations/filelists/Pythia8_Photon120_pp502_TuneCUETP8M1_HINppWinter16DR_FOREST.list"
"Configurations/filelists/Pythia8_PhotonXX_pp502_TuneCUETP8M1_HINppWinter16DR_FOREST.list"
);

configFiles=(
"Configurations/event/Pythia8_AllQCDPhoton_HINppWinter16DR/spectra.pthat.conf"
"Configurations/event/Pythia8_AllQCDPhoton_HINppWinter16DR/spectra.pthat.conf"
"Configurations/event/Pythia8_AllQCDPhoton_HINppWinter16DR/spectra.pthat.conf"
"Configurations/event/Pythia8_AllQCDPhoton_HINppWinter16DR/spectra.pthat.conf"
"Configurations/event/Pythia8_AllQCDPhoton_HINppWinter16DR/spectra.pthat.conf"
"Configurations/event/Pythia8_AllQCDPhoton_HINppWinter16DR/spectra.pthat.conf"
);

outDirBase=$EWJTAOUT
if [ -z "$outDirBase" ]; then
  outDirBase="/export/d00/scratch/"$USER"/EWJTA-out"
fi
outputFiles=(
$outDirBase"/Configurations/event/spectra_pthat_Photon15_pp502_TuneCUETP8M1_HINppWinter16DR.root"
$outDirBase"/Configurations/event/spectra_pthat_Photon30_pp502_TuneCUETP8M1_HINppWinter16DR.root"
$outDirBase"/Configurations/event/spectra_pthat_Photon50_pp502_TuneCUETP8M1_HINppWinter16DR.root"
$outDirBase"/Configurations/event/spectra_pthat_Photon80_pp502_TuneCUETP8M1_HINppWinter16DR.root"
$outDirBase"/Configurations/event/spectra_pthat_Photon120_pp502_TuneCUETP8M1_HINppWinter16DR.root"
$outDirBase"/Configurations/event/spectra_pthat_PhotonXX_pp502_TuneCUETP8M1_HINppWinter16DR.root"
);

arrayIndices=${!outputFiles[*]}
for i1 in $arrayIndices
do
    inputFile=${inputFiles[i1]}
    configFile=${configFiles[i1]}
    outputFile=${outputFiles[i1]}
    outputFileLOG="${outputFile/.root/.log}"
    outDir=$(dirname "${outputFile}")
    mkdir -p $outDir
    $runCmd $progPath $configFile $inputFile $outputFile &> $outputFileLOG &
    echo "$runCmd $progPath $configFile $inputFile $outputFile &> $outputFileLOG &"
done


