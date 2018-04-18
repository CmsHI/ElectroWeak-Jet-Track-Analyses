#!/bin/bash

runCmd="./ShellScripts/myRun.sh"
progPath="./Performance/photons/photonTriggerAna.exe"
triggerFiles=(
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_XeXeRun2017_HIMinimumBias/180205_205303/L1Ntuple_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_XeXeRun2017_HIMinimumBias_egBypassEGVetos1/180205_205730/L1Ntuple_merged.root"
)
inputFile="/mnt/hadoop/cms/store/user/cmcginn/XeXe_94X_SkimForest_MERGED/HIMinimumBias8/HiForestAOD_HIMinimumBias8_HighPtL1Skim_Jet30OrGamma15_MERGED_20180123.root"

configList=(
"Configurations/Performance/photons/HIRun2018PbPb/L1T/photonTriggerAna.L1T.XeXeRun2017.HIMinimumBias.conf"
"Configurations/Performance/photons/HIRun2018PbPb/L1T/photonTriggerAna.L1T.XeXeRun2017.HIMinimumBias.egBypassEGVetos1.conf"
);

outDirBase=$EWJTAOUT
if [ -z "$outDirBase" ]; then
  outDirBase="/export/d00/scratch/"$USER"/EWJTA-out"
fi
outputSuffix="L1Ntuple_XeXeRun2017_HIMinimumBias"
outList=(
$outDirBase"/Performance/photons/HIRun2018PbPb/L1T/photonTriggerAna_"$outputSuffix".root"
$outDirBase"/Performance/photons/HIRun2018PbPb/L1T/photonTriggerAna_"$outputSuffix"_egBypassEGVetos1.root"
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

