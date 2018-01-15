#!/bin/bash

runCmd="./ShellScripts/myRun.sh"
progPath="./Performance/photons/photonSpectra.exe"
inputFile="Configurations/filelists/HIRun2017PP/Prompt_Reco_HighEGJet_Run306631.list"

outDirBase=$EWJTAOUT
if [ -z "$outDirBase" ]; then
  userInitial=${USER:0:1}
  outDirBase="/afs/cern.ch/work/"$userInitial"/"$USER"/public/code/ElectroWeak-Jet-Track-Analyses/EWJTA-out"
fi
outputSuffix="Prompt_Reco_HighEGJet_Run306631"
outList=(
$outDirBase"/Performance/photons/HIRun2017PP/photonSpectra_"$outputSuffix".root"
);

configList=(
"Configurations/Performance/photons/HIRun2017PP/photonSpectra.Run306XXX.conf"
);

arrayIndices=${!outList[*]}
for i1 in $arrayIndices
do
    configFile=${configList[i1]}
    hltFile=${hltFiles[i1]}
    outputFile=${outList[i1]}
    outputFileLOG="${outputFile/.root/.log}"
    outDir=$(dirname "${outputFile}")
    mkdir -p $outDir
    $runCmd $progPath $configFile $hltFile $inputFile $outputFile &> $outputFileLOG &
    echo "$runCmd $progPath $configFile $hltFile $inputFile $outputFile &> $outputFileLOG &"
done

