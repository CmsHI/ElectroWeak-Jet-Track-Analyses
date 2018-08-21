#!/bin/bash

runCmd="./ShellScripts/myRun.sh"
progPath="./Performance/photons/photonTriggerAna.exe"
triggerFiles=(
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/HLT/Pythia8_AllQCDPhoton15_Hydjet_Quenched_Cymbal5Ev8/hltTestEgamma_V29/openHLT_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/HLT/Pythia8_AllQCDPhoton30_Hydjet_Quenched_Cymbal5Ev8/hltTestEgamma_V29/openHLT_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/HLT/Pythia8_EmEnrichedDijet30_Hydjet_Quenched_Cymbal5Ev8/hltTestEgamma_V29/openHLT_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/HLT/Pythia8_EmEnrichedDijet50_Hydjet_Quenched_Cymbal5Ev8/hltTestEgamma_V29/openHLT_merged.root"
);

inputList=(
"Configurations/filelists/HIRun2018PbPb/Pythia8_AllQCDPhoton15_Hydjet_Quenched_Cymbal5Ev8_pp_on_AA_FOREST.list"
"Configurations/filelists/HIRun2018PbPb/Pythia8_AllQCDPhoton30_Hydjet_Quenched_Cymbal5Ev8_pp_on_AA_FOREST.list"
"Configurations/filelists/HIRun2018PbPb/Pythia8_EmEnrichedDijet30_Hydjet_Quenched_Cymbal5Ev8_pp_on_AA_FOREST.list"
"Configurations/filelists/HIRun2018PbPb/Pythia8_EmEnrichedDijet50_Hydjet_Quenched_Cymbal5Ev8_pp_on_AA_FOREST.list"
);

configList=(
"Configurations/Performance/photons/HIRun2018PbPb/HLT/photonTriggerAna.HLT.Pythia.Hydjet.conf"
"Configurations/Performance/photons/HIRun2018PbPb/HLT/photonTriggerAna.HLT.Pythia.Hydjet.conf"
"Configurations/Performance/photons/HIRun2018PbPb/HLT/photonTriggerAna.HLT.Pythia.Hydjet.conf"
"Configurations/Performance/photons/HIRun2018PbPb/HLT/photonTriggerAna.HLT.Pythia.Hydjet.conf"
);

outDirBase=$EWJTAOUT
if [ -z "$outDirBase" ]; then
  outDirBase="/export/d00/scratch/"$USER"/EWJTA-out"
fi
outputSuffix="Hydjet_Quenched_Cymbal5Ev8"
outList=(
$outDirBase"/Performance/photons/HIRun2018PbPb/HLT/photonTriggerAna_Pythia8_AllQCDPhoton15_"$outputSuffix".root"
$outDirBase"/Performance/photons/HIRun2018PbPb/HLT/photonTriggerAna_Pythia8_AllQCDPhoton30_"$outputSuffix".root"
$outDirBase"/Performance/photons/HIRun2018PbPb/HLT/photonTriggerAna_Pythia8_EmEnrichedDijet30_"$outputSuffix".root"
$outDirBase"/Performance/photons/HIRun2018PbPb/HLT/photonTriggerAna_Pythia8_EmEnrichedDijet50_"$outputSuffix".root"
);

arrayIndices=${!outList[*]}
for i1 in $arrayIndices
do
    configFile=${configList[i1]}
    triggerFile=${triggerFiles[i1]}
    inputFile=${inputList[i1]}
    outputFile=${outList[i1]}
    outputFileLOG="${outputFile/.root/.log}"
    outDir=$(dirname "${outputFile}")
    mkdir -p $outDir
    $runCmd $progPath $configFile $triggerFile $inputFile $outputFile &> $outputFileLOG &
    echo "$runCmd $progPath $configFile $triggerFile $inputFile $outputFile &> $outputFileLOG &"
done

