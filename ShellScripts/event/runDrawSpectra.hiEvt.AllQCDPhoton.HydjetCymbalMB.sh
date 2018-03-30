#!/bin/bash

#g++ -Wall -Werror -Wextra -O2 -Wno-narrowing `root-config --cflags --libs` -lTMVA -lRooFitCore -lRooFit -MMD -MF ./build/Performance/drawSpectra.d Performance/drawSpectra.C -o Performance/drawSpectra.exe

runCmd="./ShellScripts/myRun.sh"
progPath="./Performance/drawSpectra.exe"
inputFiles=(
"Configurations/filelists/Pythia8_AllQCDPhoton80Flt30_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST.list"
"Configurations/filelists/Hydjet_Quenched_Cymbal5Ev8_PbPbMinBias_5020GeV.list"
);

configFiles=(
"Configurations/event/spectra.hiEvt.conf"
"Configurations/event/spectra.hiEvt.conf"
);

outDirBase=$EWJTAOUT
if [ -z "$outDirBase" ]; then
  outDirBase="/export/d00/scratch/"$USER"/EWJTA-out"
fi
outList=(
$outDirBase"/Configurations/event/spectra_hiEvt_AllQCDPhoton80Flt30_Hydjet_Cymbal_MB_HINPbPbWinter16DR.root"
$outDirBase"/Configurations/event/spectra_hiEvt_Hydjet_Quenched_Cymbal5Ev8_PbPbMinBias_5020GeV.root"
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


