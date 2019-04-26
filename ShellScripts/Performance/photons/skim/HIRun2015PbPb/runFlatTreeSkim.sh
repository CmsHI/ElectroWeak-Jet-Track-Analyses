#!/bin/bash

runCmd="./ShellScripts/myRun.sh"
progPath="./Performance/objects/flatTreeSkim.exe"
progCode="${progPath/.exe/.C}"
#g++ $progCode $(root-config --cflags --libs) -Werror -Wall -Wno-narrowing -lTMVA -lRooFitCore -lRooFit -O2 -o $progPath || exit 1

inputList=(
"Configurations/filelists/Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST.list"
);

configList=(
"Configurations/Performance/photons/skim/HIRun2015PbPb/flatTreeSkim.island.conf"
);

outDirBase=$EWJTAOUT
if [ -z "$outDirBase" ]; then
  outDirBase="/export/d00/scratch/"$USER"/EWJTA-out"
fi
outList=(
$outDirBase"/Performance/photons/skim/HIRun2015PbPb/flatTreeSkim_island_Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB_HINPbPbWinter16DR.root"
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
    rm -rf $outputFile
    $runCmd $progPath $configFile $inputFile $outputFile &> $outputFileLOG &
    echo "$runCmd $progPath $configFile $inputFile $outputFile &> $outputFileLOG &"
done

