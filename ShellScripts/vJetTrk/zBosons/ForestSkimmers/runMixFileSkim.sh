#!/bin/bash

source ./ShellScripts/bashUtils.sh
source ./ShellScripts/vJetTrk/zBosons/bashUtils.sh

runCmd="./ShellScripts/myRun.sh"
progPath="./ForestSkimmers/vJetTrk/mixFileSkim.exe"
progCode="${progPath/.exe/.C}"
#g++ $progCode $(root-config --cflags --libs) -Werror -Wall -Wextra -Wno-narrowing -lTMVA -lRooFitCore -lRooFit -O2 -o $progPath || exit 1

### USER INPUT START
dirSpecial=""  # name of the directory where special/non-nominal output (if any) would be written
sampleFlags=(
1 # pbpb_2018_mc_mix
0 # pbpb_2018_data_mix_1
0 # pbpb_2018_data_mix_2
0 # pbpb_2018_data_mix
);
### USER INPUT END

inputList=(
${forest_pbpb_2018_mc_mix}
${forest_pbpb_2018_data_mix_1}
${forest_pbpb_2018_data_mix_2}
${forest_pbpb_2018_data_mix}
);

configList=(
"Configurations/vJetTrk/zBoson/mixFileSkim.pbpb.2018.mc.conf"
"Configurations/vJetTrk/zBoson/mixFileSkim.pbpb.2018.data.conf"
"Configurations/vJetTrk/zBoson/mixFileSkim.pbpb.2018.data.conf"
"Configurations/vJetTrk/zBoson/mixFileSkim.pbpb.2018.data.conf"
);

optionList=(
"--jetPtMin=20 --jetEtaMax=1.6 --trkPtMin=0.7 --trkEtaMax=2.4"
"--jetPtMin=20 --jetEtaMax=1.6 --trkPtMin=0.7 --trkEtaMax=2.4"
"--jetPtMin=20 --jetEtaMax=1.6 --trkPtMin=0.7 --trkEtaMax=2.4"
"--jetPtMin=20 --jetEtaMax=1.6 --trkPtMin=0.7 --trkEtaMax=2.4"
);

outDirBase=$EWJTAOUT
if [ -z "$outDirBase" ]; then
  outDirBase="/export/d00/scratch/"$USER"/EWJTA-out"
fi

outList=(
$outDirBase"/"${relDirSkim}"/"${dirSpecial}"/mixFileSkim_pbpb_2018_mc.root"
$outDirBase"/"${relDirSkim}"/"${dirSpecial}"/mixFileSkim_pbpb_2018_data_1.root"
$outDirBase"/"${relDirSkim}"/"${dirSpecial}"/mixFileSkim_pbpb_2018_data_2.root"
$outDirBase"/"${relDirSkim}"/"${dirSpecial}"/mixFileSkim_pbpb_2018_data.root"
);

arrayIndices=${!sampleFlags[*]}
for i1 in $arrayIndices
do
  if [ ${sampleFlags[i1]} == 1 ]; then

    configFile=${configList[i1]}
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
