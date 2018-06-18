#!/bin/bash

runCmd="./ShellScripts/myRun.sh"
progPath="./Performance/drawSpectra.exe"
progCode="${progPath/.exe/.C}"
#g++ $progCode $(root-config --cflags --libs) -Werror -Wall -Wno-narrowing -lTMVA -lRooFitCore -lRooFit -O2 -o $progPath || exit 1

inputList=(
"Configurations/filelists/Hydjet_Quenched_Cymbal5Ev8_PbPbMinBias_5020GeV.list"
);

configList=(
"Configurations/genparticle/spectra.ngen.conf"
);

outDirBase=$EWJTAOUT
if [ -z "$outDirBase" ]; then
  outDirBase="/export/d00/scratch/"$USER"/EWJTA-out"
fi
outputSuffix="Hydjet_Quenched_Cymbal5Ev8_PbPbMinBias_5020GeV"
outList=(
$outDirBase"/Configurations/genparticle/spectra_ngen_"$outputSuffix".root"
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
    $runCmd $progPath $configFile $inputFile $outputFile &> $outputFileLOG &
    echo "$runCmd $progPath $configFile $inputFile $outputFile &> $outputFileLOG &"
done


