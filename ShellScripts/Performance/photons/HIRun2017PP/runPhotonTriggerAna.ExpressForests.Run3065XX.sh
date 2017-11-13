#!/bin/bash

runCmd="./ShellScripts/myRun.sh"
progPath="./Performance/photons/photonTriggerAna.exe"
hltFiles=(
"DUMMY.root"
"DUMMY.root"
)
#inputFile="Configurations/filelists/HIRun2017PP_ExpressForests_Run3065XX.list"
inputFile="Configurations/filelists/HIRun2017PP_ExpressForests_Run306572.list"

userInitial=${USER:0:1}
outDirBase="/afs/cern.ch/work/"$userInitial"/"$USER"/public/code/ElectroWeak-Jet-Track-Analyses/EWJTA-out/"
#outputSuffix="ExpressForests_Run3065XX"
outputSuffix="ExpressForests_Run306572"
outList=(
$outDirBase"Performance/photons/HIRun2017PP/photonTriggerAna_"$outputSuffix".root"
$outDirBase"Performance/photons/HIRun2017PP/photonTriggerAna_"$outputSuffix"_HoverELoose.root"
);

configList=(
"Configurations/Performance/photons/HIRun2017PP/photonTriggerAna.ExpressForests.Run3065XX.conf"
"Configurations/Performance/photons/HIRun2017PP/photonTriggerAna.ExpressForests.Run3065XX.HoverELoose.conf"
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

