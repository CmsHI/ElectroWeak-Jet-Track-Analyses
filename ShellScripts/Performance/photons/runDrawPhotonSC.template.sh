#!/bin/bash


runCmd="./ShellScripts/myRun.sh"
progPath="./Performance/photons/drawPhotonSC.exe"
progCode="${progPath/.exe/.C}"
#g++ $progCode $(root-config --cflags --libs) -Werror -Wall -Wextra -Wno-narrowing -lTMVA -lRooFitCore -lRooFit -O2 -o $progPath || exit 1

inputFile="/mnt/hadoop/cms/store/user/katatar/EGamma/Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB/HiFall15-75X_mcRun2_HeavyIon_v14-v1_FOREST_extendEC/171222_204204/0000/HiForestAOD_ggHi_doRecHits_1.root"
entry=454
phoIdx=0
treePath="ggHiNtuplizer/EventTree"
nBinsX=15
nBinsY=43

outDirBase=$EWJTAOUT
if [ -z "$outDirBase" ]; then
  outDirBase="/export/d00/scratch/"$USER"/EWJTA-out"
fi
outputSuffix="Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB"
outList=(
$outDirBase"Performance/photons/drawPhotonSC_"$outputSuffix".root"
);

arrayIndices=${!outList[*]}
for i1 in $arrayIndices
do
    outputFile=${outList[i1]}
    outputFileLOG="${outputFile/.root/.log}"
    outDir=$(dirname "${outputFile}")
    mkdir -p $outDir
    $runCmd $progPath $inputFile $entry $phoIdx $treePath $nBinsX $nBinsY $outputFile &> $outputFileLOG &
    echo "$runCmd $progPath $inputFile $entry $phoIdx $treePath $nBinsX $nBinsY $outputFile &> $outputFileLOG &"
done


