#!/bin/bash

#g++ -Wall -Werror -Wextra -O2 -Wno-narrowing `root-config --cflags --libs` -lTMVA -lRooFitCore -lRooFit -MMD -MF ./build/Performance/drawSpectra.d Performance/drawSpectra.C -o Performance/drawSpectra.exe

runCmd="./ShellScripts/myRun.sh"
progPath="./Performance/drawTree.exe"
inputFiles=(
"Configurations/filelists/official/HIRun2017PP/QCD_pThat_30_EMEnrichedDijet_TuneCP5_5p02TeV_pythia8_RunIIpp5Spring18DR_94X_FOREST_unmerged.list"
"Configurations/filelists/official/HIRun2017PP/QCD_pThat_50_EMEnrichedDijet_TuneCP5_5p02TeV_pythia8_RunIIpp5Spring18DR_94X_FOREST_unmerged.list"
"Configurations/filelists/official/HIRun2017PP/QCD_pThat_80_EMEnrichedDijet_TuneCP5_5p02TeV_pythia8_RunIIpp5Spring18DR_94X_FOREST_unmerged.list"
"Configurations/filelists/official/HIRun2017PP/QCD_pThat_120_EMEnrichedDijet_TuneCP5_5p02TeV_pythia8_RunIIpp5Spring18DR_94X_FOREST_unmerged.list"
"Configurations/filelists/official/HIRun2017PP/QCD_pThat_170_EMEnrichedDijet_TuneCP5_5p02TeV_pythia8_RunIIpp5Spring18DR_94X_FOREST_unmerged.list"
"Configurations/filelists/official/HIRun2017PP/QCD_pThat_220_EMEnrichedDijet_TuneCP5_5p02TeV_pythia8_RunIIpp5Spring18DR_94X_FOREST_unmerged.list"
"Configurations/filelists/official/HIRun2017PP/QCD_pThat_XX_EMEnrichedDijet_TuneCP5_5p02TeV_pythia8_RunIIpp5Spring18DR_94X_FOREST_unmerged.list"
);

configFiles=(
"Configurations/event/HIRun2017PP/Pythia8_EMEnrichedDijet_RunIIpp5Spring18DR/spectra.pthat.conf"
"Configurations/event/HIRun2017PP/Pythia8_EMEnrichedDijet_RunIIpp5Spring18DR/spectra.pthat.conf"
"Configurations/event/HIRun2017PP/Pythia8_EMEnrichedDijet_RunIIpp5Spring18DR/spectra.pthat.conf"
"Configurations/event/HIRun2017PP/Pythia8_EMEnrichedDijet_RunIIpp5Spring18DR/spectra.pthat.conf"
"Configurations/event/HIRun2017PP/Pythia8_EMEnrichedDijet_RunIIpp5Spring18DR/spectra.pthat.conf"
"Configurations/event/HIRun2017PP/Pythia8_EMEnrichedDijet_RunIIpp5Spring18DR/spectra.pthat.conf"
"Configurations/event/HIRun2017PP/Pythia8_EMEnrichedDijet_RunIIpp5Spring18DR/spectra.pthat.conf"
);

outDirBase=$EWJTAOUT
if [ -z "$outDirBase" ]; then
  outDirBase="/export/d00/scratch/"$USER"/EWJTA-out"
fi
outList=(
$outDirBase"/Configurations/event/HIRun2017PP/spectra_pthat_EMEnrichedDijet30_TuneCP5_5p02TeV_pythia8_RunIIpp5Spring18DR.root"
$outDirBase"/Configurations/event/HIRun2017PP/spectra_pthat_EMEnrichedDijet50_TuneCP5_5p02TeV_pythia8_RunIIpp5Spring18DR.root"
$outDirBase"/Configurations/event/HIRun2017PP/spectra_pthat_EMEnrichedDijet80_TuneCP5_5p02TeV_pythia8_RunIIpp5Spring18DR.root"
$outDirBase"/Configurations/event/HIRun2017PP/spectra_pthat_EMEnrichedDijet120_TuneCP5_5p02TeV_pythia8_RunIIpp5Spring18DR.root"
$outDirBase"/Configurations/event/HIRun2017PP/spectra_pthat_EMEnrichedDijet170_TuneCP5_5p02TeV_pythia8_RunIIpp5Spring18DR.root"
$outDirBase"/Configurations/event/HIRun2017PP/spectra_pthat_EMEnrichedDijet220_TuneCP5_5p02TeV_pythia8_RunIIpp5Spring18DR.root"
$outDirBase"/Configurations/event/HIRun2017PP/spectra_pthat_EMEnrichedDijetXX_TuneCP5_5p02TeV_pythia8_RunIIpp5Spring18DR.root"
);

arrayIndices=${!outList[*]}
for i1 in $arrayIndices
do
    inputFile=${inputFiles[i1]}
    configFile=${configFiles[i1]}
    outputFile=${outList[i1]}
    outputFileLOG="${outputFile/.root/.log}"
    outDir=$(dirname "${outputFile}")
    mkdir -p $outDir
    $runCmd $progPath $configFile $inputFile $outputFile &> $outputFileLOG &
    echo "$runCmd $progPath $configFile $inputFile $outputFile &> $outputFileLOG &"
done


