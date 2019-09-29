#!/bin/bash

source ./ShellScripts/bashUtils.sh
source ./ShellScripts/vJetTrk/zBosons/bashUtils.sh

runCmd="./ShellScripts/myRun.sh"
progPath="./Histogramming/vJetTrk/trkSpectra.exe"
progCode="${progPath/.exe/.C}"
#g++ $progCode $(root-config --cflags --libs) -Werror -Wall -Wextra -Wno-narrowing -lTMVA -lRooFitCore -lRooFit -O2 -o $progPath || exit 1

### USER INPUT START
dirSpecial=""  # name of the directory where special/non-nominal output (if any) would be written
sampleFlags=(
1 # pbpb_2018_mc_mix
1 # pbpb_2018_mc_zmm
1 # pbpb_2018_mc_zee
0 # pbpb_2018_data_mix
);
### USER INPUT END

inputList=(
${forest_pbpb_2018_mc_mix}
${forest_pbpb_2018_mc_zmm}
${forest_pbpb_2018_mc_zee}
${forest_pbpb_2018_data_mix}
);

optionList=(
"--sampleType=pbpb_2018_mc"
"--sampleType=pbpb_2018_mc"
"--sampleType=pbpb_2018_mc"
"--sampleType=pbpb_2018_data"
);

outDirBase=$EWJTAOUT
if [ -z "$outDirBase" ]; then
  outDirBase="/export/d00/scratch/"$USER"/EWJTA-out"
fi

outList=(
$outDirBase"/"${relDirHist}"/"${dirSpecial}"/trkSpectra_pbpb_2018_mc_mix.root"
$outDirBase"/"${relDirHist}"/"${dirSpecial}"/trkSpectra_pbpb_2018_mc_zmm.root"
$outDirBase"/"${relDirHist}"/"${dirSpecial}"/trkSpectra_pbpb_2018_mc_zee.root"
$outDirBase"/"${relDirHist}"/"${dirSpecial}"/trkSpectra_pbpb_2018_data_mix.root"
);

arrayIndices=${!sampleFlags[*]}
for i1 in $arrayIndices
do
  if [ ${sampleFlags[i1]} == 1 ]; then

    configFile="DUMMY"
    inputFile=${inputList[i1]}
    options=${optionList[i1]}
    outputFile=${outList[i1]}
    outputFileLOG="${outputFile/.root/.log}"
    outDir=$(dirname "${outputFile}")
    mkdir -p $outDir

    $runCmd $progPath $configFile $inputFile $outputFile ${options} &> $outputFileLOG &
    echo "$runCmd $progPath $configFile $inputFile $outputFile ${options} &> $outputFileLOG &"

  fi
done
