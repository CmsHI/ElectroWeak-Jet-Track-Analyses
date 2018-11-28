#!/bin/bash

runCmd="./ShellScripts/myRun.sh"
progPath="./Performance/photons/photonTriggerAna.exe"

triggerFile="DUMMY.root"
inputList=(
"Configurations/filelists/HIRun2018PbPb/HIMinimumBias0123_HIRun2018A-PromptReco-v1-Run326500-326886-filterPhoPt20-FOREST.list"
"Configurations/filelists/HIRun2018PbPb/HIMinimumBias0123_HIRun2018A-PromptReco-v1-Run326500-326886-filterPhoPt20-FOREST.list"
"Configurations/filelists/HIRun2018PbPb/HIMinimumBias0123_HIRun2018A-PromptReco-v1-Run326500-326886-filterPhoPt20-FOREST.list"
"Configurations/filelists/HIRun2018PbPb/HIMinimumBias0123_HIRun2018A-PromptReco-v1-Run326500-326886-filterPhoPt20-FOREST.list"
);

configList=(
"Configurations/Performance/photons/HIRun2018PbPb/HLT/photonTriggerAna.Run326XXX.conf"
"Configurations/Performance/photons/HIRun2018PbPb/HLT/photonTriggerAna.Run326XXX.EB.conf"
"Configurations/Performance/photons/HIRun2018PbPb/HLT/photonTriggerAna.Run326XXX.HIIslandPhoton.Eta1p5.conf"
"Configurations/Performance/photons/HIRun2018PbPb/HLT/photonTriggerAna.Run326XXX.HIIslandPhoton.Eta2p4.conf"
);

outDirBase=$EWJTAOUT
if [ -z "$outDirBase" ]; then
  outDirBase="/export/d00/scratch/"$USER"/EWJTA-out"
fi

outSuffix="HIMinimumBias0123_PromptReco_Run326500_326886_filterPhoPt20"
outList=(
$outDirBase"/Performance/photons/HIRun2018PbPb/HLT/photonTriggerAna_HIGEDPhoton_"${outSuffix}"_matchHLTobj.root"
$outDirBase"/Performance/photons/HIRun2018PbPb/HLT/photonTriggerAna_HIGEDPhoton_EB_"${outSuffix}"_matchHLTobj.root"
$outDirBase"/Performance/photons/HIRun2018PbPb/HLT/photonTriggerAna_HIIslandPhoton_Eta1p5_"${outSuffix}"_matchHLTobj.root"
$outDirBase"/Performance/photons/HIRun2018PbPb/HLT/photonTriggerAna_HIIslandPhoton_Eta2p4_"${outSuffix}"_matchHLTobj.root"
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
    $runCmd $progPath $configFile $triggerFile $inputFile $outputFile &> $outputFileLOG &
    echo "$runCmd $progPath $configFile $triggerFile $inputFile $outputFile &> $outputFileLOG &"
done

