#!/bin/bash

runCmd="./ShellScripts/myRun.sh"
progPath="./Performance/photons/photonTriggerAna.exe"
triggerFiles=(
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIPhoton40AndZ/180218_063756/L1Ntuple_merged_v2.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIPhoton40AndZ/180218_063756/L1Ntuple_merged_v2.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIPhoton40AndZ_egBypassEGVetos1/180218_063831/L1Ntuple_merged_v2.root"
)
inputFile="Configurations/filelists/HIPhoton40AndZ_HIRun2015_PromptReco_v1_HLTfilter_SinglePhoton_FOREST_Run262XXX.list"
## create the file list on the fly
inputDirTmp="/mnt/hadoop/cms/store/user/richard/2015-Data-promptRECO-photonSkims/HIPhoton40AndZ/PbPb-photonHLTFilter-v3/160202_145715/0000"
rm -f $inputFile
ls -1 $inputDirTmp"/"HiForestAOD_6??.root >> $inputFile
ls -1 $inputDirTmp"/"HiForestAOD_7??.root >> $inputFile
ls -1 $inputDirTmp"/"HiForestAOD_8??.root >> $inputFile

outDirBase=$EWJTAOUT
if [ -z "$outDirBase" ]; then
  outDirBase="/export/d00/scratch/"$USER"/EWJTA-out"
fi
outputSuffix="HIPhoton40AndZ_HIRun2015_PromptReco_v1_HLTfilter_SinglePhoton"
outList=(
$outDirBase"/Performance/photons/HIRun2018PbPb/L1T/photonTriggerAna_"$outputSuffix"_L1SingleEG.root"
$outDirBase"/Performance/photons/HIRun2018PbPb/L1T/photonTriggerAna_"$outputSuffix"_L1T.root"
$outDirBase"/Performance/photons/HIRun2018PbPb/L1T/photonTriggerAna_"$outputSuffix"_L1T_egBypassEGVetos1.root"
);

configList=(
"Configurations/Performance/photons/HIRun2018PbPb/L1T/photonTriggerAna.HIPhoton40AndZ.Run2630XX.L1SingleEG.conf"
"Configurations/Performance/photons/HIRun2018PbPb/L1T/photonTriggerAna.HIPhoton40AndZ.Run2630XX.L1T.conf"
"Configurations/Performance/photons/HIRun2018PbPb/L1T/photonTriggerAna.HIPhoton40AndZ.Run2630XX.L1T.egBypassEGVetos1.conf"
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

