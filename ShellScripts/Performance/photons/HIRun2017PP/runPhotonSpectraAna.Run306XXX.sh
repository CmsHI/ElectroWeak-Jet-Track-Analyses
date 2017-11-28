#!/bin/bash

runCmd="./ShellScripts/myRun.sh"
progPath="./Performance/photons/photonSpectraAna.exe"
#inputFile="Configurations/filelists/HIRun2017PP/Prompt_Reco_HighEGJet_Run306631.list"
inputFile="Configurations/filelists/HIRun2017PP/Prompt_Reco_HighEGJet_Run306604.list"

userInitial=${USER:0:1}
outDirBase="/afs/cern.ch/work/"$userInitial"/"$USER"/public/code/ElectroWeak-Jet-Track-Analyses/EWJTA-out/"
#outputSuffix="Prompt_Reco_HighEGJet_Run306631"
outputSuffix="Prompt_Reco_HighEGJet_Run306604"
outList=(
$outDirBase"Performance/photons/HIRun2017PP/photonSpectraAna_"$outputSuffix".root"
);

configList=(
"Configurations/Performance/photons/HIRun2017PP/photonSpectraAna.Run306XXX.conf"
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

