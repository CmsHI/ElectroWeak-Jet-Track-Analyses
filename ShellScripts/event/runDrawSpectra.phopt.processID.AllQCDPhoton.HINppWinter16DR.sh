#!/bin/bash

runCmd="./ShellScripts/myRun.sh"
progPath="./Performance/drawSpectra.exe"
progCode="${progPath/.exe/.C}"
#g++ $progCode $(root-config --cflags --libs) -Werror -Wall -Wno-narrowing -lTMVA -lRooFitCore -lRooFit -O2 -o $progPath || exit 1

inputList=(
"Configurations/filelists/Pythia8_Photon15_pp502_TuneCUETP8M1_HINppWinter16DR_FOREST.list"
"Configurations/filelists/Pythia8_PhotonXX_pp502_TuneCUETP8M1_HINppWinter16DR_FOREST.list"
"Configurations/filelists/Pythia8_Photon15_pp502_TuneCUETP8M1_HINppWinter16DR_FOREST.list"
"Configurations/filelists/Pythia8_PhotonXX_pp502_TuneCUETP8M1_HINppWinter16DR_FOREST.list"

);

configList=(
"Configurations/event/spectra.phopt.processID.conf"
"Configurations/event/spectra.phopt.processID.conf"
"Configurations/event/spectra.phopt.processID.mcCalIso.conf"
"Configurations/event/spectra.phopt.processID.mcCalIso.conf"
);

outDirBase=$EWJTAOUT
if [ -z "$outDirBase" ]; then
  outDirBase="/export/d00/scratch/"$USER"/EWJTA-out"
fi
outList=(
$outDirBase"/Configurations/event/spectra_phopt_processID_Photon15_pp502_TuneCUETP8M1_HINppWinter16DR.root"
$outDirBase"/Configurations/event/spectra_phopt_processID_PhotonXX_pp502_TuneCUETP8M1_HINppWinter16DR.root"
$outDirBase"/Configurations/event/spectra_phopt_processID_mcCalIso_Photon15_pp502_TuneCUETP8M1_HINppWinter16DR.root"
$outDirBase"/Configurations/event/spectra_phopt_processID_mcCalIso_PhotonXX_pp502_TuneCUETP8M1_HINppWinter16DR.root"
);

arrayIndices=${!outList[*]}
for i1 in $arrayIndices
do
    inputFile=${inputList[i1]}
    configFile=${configList[i1]}
    outputFile=${outList[i1]}
    outputFileLOG="${outputFile/.root/.log}"
    outDir=$(dirname "${outputFile}")
    mkdir -p $outDir
    $runCmd $progPath $configFile $inputFile $outputFile &> $outputFileLOG &
    echo "$runCmd $progPath $configFile $inputFile $outputFile &> $outputFileLOG &"
done


