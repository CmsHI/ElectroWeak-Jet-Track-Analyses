#!/bin/bash

set -x

g++ Plotting/photonJetFF/plotPhotonJetFF.C $(root-config --cflags --libs) -Werror -Wall -Wextra -Wno-narrowing -O2 -o Plotting/photonJetFF/plotPhotonJetFF.exe || exit 1

progPath="./Plotting/photonJetFF/plotPhotonJetFF.exe"
inputFiles=(
"./Data/photonJetFF/data_60_30_gxi0_defnFF1-final-and-systematics.root"
"./Data/photonJetFF/data_60_30_gxi1_defnFF1-final-and-systematics.root"
);

histLists=(
"Configurations/photonJetFF/ff-gxi0-theory.list"
"Configurations/photonJetFF/ff-gxi1-theory.list"
);

configList=(
"Configurations/photonJetFF/ff-60-30-gxi0-theory.conf"
"Configurations/photonJetFF/ff-60-30-gxi1-theory.conf"
);

inputFileTheory="./Data/photonJetFF/model_photonJetFF_5TeV.root"

arrayIndices=${!configList[*]}
for i1 in $arrayIndices
do
    configFile=${configList[i1]}
    outputFileLOG="${configFile/.conf/.log}"
    inputFileTmp=${inputFiles[i1]}
    # create the input file on the fly. That file will contain both data and theory.
    inputFile="${inputFileTmp/.root/-and-theory.root}"
    hadd -f $inputFile $inputFileTmp $inputFileTheory
    histList=${histLists[i1]}
    $progPath $inputFile $histList $configFile &> $outputFileLOG
    rm $inputFile # the file with both data and theory is not to be kept.
done

