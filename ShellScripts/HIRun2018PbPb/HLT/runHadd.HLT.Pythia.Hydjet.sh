#!/bin/bash

runCmd="./ShellScripts/myRun.sh"
runScript="./ShellScripts/runHadd.sh"

set -x

outputList=(
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/HLT/Pythia8_AllQCDPhoton15_Hydjet_Quenched_Cymbal5Ev8/hltTestEgamma_V29/openHLT_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/HLT/Pythia8_AllQCDPhoton30_Hydjet_Quenched_Cymbal5Ev8/hltTestEgamma_V29/openHLT_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/HLT/Pythia8_EmEnrichedDijet30_Hydjet_Quenched_Cymbal5Ev8/hltTestEgamma_V29/openHLT_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/HLT/Pythia8_EmEnrichedDijet50_Hydjet_Quenched_Cymbal5Ev8/hltTestEgamma_V29/openHLT_merged.root"
);

inputList=(
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/HLT/Pythia8_AllQCDPhoton15_Hydjet_Quenched_Cymbal5Ev8/hltTestEgamma_V29/180809_081856/0000/openHLT_*.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/HLT/Pythia8_AllQCDPhoton30_Hydjet_Quenched_Cymbal5Ev8/hltTestEgamma_V29/180809_082457/0000/openHLT_*.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/HLT/Pythia8_EmEnrichedDijet30_Hydjet_Quenched_Cymbal5Ev8/hltTestEgamma_V29/180809_082723/0000/openHLT_*.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/HLT/Pythia8_EmEnrichedDijet50_Hydjet_Quenched_Cymbal5Ev8/hltTestEgamma_V29/180809_082826/0000/openHLT_*.root"
);

outDirBase=$EWJTAOUT
if [ -z "$outDirBase" ]; then
  outDirBase="/export/d00/scratch/"$USER"/EWJTA-out"
fi
logFileList=(
$outDirBase"/ShellScripts/HIRun2018PbPb/HLT/runHadd_HLT_Pythia8_AllQCDPhoton15_Hydjet_Quenched_Cymbal5Ev8_hltTestEgamma_V29.log"
$outDirBase"/ShellScripts/HIRun2018PbPb/HLT/runHadd_HLT_Pythia8_AllQCDPhoton30_Hydjet_Quenched_Cymbal5Ev8_hltTestEgamma_V29.log"
$outDirBase"/ShellScripts/HIRun2018PbPb/HLT/runHadd_HLT_Pythia8_EmEnrichedDijet30_Hydjet_Quenched_Cymbal5Ev8_hltTestEgamma_V29.log"
$outDirBase"/ShellScripts/HIRun2018PbPb/HLT/runHadd_HLT_Pythia8_EmEnrichedDijet50_Hydjet_Quenched_Cymbal5Ev8_hltTestEgamma_V29.log"
);

arrayIndices=${!outputList[*]}
for i1 in $arrayIndices
do
    inputFiles=${inputList[i1]}
    outputFile=${outputList[i1]}
    logFile=${logFileList[i1]}
    outDir=$(dirname "${logFile}")
    mkdir -p $outDir
    $runCmd $runScript $outputFile $inputFiles &> $logFile
    echo "$runCmd $runScript $outputFile $inputFiles &> $logFile"
done

