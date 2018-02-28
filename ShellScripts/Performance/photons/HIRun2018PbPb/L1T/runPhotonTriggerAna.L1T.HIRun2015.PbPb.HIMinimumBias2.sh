#!/bin/bash

runCmd="./ShellScripts/myRun.sh"
progPath="./Performance/photons/photonTriggerAna.exe"
triggerFiles=(
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIMinimumBias2/180225_061703/L1Ntuple_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIMinimumBias2/180225_061703/L1Ntuple_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIMinimumBias2_egBypassEGVetos1/180225_061816/L1Ntuple_merged.root"
)
inputFile="Configurations/filelists/HIMinimumBias2_HIRun2015-PromptReco-v1-Run263233-263284_FOREST_2345.list"
## create the file list during execution
inputDirTmp="/mnt/hadoop/cms/store/user/tatar/HIMinimumBias2/HIRun2015-PromptReco-v1-Run263233-263284-FOREST"
rm -f $inputFile
ls -1 $inputDirTmp"/2.root" >> $inputFile
ls -1 $inputDirTmp"/3.root" >> $inputFile
ls -1 $inputDirTmp"/4.root" >> $inputFile
ls -1 $inputDirTmp"/5.root" >> $inputFile

outDirBase=$EWJTAOUT
if [ -z "$outDirBase" ]; then
  outDirBase="/export/d00/scratch/"$USER"/EWJTA-out"
fi
outputSuffix="HIMinimumBias2_HIRun2015_PromptReco_v1_Run263233_263284"
outList=(
$outDirBase"/Performance/photons/HIRun2018PbPb/L1T/photonTriggerAna_"$outputSuffix"_L1SingleEG.root"
$outDirBase"/Performance/photons/HIRun2018PbPb/L1T/photonTriggerAna_"$outputSuffix"_L1T.root"
$outDirBase"/Performance/photons/HIRun2018PbPb/L1T/photonTriggerAna_"$outputSuffix"_L1T_egBypassEGVetos1.root"
);

configList=(
"Configurations/Performance/photons/HIRun2018PbPb/L1T/photonTriggerAna.HIMinimumBias2.Run263XXX.L1SingleEG.conf"
"Configurations/Performance/photons/HIRun2018PbPb/L1T/photonTriggerAna.HIMinimumBias2.Run263XXX.L1T.conf"
"Configurations/Performance/photons/HIRun2018PbPb/L1T/photonTriggerAna.HIMinimumBias2.Run263XXX.L1T.egBypassEGVetos1.conf"
);

arrayIndices=${!outList[*]}
for i1 in $arrayIndices
do
    configFile=${configList[i1]}
    triggerFile=${triggerFiles[i1]}
    outputFile=${outList[i1]}
    outputFileLOG="${outputFile/.root/.log}"
    outDir=$(dirname "${outputFile}")
    mkdir -p $outDir
    $runCmd $progPath $configFile $triggerFile $inputFile $outputFile &> $outputFileLOG &
    echo "$runCmd $progPath $configFile $triggerFile $inputFile $outputFile &> $outputFileLOG &"
done

