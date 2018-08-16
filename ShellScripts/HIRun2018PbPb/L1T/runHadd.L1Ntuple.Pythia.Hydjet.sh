#!/bin/bash

runCmd="./ShellScripts/myRun.sh"
runScript="./ShellScripts/runHadd.sh"

set -x

outputList=(
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/Pythia8_AllQCDPhoton15_Hydjet_Quenched_Cymbal5Ev8/L1Ntuple_egHOverEcut_EB1_EE1_egEtaCut24/L1Ntuple_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/Pythia8_AllQCDPhoton15_Hydjet_Quenched_Cymbal5Ev8/L1Ntuple_egHOverEcut_EB1_EE1_egEtaCut26/L1Ntuple_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/Pythia8_AllQCDPhoton15_Hydjet_Quenched_Cymbal5Ev8/L1Ntuple_egHOverEcut_EB1_EE1_noEgEtaCut/L1Ntuple_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/Pythia8_AllQCDPhoton30_Hydjet_Quenched_Cymbal5Ev8/L1Ntuple_egHOverEcut_EB1_EE1_egEtaCut24/L1Ntuple_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/Pythia8_EmEnrichedDijet30_Hydjet_Quenched_Cymbal5Ev8/L1Ntuple_egHOverEcut_EB1_EE1_egEtaCut24/L1Ntuple_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/Pythia8_EmEnrichedDijet50_Hydjet_Quenched_Cymbal5Ev8/L1Ntuple_egHOverEcut_EB1_EE1_egEtaCut24/L1Ntuple_merged.root"
);

inputList=(
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/Pythia8_AllQCDPhoton15_Hydjet_Quenched_Cymbal5Ev8/L1Ntuple_egHOverEcut_EB1_EE1_egEtaCut24/180813_234609/0000/L1Ntuple_*.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/Pythia8_AllQCDPhoton15_Hydjet_Quenched_Cymbal5Ev8/L1Ntuple_egHOverEcut_EB1_EE1_egEtaCut26/180813_235822/0000/L1Ntuple_*.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/Pythia8_AllQCDPhoton15_Hydjet_Quenched_Cymbal5Ev8/L1Ntuple_egHOverEcut_EB1_EE1_noEgEtaCut/180813_235553/0000/L1Ntuple_*.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/Pythia8_AllQCDPhoton30_Hydjet_Quenched_Cymbal5Ev8/L1Ntuple_egHOverEcut_EB1_EE1_egEtaCut24/180813_234856/0000/L1Ntuple_*.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/Pythia8_EmEnrichedDijet30_Hydjet_Quenched_Cymbal5Ev8/L1Ntuple_egHOverEcut_EB1_EE1_egEtaCut24/180813_235121/0000/L1Ntuple_*.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/Pythia8_EmEnrichedDijet50_Hydjet_Quenched_Cymbal5Ev8/L1Ntuple_egHOverEcut_EB1_EE1_egEtaCut24/180813_235251/0000/L1Ntuple_*.root"
);

outDirBase=$EWJTAOUT
if [ -z "$outDirBase" ]; then
  outDirBase="/export/d00/scratch/"$USER"/EWJTA-out"
fi
logFileList=(
$outDirBase"/ShellScripts/HIRun2018PbPb/L1T/runHadd_Pythia8_AllQCDPhoton15_Hydjet_Quenched_Cymbal5Ev8_L1Ntuple_egHOverEcut_EB1_EE1_egEtaCut24.log"
$outDirBase"/ShellScripts/HIRun2018PbPb/L1T/runHadd_Pythia8_AllQCDPhoton15_Hydjet_Quenched_Cymbal5Ev8_L1Ntuple_egHOverEcut_EB1_EE1_egEtaCut26.log"
$outDirBase"/ShellScripts/HIRun2018PbPb/L1T/runHadd_Pythia8_AllQCDPhoton15_Hydjet_Quenched_Cymbal5Ev8_L1Ntuple_egHOverEcut_EB1_EE1_noEgEtaCut.log"
$outDirBase"/ShellScripts/HIRun2018PbPb/L1T/runHadd_Pythia8_AllQCDPhoton30_Hydjet_Quenched_Cymbal5Ev8_L1Ntuple_egHOverEcut_EB1_EE1_egEtaCut24.log"
$outDirBase"/ShellScripts/HIRun2018PbPb/L1T/runHadd_Pythia8_EmEnrichedDijet30_Hydjet_Quenched_Cymbal5Ev8_L1Ntuple_egHOverEcut_EB1_EE1_egEtaCut24.log"
$outDirBase"/ShellScripts/HIRun2018PbPb/L1T/runHadd_Pythia8_EmEnrichedDijet50_Hydjet_Quenched_Cymbal5Ev8_L1Ntuple_egHOverEcut_EB1_EE1_egEtaCut24.log"
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

