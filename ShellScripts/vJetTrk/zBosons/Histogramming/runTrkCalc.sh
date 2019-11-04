#!/bin/bash

source ./ShellScripts/bashUtils.sh
source ./ShellScripts/vJetTrk/zBosons/bashUtils.sh

runCmd="./ShellScripts/myRun.sh"
progPath="./Histogramming/vJetTrk/trkCalc.exe"
progCode="${progPath/.exe/.C}"
#g++ $progCode $(root-config --cflags --libs) -Werror -Wall -Wextra -Wno-narrowing -lTMVA -lRooFitCore -lRooFit -O2 -o $progPath || exit 1

### USER INPUT START
dirSpecial=""  # name of the directory where special/non-nominal output (if any) would be written
### USER INPUT END

inputDirBase=$EWJTAOUT
if [ -z "$inputDirBase" ]; then
  inputDirBase="/export/d00/scratch/"$USER"/EWJTA-out"
fi

inputList=(
$inputDirBase"/"${relDirHist}"/"${dirSpecial}"/trkSpectra_pbpb_2018_mc_mix_v8.root"
$inputDirBase"/"${relDirHist}"/"${dirSpecial}"/trkSpectra_pbpb_2018_mc_zmm_v8.root"
$inputDirBase"/"${relDirHist}"/"${dirSpecial}"/trkSpectra_pbpb_2018_mc_zee_v8.root"
);

outDirBase=$EWJTAOUT
if [ -z "$outDirBase" ]; then
  outDirBase="/export/d00/scratch/"$USER"/EWJTA-out"
fi

outList=(
$outDirBase"/"${relDirHist}"/"${dirSpecial}"/trkCalc_pbpb_2018_mc_mix_v8.root"
$outDirBase"/"${relDirHist}"/"${dirSpecial}"/trkCalc_pbpb_2018_mc_zmm_v8.root"
$outDirBase"/"${relDirHist}"/"${dirSpecial}"/trkCalc_pbpb_2018_mc_zee_v8.root"
);

arrayIndices=${!outList[*]}
for i1 in $arrayIndices
do
    inputFile=${inputList[i1]}
    outputFile=${outList[i1]}
    outputFileLOG="${outputFile/.root/.log}"
    outDir=$(dirname "${outputFile}")
    mkdir -p $outDir

    $runCmd $progPath $inputFile $outputFile &> $outputFileLOG &
    echo "$runCmd $progPath $inputFile $outputFile &> $outputFileLOG &"

done
